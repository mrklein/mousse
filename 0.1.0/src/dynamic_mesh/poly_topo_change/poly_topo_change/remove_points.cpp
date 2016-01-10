// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bi_indirect_list.hpp"
#include "remove_points.hpp"
#include "pstream_reduce_ops.hpp"
#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"
#include "poly_remove_point.hpp"
#include "poly_add_point.hpp"
#include "poly_modify_face.hpp"
#include "sync_tools.hpp"
#include "face_set.hpp"
#include "dummy_transform.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(removePoints, 0);
//- Combine-reduce operator to combine data on faces. Takes care
//  of reverse orientation on coupled face.
template<class T, template<class> class CombineOp>
class faceEqOp
{
public:
  void operator()(List<T>& x, const List<T>& y) const
  {
    if (y.size() > 0)
    {
      if (x.size() == 0)
      {
        x = y;
      }
      else
      {
        label j = 0;
        FOR_ALL(x, i)
        {
          CombineOp<T>()(x[i], y[j]);
          j = y.rcIndex(j);
        }
      }
    }
  }
};
//// Dummy transform for List. Used in synchronisation.
//template<class T>
//class dummyTransformList
//{
//public:
//    void operator()(const coupledPolyPatch&, Field<List<T> >&) const
//    {}
//};
//// Dummy template specialisation. Used in synchronisation.
//template<>
//class pTraits<boolList>
//{
//public:
//
//    //- Component type
//    typedef label cmptType;
//};
}
// Private Member Functions 
// Change the vertices of the face whilst keeping everything else the same.
void mousse::removePoints::modifyFace
(
  const label faceI,
  const face& newFace,
  polyTopoChange& meshMod
) const
{
  // Get other face data.
  label patchI = -1;
  label owner = mesh_.faceOwner()[faceI];
  label neighbour = -1;
  if (mesh_.isInternalFace(faceI))
  {
    neighbour = mesh_.faceNeighbour()[faceI];
  }
  else
  {
    patchI = mesh_.boundaryMesh().whichPatch(faceI);
  }
  label zoneID = mesh_.faceZones().whichZone(faceI);
  bool zoneFlip = false;
  if (zoneID >= 0)
  {
    const faceZone& fZone = mesh_.faceZones()[zoneID];
    zoneFlip = fZone.flipMap()[fZone.whichFace(faceI)];
  }
  meshMod.setAction
  (
    polyModifyFace
    (
      newFace,        // modified face
      faceI,          // label of face being modified
      owner,          // owner
      neighbour,      // neighbour
      false,          // face flip
      patchI,         // patch for face
      false,          // remove from zone
      zoneID,         // zone for face
      zoneFlip        // face flip in zone
    )
  );
}
// Constructors 
// Construct from mesh
mousse::removePoints::removePoints
(
  const polyMesh& mesh,
  const bool undoable
)
:
  mesh_(mesh),
  undoable_(undoable)
{}
// Member Functions 
mousse::label mousse::removePoints::countPointUsage
(
  const scalar minCos,
  boolList& pointCanBeDeleted
) const
{
  // Containers to store two edges per point:
  // -1   : not filled
  // >= 0 : edge label
  // -2   : more than two edges for point
  labelList edge0(mesh_.nPoints(), -1);
  labelList edge1(mesh_.nPoints(), -1);
  const edgeList& edges = mesh_.edges();
  FOR_ALL(edges, edgeI)
  {
    const edge& e = edges[edgeI];
    FOR_ALL(e, eI)
    {
      label pointI = e[eI];
      if (edge0[pointI] == -2)
      {
        // Already too many edges
      }
      else if (edge0[pointI] == -1)
      {
        // Store first edge using point
        edge0[pointI] = edgeI;
      }
      else
      {
        // Already one edge using point. Check second container.
        if (edge1[pointI] == -1)
        {
          // Store second edge using point
          edge1[pointI] = edgeI;
        }
        else
        {
          // Third edge using point. Mark.
          edge0[pointI] = -2;
          edge1[pointI] = -2;
        }
      }
    }
  }
  // Check the ones used by only 2 edges that these are sufficiently in line.
  const pointField& points = mesh_.points();
  pointCanBeDeleted.setSize(mesh_.nPoints());
  pointCanBeDeleted = false;
  //label nDeleted = 0;
  FOR_ALL(edge0, pointI)
  {
    if (edge0[pointI] >= 0 && edge1[pointI] >= 0)
    {
      // Point used by two edges exactly
      const edge& e0 = edges[edge0[pointI]];
      const edge& e1 = edges[edge1[pointI]];
      label common = e0.commonVertex(e1);
      label vLeft = e0.otherVertex(common);
      label vRight = e1.otherVertex(common);
      vector e0Vec = points[common] - points[vLeft];
      e0Vec /= mag(e0Vec) + VSMALL;
      vector e1Vec = points[vRight] - points[common];
      e1Vec /= mag(e1Vec) + VSMALL;
      if ((e0Vec & e1Vec) > minCos)
      {
        pointCanBeDeleted[pointI] = true;
        //nDeleted++;
      }
    }
    else if (edge0[pointI] == -1)
    {
      // point not used at all
      pointCanBeDeleted[pointI] = true;
      //nDeleted++;
    }
  }
  edge0.clear();
  edge1.clear();
  // Protect any points on faces that would collapse down to nothing
  // No particular intelligence so might protect too many points
  FOR_ALL(mesh_.faces(), faceI)
  {
    const face& f = mesh_.faces()[faceI];
    label nCollapse = 0;
    FOR_ALL(f, fp)
    {
      if (pointCanBeDeleted[f[fp]])
      {
        nCollapse++;
      }
    }
    if ((f.size() - nCollapse) < 3)
    {
      // Just unmark enough points
      FOR_ALL(f, fp)
      {
        if (pointCanBeDeleted[f[fp]])
        {
          pointCanBeDeleted[f[fp]] = false;
          --nCollapse;
          if (nCollapse == 0)
          {
            break;
          }
        }
      }
    }
  }
  // Point can be deleted only if all processors want to delete it
  syncTools::syncPointList
  (
    mesh_,
    pointCanBeDeleted,
    andEqOp<bool>(),
    true                // null value
  );
  label nDeleted = 0;
  FOR_ALL(pointCanBeDeleted, pointI)
  {
    if (pointCanBeDeleted[pointI])
    {
      nDeleted++;
    }
  }
  return returnReduce(nDeleted, sumOp<label>());
}
void mousse::removePoints::setRefinement
(
  const boolList& pointCanBeDeleted,
  polyTopoChange& meshMod
)
{
  // Count deleted points
  label nDeleted = 0;
  FOR_ALL(pointCanBeDeleted, pointI)
  {
    if (pointCanBeDeleted[pointI])
    {
      nDeleted++;
    }
  }
  // Faces (in mesh face labels) affected by points removed. Will hopefully
  // be only a few.
  labelHashSet facesAffected(4*nDeleted);
  // Undo: from global mesh point to index in savedPoint_
  Map<label> pointToSaved;
  // Size undo storage
  if (undoable_)
  {
    savedPoints_.setSize(nDeleted);
    pointToSaved.resize(2*nDeleted);
  }
  // Remove points
  // ~~~~~~~~~~~~~
  nDeleted = 0;
  FOR_ALL(pointCanBeDeleted, pointI)
  {
    if (pointCanBeDeleted[pointI])
    {
      if (undoable_)
      {
        pointToSaved.insert(pointI, nDeleted);
        savedPoints_[nDeleted++] = mesh_.points()[pointI];
      }
      meshMod.setAction(polyRemovePoint(pointI));
      // Store faces affected
      const labelList& pFaces = mesh_.pointFaces()[pointI];
      FOR_ALL(pFaces, i)
      {
        facesAffected.insert(pFaces[i]);
      }
    }
  }
  // Update faces
  // ~~~~~~~~~~~~
  if (undoable_)
  {
    savedFaceLabels_.setSize(facesAffected.size());
    savedFaces_.setSize(facesAffected.size());
  }
  label nSaved = 0;
  FOR_ALL_CONST_ITER(labelHashSet, facesAffected, iter)
  {
    label faceI = iter.key();
    const face& f = mesh_.faces()[faceI];
    face newFace(f.size());
    label newI = 0;
    FOR_ALL(f, fp)
    {
      label pointI = f[fp];
      if (!pointCanBeDeleted[pointI])
      {
        newFace[newI++] = pointI;
      }
    }
    newFace.setSize(newI);
    // Actually change the face to the new vertices
    modifyFace(faceI, newFace, meshMod);
    // Save the face. Negative indices are into savedPoints_
    if (undoable_)
    {
      savedFaceLabels_[nSaved] = faceI;
      face& savedFace = savedFaces_[nSaved++];
      savedFace.setSize(f.size());
      FOR_ALL(f, fp)
      {
        label pointI = f[fp];
        if (pointCanBeDeleted[pointI])
        {
          savedFace[fp] = -pointToSaved[pointI]-1;
        }
        else
        {
          savedFace[fp] = pointI;
        }
      }
    }
  }
  if (undoable_)
  {
    // DEBUG: Compare the stored faces with the current ones.
    if (debug)
    {
      FOR_ALL(savedFaceLabels_, saveI)
      {
        // Points from the mesh
        List<point> meshPoints
        (
          UIndirectList<point>
          (
            mesh_.points(),
            mesh_.faces()[savedFaceLabels_[saveI]]  // mesh face
          )
        );
        // Points from the saved face
        List<point> keptPoints
        (
          BiIndirectList<point>
          (
            mesh_.points(),
            savedPoints_,
            savedFaces_[saveI]  // saved face
          )
        );
        if (meshPoints != keptPoints)
        {
          FATAL_ERROR_IN("setRefinement")
            << "faceI:" << savedFaceLabels_[saveI] << nl
            << "meshPoints:" << meshPoints << nl
            << "keptPoints:" << keptPoints << nl
            << abort(FatalError);
        }
      }
    }
  }
}
void mousse::removePoints::updateMesh(const mapPolyMesh& map)
{
  if (undoable_)
  {
    FOR_ALL(savedFaceLabels_, localI)
    {
      if (savedFaceLabels_[localI] >= 0)
      {
        label newFaceI = map.reverseFaceMap()[savedFaceLabels_[localI]];
        if (newFaceI == -1)
        {
          FATAL_ERROR_IN
          (
            "removePoints::updateMesh(const mapPolyMesh&)"
          )   << "Old face " << savedFaceLabels_[localI]
            << " seems to have dissapeared."
            << abort(FatalError);
        }
        savedFaceLabels_[localI] = newFaceI;
      }
    }
    // Renumber mesh vertices (indices >=0). Leave saved vertices
    // (<0) intact.
    FOR_ALL(savedFaces_, i)
    {
      face& f = savedFaces_[i];
      FOR_ALL(f, fp)
      {
        label pointI = f[fp];
        if (pointI >= 0)
        {
          f[fp] = map.reversePointMap()[pointI];
          if (f[fp] == -1)
          {
            FATAL_ERROR_IN
            (
              "removePoints::updateMesh(const mapPolyMesh&)"
            )   << "Old point " << pointI
              << " seems to have dissapeared."
              << abort(FatalError);
          }
        }
      }
    }
    // DEBUG: Compare the stored faces with the current ones.
    if (debug)
    {
      FOR_ALL(savedFaceLabels_, saveI)
      {
        if (savedFaceLabels_[saveI] >= 0)
        {
          const face& f = mesh_.faces()[savedFaceLabels_[saveI]];
          // Get kept points of saved faces.
          const face& savedFace = savedFaces_[saveI];
          face keptFace(savedFace.size());
          label keptFp = 0;
          FOR_ALL(savedFace, fp)
          {
            label pointI = savedFace[fp];
            if (pointI >= 0)
            {
              keptFace[keptFp++] = pointI;
            }
          }
          keptFace.setSize(keptFp);
          // Compare as faces (since f might have rotated and
          // face::operator== takes care of this)
          if (keptFace != f)
          {
            FATAL_ERROR_IN("setRefinement")
              << "faceI:" << savedFaceLabels_[saveI] << nl
              << "face:" << f << nl
              << "keptFace:" << keptFace << nl
              << "saved points:"
              <<  BiIndirectList<point>
                (
                  mesh_.points(),
                  savedPoints_,
                  savedFace
                )() << nl
              << abort(FatalError);
          }
        }
      }
    }
  }
}
// Given list of faces to undo picks up the local indices of the faces
// to restore. Additionally it also picks up all the faces that use
// any of the deleted points.
void mousse::removePoints::getUnrefimentSet
(
  const labelList& undoFaces,
  labelList& localFaces,
  labelList& localPoints
) const
{
  if (!undoable_)
  {
    FATAL_ERROR_IN
    (
      "removePoints::getUnrefimentSet(const labelList&"
      ", labelList&, labelList&) const"
    )   << "removePoints not constructed with"
      << " unrefinement capability."
      << abort(FatalError);
  }
  if (debug)
  {
    // Check if synced.
    faceSet undoFacesSet(mesh_, "undoFacesSet", undoFaces);
    label sz = undoFacesSet.size();
    undoFacesSet.sync(mesh_);
    if (sz != undoFacesSet.size())
    {
      FATAL_ERROR_IN
      (
        "removePoints::getUnrefimentSet(const labelList&"
        ", labelList&, labelList&) const"
      )   << "undoFaces not synchronised across coupled faces." << endl
        << "Size before sync:" << sz
        << "  after sync:" << undoFacesSet.size()
        << abort(FatalError);
    }
  }
  // Problem: input undoFaces are synced. But e.g.
  // two faces, A (uncoupled) and B(coupled) share a deleted point. A gets
  // passed in to be restored. Now picking up the deleted point and the
  // original faces using it picks up face B. But not its coupled neighbour!
  // Problem is that we cannot easily synchronise the deleted points
  // (e.g. syncPointList) since they're not in the mesh anymore - only the
  // faces are. So we have to collect the points-to-restore as indices
  // in the faces (which is information we can synchronise)
  // Mark points-to-restore
  labelHashSet localPointsSet(undoFaces.size());
  {
    // Create set of faces to be restored
    labelHashSet undoFacesSet(undoFaces);
    FOR_ALL(savedFaceLabels_, saveI)
    {
      if (savedFaceLabels_[saveI] < 0)
      {
        FATAL_ERROR_IN
        (
          "removePoints::getUnrefimentSet(const labelList&"
          ", labelList&, labelList&) const"
        )   << "Illegal face label " << savedFaceLabels_[saveI]
          << " at index " << saveI
          << abort(FatalError);
      }
      if (undoFacesSet.found(savedFaceLabels_[saveI]))
      {
        const face& savedFace = savedFaces_[saveI];
        FOR_ALL(savedFace, fp)
        {
          if (savedFace[fp] < 0)
          {
            label savedPointI = -savedFace[fp]-1;
            if (savedPoints_[savedPointI] == vector::max)
            {
              FATAL_ERROR_IN
              (
                "removePoints::getUnrefimentSet"
                "(const labelList&, labelList&, labelList&)"
                " const"
              )   << "Trying to restore point " << savedPointI
                << " from mesh face " << savedFaceLabels_[saveI]
                << " saved face:" << savedFace
                << " which has already been undone."
                << abort(FatalError);
            }
            localPointsSet.insert(savedPointI);
          }
        }
      }
    }
    // Per boundary face, per index in face whether the point needs
    // restoring. Note that this is over all saved faces, not just over
    // the ones in undoFaces.
    boolListList faceVertexRestore(mesh_.nFaces()-mesh_.nInternalFaces());
    // Populate with my local points-to-restore.
    FOR_ALL(savedFaces_, saveI)
    {
      label bFaceI = savedFaceLabels_[saveI] - mesh_.nInternalFaces();
      if (bFaceI >= 0)
      {
        const face& savedFace = savedFaces_[saveI];
        boolList& fRestore = faceVertexRestore[bFaceI];
        fRestore.setSize(savedFace.size());
        fRestore = false;
        FOR_ALL(savedFace, fp)
        {
          if (savedFace[fp] < 0)
          {
            label savedPointI = -savedFace[fp]-1;
            if (localPointsSet.found(savedPointI))
            {
              fRestore[fp] = true;
            }
          }
        }
      }
    }
    syncTools::syncBoundaryFaceList
    (
      mesh_,
      faceVertexRestore,
      faceEqOp<bool, orEqOp>(),   // special operator to handle faces
      mousse::dummyTransform()      // no transformation
    );
    // So now if any of the points-to-restore is used by any coupled face
    // anywhere the corresponding index in faceVertexRestore will be set.
    // Now combine the localPointSet and the (sychronised)
    // boundary-points-to-restore.
    FOR_ALL(savedFaces_, saveI)
    {
      label bFaceI = savedFaceLabels_[saveI] - mesh_.nInternalFaces();
      if (bFaceI >= 0)
      {
        const boolList& fRestore = faceVertexRestore[bFaceI];
        const face& savedFace = savedFaces_[saveI];
        FOR_ALL(fRestore, fp)
        {
          // Does neighbour require point restored?
          if (fRestore[fp])
          {
            if (savedFace[fp] >= 0)
            {
              FATAL_ERROR_IN
              (
                "removePoints::getUnrefimentSet"
                "(const labelList&, labelList&, labelList&)"
                " const"
              )   << "Problem: on coupled face:"
                << savedFaceLabels_[saveI]
                << " fc:"
                << mesh_.faceCentres()[savedFaceLabels_[saveI]]
                << endl
                << " my neighbour tries to restore the vertex"
                << " at index " << fp
                << " whereas my saved face:" << savedFace
                << " does not indicate a deleted vertex"
                << " at that position."
                << abort(FatalError);
            }
            label savedPointI = -savedFace[fp]-1;
            localPointsSet.insert(savedPointI);
          }
        }
      }
    }
  }
  localPoints = localPointsSet.toc();
  // Collect all saved faces using any localPointsSet
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  labelHashSet localFacesSet(2*undoFaces.size());
  FOR_ALL(savedFaces_, saveI)
  {
    const face& savedFace = savedFaces_[saveI];
    FOR_ALL(savedFace, fp)
    {
      if (savedFace[fp] < 0)
      {
        label savedPointI = -savedFace[fp]-1;
        if (localPointsSet.found(savedPointI))
        {
          localFacesSet.insert(saveI);
        }
      }
    }
  }
  localFaces = localFacesSet.toc();
  // Note that at this point the localFaces to restore can contain points
  // that are not going to be restored! The localFaces though will
  // be guaranteed to be all the ones affected by the restoration of the
  // localPoints.
}
void mousse::removePoints::setUnrefinement
(
  const labelList& localFaces,
  const labelList& localPoints,
  polyTopoChange& meshMod
)
{
  if (!undoable_)
  {
    FATAL_ERROR_IN
    (
      "removePoints::setUnrefinement(const labelList&"
      ", labelList&, polyTopoChange&)"
    )   << "removePoints not constructed with"
      << " unrefinement capability."
      << abort(FatalError);
  }
  // Per savedPoint -1 or the restored point label
  labelList addedPoints(savedPoints_.size(), -1);
  FOR_ALL(localPoints, i)
  {
    label localI = localPoints[i];
    if (savedPoints_[localI] == vector::max)
    {
      FATAL_ERROR_IN
      (
        "removePoints::setUnrefinement(const labelList&"
        ", labelList&, polyTopoChange&)"
      )   << "Saved point " << localI << " already restored!"
        << abort(FatalError);
    }
    addedPoints[localI] = meshMod.setAction
    (
      polyAddPoint
      (
        savedPoints_[localI],   // point
        -1,                     // master point
        -1,                     // zone for point
        true                    // supports a cell
      )
    );
    // Mark the restored points so they are not restored again.
    savedPoints_[localI] = vector::max;
  }
  FOR_ALL(localFaces, i)
  {
    label saveI = localFaces[i];
    // Modify indices into saved points (so < 0) to point to the
    // added points.
    face& savedFace = savedFaces_[saveI];
    face newFace(savedFace.size());
    label newFp = 0;
    bool hasSavedPoints = false;
    FOR_ALL(savedFace, fp)
    {
      if (savedFace[fp] < 0)
      {
        label addedPointI = addedPoints[-savedFace[fp]-1];
        if (addedPointI != -1)
        {
          savedFace[fp] = addedPointI;
          newFace[newFp++] = addedPointI;
        }
        else
        {
          hasSavedPoints = true;
        }
      }
      else
      {
        newFace[newFp++] = savedFace[fp];
      }
    }
    newFace.setSize(newFp);
    modifyFace(savedFaceLabels_[saveI], newFace, meshMod);
    if (!hasSavedPoints)
    {
      // Face fully restored. Mark for compaction later on
      savedFaceLabels_[saveI] = -1;
      savedFaces_[saveI].clear();
    }
  }
  // Compact face labels
  label newSaveI = 0;
  FOR_ALL(savedFaceLabels_, saveI)
  {
    if (savedFaceLabels_[saveI] != -1)
    {
      if (newSaveI != saveI)
      {
        savedFaceLabels_[newSaveI] = savedFaceLabels_[saveI];
        savedFaces_[newSaveI].transfer(savedFaces_[saveI]);
      }
      newSaveI++;
    }
  }
  savedFaceLabels_.setSize(newSaveI);
  savedFaces_.setSize(newSaveI);
  // Check that all faces have been restored that use any restored points
  if (debug)
  {
    FOR_ALL(savedFaceLabels_, saveI)
    {
      const face& savedFace = savedFaces_[saveI];
      FOR_ALL(savedFace, fp)
      {
        if (savedFace[fp] < 0)
        {
          label addedPointI = addedPoints[-savedFace[fp]-1];
          if (addedPointI != -1)
          {
            FATAL_ERROR_IN("setUnrefinement")
              << "Face:" << savedFaceLabels_[saveI]
              << " savedVerts:" << savedFace
              << " uses restored point:" << -savedFace[fp]-1
              << " with new pointlabel:" << addedPointI
              << abort(FatalError);
          }
        }
      }
    }
  }
}
