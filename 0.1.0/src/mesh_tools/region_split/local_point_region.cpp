// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "local_point_region.hpp"
#include "sync_tools.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "global_index.hpp"
#include "indirect_primitive_patch.hpp"
#include "dummy_transform.hpp"
#include "pstream_reduce_ops.hpp"
#include "processor_poly_patch.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(localPointRegion, 0);
// Reduction class to get minimum value over face.
class minEqOpFace
{
public:
  void operator()(face& x, const face& y) const
  {
    if (x.size())
    {
      label j = 0;
      FOR_ALL(x, i)
      {
        x[i] = min(x[i], y[j]);
        j = y.rcIndex(j);
      }
    }
  }
};
}
// Private Member Functions
// Are two lists identical either in forward or in reverse order.
bool mousse::localPointRegion::isDuplicate
(
  const face& f0,
  const face& f1,
  const bool forward
)
{
  label fp1 = findIndex(f1, f0[0]);
  if (fp1 == -1)
  {
    return false;
  }
  FOR_ALL(f0, fp0)
  {
    if (f0[fp0] != f1[fp1])
    {
      return false;
    }
    if (forward)
    {
      fp1 = f1.fcIndex(fp1);
    }
    else
    {
      fp1 = f1.rcIndex(fp1);
    }
  }
  return true;
}
// Count regions per point
void mousse::localPointRegion::countPointRegions
(
  const polyMesh& mesh,
  const boolList& candidatePoint,
  const Map<label>& candidateFace,
  faceList& minRegion
)
{
  // Almost all will have only one so only
  // populate Map if more than one.
  labelList minPointRegion(mesh.nPoints(), -1);
  // From global point to local (multi-region) point numbering
  meshPointMap_.resize(candidateFace.size()/100);
  // From local (multi-region) point to regions
  DynamicList<labelList> pointRegions(meshPointMap_.size());
  // From faces with any duplicated point on it to local face
  meshFaceMap_.resize(meshPointMap_.size());
  FOR_ALL_CONST_ITER(Map<label>, candidateFace, iter)
  {
    label faceI = iter.key();
    if (!mesh.isInternalFace(faceI))
    {
      const face& f = mesh.faces()[faceI];
      if (minRegion[faceI].empty())
      {
        FATAL_ERROR_IN("localPointRegion::countPointRegions(..)")
          << "Face from candidateFace without minRegion set." << endl
          << "Face:" << faceI << " fc:" << mesh.faceCentres()[faceI]
          << " verts:" << f << abort(FatalError);
      }
      FOR_ALL(f, fp)
      {
        label pointI = f[fp];
        // Even points which were not candidates for splitting might
        // be on multiple baffles that are being split so check.
        if (candidatePoint[pointI])
        {
          label region = minRegion[faceI][fp];
          if (minPointRegion[pointI] == -1)
          {
            minPointRegion[pointI] = region;
          }
          else if (minPointRegion[pointI] != region)
          {
            // Multiple regions for this point. Add.
            Map<label>::iterator iter = meshPointMap_.find(pointI);
            if (iter != meshPointMap_.end())
            {
              labelList& regions = pointRegions[iter()];
              if (findIndex(regions, region) == -1)
              {
                label sz = regions.size();
                regions.setSize(sz+1);
                regions[sz] = region;
              }
            }
            else
            {
              label localPointI = meshPointMap_.size();
              meshPointMap_.insert(pointI, localPointI);
              labelList regions(2);
              regions[0] = minPointRegion[pointI];
              regions[1] = region;
              pointRegions.append(regions);
            }
            label meshFaceMapI = meshFaceMap_.size();
            meshFaceMap_.insert(faceI, meshFaceMapI);
          }
        }
      }
    }
  }
  minPointRegion.clear();
  // Add internal faces that use any duplicated point. Can only have one
  // region!
  FOR_ALL_CONST_ITER(Map<label>, candidateFace, iter)
  {
    label faceI = iter.key();
    if (mesh.isInternalFace(faceI))
    {
      const face& f = mesh.faces()[faceI];
      FOR_ALL(f, fp)
      {
        // Note: candidatePoint test not really necessary but
        // speeds up rejection.
        if (candidatePoint[f[fp]] && meshPointMap_.found(f[fp]))
        {
          label meshFaceMapI = meshFaceMap_.size();
          meshFaceMap_.insert(faceI, meshFaceMapI);
        }
      }
    }
  }
  // Transfer to member data
  pointRegions.shrink();
  pointRegions_.setSize(pointRegions.size());
  FOR_ALL(pointRegions, i)
  {
    pointRegions_[i].transfer(pointRegions[i]);
  }
  // Compact minRegion
  faceRegions_.setSize(meshFaceMap_.size());
  FOR_ALL_CONST_ITER(Map<label>, meshFaceMap_, iter)
  {
    faceRegions_[iter()].labelList::transfer(minRegion[iter.key()]);
    //// Print a bit
    //{
    //    label faceI = iter.key();
    //    const face& f = mesh.faces()[faceI];
    //    Pout<< "Face:" << faceI << " fc:" << mesh.faceCentres()[faceI]
    //        << " verts:" << f << endl;
    //    FOR_ALL(f, fp)
    //    {
    //        Pout<< "    " << f[fp] << " min:" << faceRegions_[iter()][fp]
    //            << endl;
    //    }
    //    Pout<< endl;
    //}
  }
  // Compact region numbering
  // ? TBD.
}
void mousse::localPointRegion::calcPointRegions
(
  const polyMesh& mesh,
  boolList& candidatePoint
)
{
  label nBnd = mesh.nFaces()-mesh.nInternalFaces();
  const labelList& faceOwner = mesh.faceOwner();
  const labelList& faceNeighbour = mesh.faceNeighbour();
  syncTools::syncPointList
  (
    mesh,
    candidatePoint,
    orEqOp<bool>(),
    false               // nullValue
  );
  // Mark any face/boundaryFace/cell with a point on a candidate point.
  // - candidateFace does not necessary have to be a baffle!
  // - candidateFace is synchronised (since candidatePoint is)
  Map<label> candidateFace(2*nBnd);
  label candidateFaceI = 0;
  Map<label> candidateCell(nBnd);
  label candidateCellI = 0;
  FOR_ALL(mesh.faces(), faceI)
  {
    const face& f = mesh.faces()[faceI];
    FOR_ALL(f, fp)
    {
      if (candidatePoint[f[fp]])
      {
        // Mark face
        if (candidateFace.insert(faceI, candidateFaceI))
        {
          candidateFaceI++;
        }
        // Mark cells
        if (candidateCell.insert(faceOwner[faceI], candidateCellI))
        {
          candidateCellI++;
        }
        if (mesh.isInternalFace(faceI))
        {
          label nei = faceNeighbour[faceI];
          if (candidateCell.insert(nei, candidateCellI))
          {
            candidateCellI++;
          }
        }
        break;
      }
    }
  }
  // Get global indices for cells
  globalIndex globalCells(mesh.nCells());
  // Determine for every candidate face per point the minimum region
  // (global cell) it is connected to. (candidateFaces are the
  // only ones using a
  // candidate point so the only ones that can be affected)
  faceList minRegion(mesh.nFaces());
  FOR_ALL_CONST_ITER(Map<label>, candidateFace, iter)
  {
    label faceI = iter.key();
    const face& f = mesh.faces()[faceI];
    if (mesh.isInternalFace(faceI))
    {
      label globOwn = globalCells.toGlobal(faceOwner[faceI]);
      label globNei = globalCells.toGlobal(faceNeighbour[faceI]);
      minRegion[faceI].setSize(f.size(), min(globOwn, globNei));
    }
    else
    {
      label globOwn = globalCells.toGlobal(faceOwner[faceI]);
      minRegion[faceI].setSize(f.size(), globOwn);
    }
  }
  // Now minimize over all faces that are connected through internal
  // faces or through cells. This loop iterates over the max number of
  // cells connected to a point (=8 for hex mesh)
  while (true)
  {
    // Transport minimum from face across cell
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Map<label> minPointValue(100);
    label nChanged = 0;
    FOR_ALL_CONST_ITER(Map<label>, candidateCell, iter)
    {
      minPointValue.clear();
      label cellI = iter.key();
      const cell& cFaces = mesh.cells()[cellI];
      // Determine minimum per point
      FOR_ALL(cFaces, cFaceI)
      {
        label faceI = cFaces[cFaceI];
        if (minRegion[faceI].size())
        {
          const face& f = mesh.faces()[faceI];
          FOR_ALL(f, fp)
          {
            label pointI = f[fp];
            Map<label>::iterator iter = minPointValue.find(pointI);
            if (iter == minPointValue.end())
            {
              minPointValue.insert(pointI, minRegion[faceI][fp]);
            }
            else
            {
              label currentMin = iter();
              iter() = min(currentMin, minRegion[faceI][fp]);
            }
          }
        }
      }
      // Set face minimum from point minimum
      FOR_ALL(cFaces, cFaceI)
      {
        label faceI = cFaces[cFaceI];
        if (minRegion[faceI].size())
        {
          const face& f = mesh.faces()[faceI];
          FOR_ALL(f, fp)
          {
            label minVal = minPointValue[f[fp]];
            if (minVal != minRegion[faceI][fp])
            {
              minRegion[faceI][fp] = minVal;
              nChanged++;
            }
          }
        }
      }
    }
    //Pout<< "nChanged:" << nChanged << endl;
    if (returnReduce(nChanged, sumOp<label>()) == 0)
    {
      break;
    }
    // Transport minimum across coupled faces
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    SubList<face> l
    (
      minRegion,
      mesh.nFaces()-mesh.nInternalFaces(),
      mesh.nInternalFaces()
    );
    syncTools::syncBoundaryFaceList
    (
      mesh,
      l,
      minEqOpFace(),
      mousse::dummyTransform()  // dummy transformation
    );
  }
  // Count regions per point
  countPointRegions(mesh, candidatePoint, candidateFace, minRegion);
  minRegion.clear();
  //// Print points with multiple regions. These points need to be duplicated.
  //FOR_ALL_CONST_ITER(Map<label>, meshPointMap_, iter)
  //{
  //    Pout<< "point:" << iter.key()
  //        << " coord:" << mesh.points()[iter.key()]
  //        << " regions:" << pointRegions_[iter()] << endl;
  //}
}
// Constructors
mousse::localPointRegion::localPointRegion(const polyMesh& mesh)
:
  //nRegions_(0),
  meshPointMap_(0),
  pointRegions_(0),
  meshFaceMap_(0),
  faceRegions_(0)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Get any point on the outside which is on a non-coupled boundary
  boolList candidatePoint(mesh.nPoints(), false);
  FOR_ALL(patches, patchI)
  {
    if (!patches[patchI].coupled())
    {
      const polyPatch& pp = patches[patchI];
      FOR_ALL(pp.meshPoints(), i)
      {
        candidatePoint[pp.meshPoints()[i]] = true;
      }
    }
  }
  calcPointRegions(mesh, candidatePoint);
}
mousse::localPointRegion::localPointRegion
(
  const polyMesh& mesh,
  const labelList& candidatePoints
)
:
  //nRegions_(0),
  meshPointMap_(0),
  pointRegions_(0),
  meshFaceMap_(0),
  faceRegions_(0)
{
  boolList candidatePoint(mesh.nPoints(), false);
  FOR_ALL(candidatePoints, i)
  {
    candidatePoint[candidatePoints[i]] = true;
  }
  calcPointRegions(mesh, candidatePoint);
}

// Member Functions
// Return a list (in allPatch indices) with either -1 or the face label
// of the face that uses the same vertices.
mousse::labelList mousse::localPointRegion::findDuplicateFaces
(
  const primitiveMesh& mesh,
  const labelList& boundaryFaces
)
{
  // Addressing engine for all boundary faces.
  indirectPrimitivePatch allPatch
  (
    IndirectList<face>(mesh.faces(), boundaryFaces),
    mesh.points()
  );
  labelList duplicateFace(allPatch.size(), -1);
  label nDuplicateFaces = 0;
  // Find all duplicate faces.
  FOR_ALL(allPatch, bFaceI)
  {
    const face& f = allPatch.localFaces()[bFaceI];
    // Get faces connected to f[0].
    // Check whether share all points with f.
    const labelList& pFaces = allPatch.pointFaces()[f[0]];
    FOR_ALL(pFaces, i)
    {
      label otherFaceI = pFaces[i];
      if (otherFaceI > bFaceI)
      {
        const face& otherF = allPatch.localFaces()[otherFaceI];
        if (isDuplicate(f, otherF, true))
        {
          FATAL_ERROR_IN
          (
            "findDuplicateFaces(const primitiveMesh&"
            ", const labelList&)"
          )   << "Face:" << bFaceI + mesh.nInternalFaces()
            << " has local points:" << f
            << " which are in same order as face:"
            << otherFaceI + mesh.nInternalFaces()
            << " with local points:" << otherF
            << abort(FatalError);
        }
        else if (isDuplicate(f, otherF, false))
        {
          label meshFace0 = bFaceI + mesh.nInternalFaces();
          label meshFace1 = otherFaceI + mesh.nInternalFaces();
          if
          (
            duplicateFace[bFaceI] != -1
          || duplicateFace[otherFaceI] != -1
          )
          {
            FATAL_ERROR_IN
            (
              "findDuplicateFaces(const primitiveMesh&"
              ", const labelList&)"
            )   << "One of two duplicate faces already marked"
              << " as duplicate." << nl
              << "This means that three or more faces share"
              << " the same points and this is illegal." << nl
              << "Face:" << meshFace0
              << " with local points:" << f
              << " which are in same order as face:"
              << meshFace1
              << " with local points:" << otherF
              << abort(FatalError);
          }
          duplicateFace[bFaceI] = otherFaceI;
          duplicateFace[otherFaceI] = bFaceI;
          nDuplicateFaces++;
        }
      }
    }
  }
  return duplicateFace;
}
mousse::List<mousse::labelPair> mousse::localPointRegion::findDuplicateFacePairs
(
  const polyMesh& mesh
)
{
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  // Faces to test: all boundary faces
  labelList testFaces
  (
    identity(mesh.nFaces()-mesh.nInternalFaces())
   + mesh.nInternalFaces()
  );
  // Find correspondencing baffle face (or -1)
  const labelList duplicateFace(findDuplicateFaces(mesh, testFaces));
  // Convert into list of coupled face pairs (mesh face labels).
  DynamicList<labelPair> baffles(testFaces.size());
  FOR_ALL(duplicateFace, i)
  {
    label otherFaceI = duplicateFace[i];
    if (otherFaceI != -1 && i < otherFaceI)
    {
      label meshFace0 = testFaces[i];
      label patch0 = patches.whichPatch(meshFace0);
      label meshFace1 = testFaces[otherFaceI];
      label patch1 = patches.whichPatch(meshFace1);
      // Check for illegal topology. Should normally not happen!
      if
      (
        (patch0 != -1 && isA<processorPolyPatch>(patches[patch0]))
      || (patch1 != -1 && isA<processorPolyPatch>(patches[patch1]))
      )
      {
        FATAL_ERROR_IN
        (
          "localPointRegion::findDuplicateFacePairs(const polyMesh&)"
        )   << "One of two duplicate faces is on"
          << " processorPolyPatch."
          << "This is not allowed." << nl
          << "Face:" << meshFace0
          << " is on patch:" << patches[patch0].name()
          << nl
          << "Face:" << meshFace1
          << " is on patch:" << patches[patch1].name()
          << abort(FatalError);
      }
      baffles.append(labelPair(meshFace0, meshFace1));
    }
  }
  return baffles.shrink();
}
void mousse::localPointRegion::updateMesh(const mapPolyMesh& map)
{
  {
    Map<label> newMap(meshFaceMap_.size());
    FOR_ALL_CONST_ITER(Map<label>, meshFaceMap_, iter)
    {
      label newFaceI = map.reverseFaceMap()[iter.key()];
      if (newFaceI >= 0)
      {
        newMap.insert(newFaceI, iter());
      }
    }
    meshFaceMap_.transfer(newMap);
  }
  {
    Map<label> newMap(meshPointMap_.size());
    FOR_ALL_CONST_ITER(Map<label>, meshPointMap_, iter)
    {
      label newPointI = map.reversePointMap()[iter.key()];
      if (newPointI >= 0)
      {
        newMap.insert(newPointI, iter());
      }
    }
    meshPointMap_.transfer(newMap);
  }
}
