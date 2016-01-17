// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface.hpp"
#include "demand_driven_data.hpp"
#include "ifstream.hpp"
#include "ofstream.hpp"
#include "time.hpp"
#include "bound_box.hpp"
#include "sortable_list.hpp"
#include "packed_bool_list.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(triSurface, 0);
}
mousse::fileName mousse::triSurface::triSurfInstance(const Time& d)
{
  fileName foamName(d.caseName() + ".ftr");
  // Search back through the time directories list to find the time
  // closest to and lower than current time
  instantList ts = d.times();
  label i;
  for (i=ts.size()-1; i>=0; i--)
  {
    if (ts[i].value() <= d.timeOutputValue())
    {
      break;
    }
  }
  // Noting that the current directory has already been searched
  // for mesh data, start searching from the previously stored time directory
  if (i>=0)
  {
    for (label j=i; j>=0; j--)
    {
      if (isFile(d.path()/ts[j].name()/typeName/foamName))
      {
        if (debug)
        {
          Pout<< " triSurface::triSurfInstance(const Time& d)"
            << "reading " << foamName
            << " from " << ts[j].name()/typeName
            << endl;
        }
        return ts[j].name();
      }
    }
  }
  if (debug)
  {
    Pout<< " triSurface::triSurfInstance(const Time& d)"
      << "reading " << foamName
      << " from constant/" << endl;
  }
  return d.constant();
}
mousse::List<mousse::labelledTri> mousse::triSurface::convertToTri
(
  const faceList& faces,
  const label defaultRegion
)
{
  List<labelledTri> triFaces(faces.size());
  FOR_ALL(triFaces, faceI)
  {
    const face& f = faces[faceI];
    if (f.size() != 3)
    {
      FATAL_ERROR_IN
      (
        "triSurface::convertToTri"
        "(const faceList&, const label)"
      )   << "Face at position " << faceI
        << " does not have three vertices:" << f
        << abort(FatalError);
    }
    labelledTri& tri = triFaces[faceI];
    tri[0] = f[0];
    tri[1] = f[1];
    tri[2] = f[2];
    tri.region() = defaultRegion;
  }
  return triFaces;
}
mousse::List<mousse::labelledTri> mousse::triSurface::convertToTri
(
  const triFaceList& faces,
  const label defaultRegion
)
{
  List<labelledTri> triFaces(faces.size());
  FOR_ALL(triFaces, faceI)
  {
    const triFace& f = faces[faceI];
    labelledTri& tri = triFaces[faceI];
    tri[0] = f[0];
    tri[1] = f[1];
    tri[2] = f[2];
    tri.region() = defaultRegion;
  }
  return triFaces;
}
// Private Member Functions 
void mousse::triSurface::printTriangle
(
  Ostream& os,
  const string& pre,
  const labelledTri& f,
  const pointField& points
)
{
  os
    << pre.c_str() << "vertex numbers:"
    << f[0] << ' ' << f[1] << ' ' << f[2] << endl
    << pre.c_str() << "vertex coords :"
    << points[f[0]] << ' ' << points[f[1]] << ' ' << points[f[2]]
    << pre.c_str() << "region        :" << f.region() << endl
    << endl;
}
mousse::string mousse::triSurface::getLineNoComment(IFstream& is)
{
  string line;
  do
  {
    is.getLine(line);
  }
  while ((line.empty() || line[0] == '#') && is.good());
  return line;
}
// Remove non-triangles, double triangles.
void mousse::triSurface::checkTriangles(const bool verbose)
{
  // Simple check on indices ok.
  const label maxPointI = points().size() - 1;
  FOR_ALL(*this, faceI)
  {
    const triSurface::FaceType& f = (*this)[faceI];
    FOR_ALL(f, fp)
    {
      if (f[fp] < 0 || f[fp] > maxPointI)
      {
        FATAL_ERROR_IN("triSurface::checkTriangles(bool)")
          << "triangle " << f
          << " uses point indices outside point range 0.."
          << maxPointI
          << exit(FatalError);
      }
    }
  }
  // Two phase process
  //   1. mark invalid faces
  //   2. pack
  // Done to keep numbering constant in phase 1
  // List of valid triangles
  boolList valid(size(), true);
  bool hasInvalid = false;
  FOR_ALL(*this, faceI)
  {
    const labelledTri& f = (*this)[faceI];
    if ((f[0] == f[1]) || (f[0] == f[2]) || (f[1] == f[2]))
    {
      // 'degenerate' triangle check
      valid[faceI] = false;
      hasInvalid = true;
      if (verbose)
      {
        WARNING_IN
        (
          "triSurface::checkTriangles(bool verbose)"
        )   << "triangle " << faceI
          << " does not have three unique vertices:\n";
        printTriangle(Warning, "    ", f, points());
      }
    }
    else
    {
      // duplicate triangle check
      const labelList& fEdges = faceEdges()[faceI];
      // Check if faceNeighbours use same points as this face.
      // Note: discards normal information - sides of baffle are merged.
      FOR_ALL(fEdges, fp)
      {
        const labelList& eFaces = edgeFaces()[fEdges[fp]];
        FOR_ALL(eFaces, i)
        {
          label neighbour = eFaces[i];
          if (neighbour > faceI)
          {
            // lower numbered faces already checked
            const labelledTri& n = (*this)[neighbour];
            if
            (
              ((f[0] == n[0]) || (f[0] == n[1]) || (f[0] == n[2]))
            && ((f[1] == n[0]) || (f[1] == n[1]) || (f[1] == n[2]))
            && ((f[2] == n[0]) || (f[2] == n[1]) || (f[2] == n[2]))
            )
            {
              valid[faceI] = false;
              hasInvalid = true;
              if (verbose)
              {
                WARNING_IN
                (
                  "triSurface::checkTriangles(bool verbose)"
                )   << "triangles share the same vertices:\n"
                  << "    face 1 :" << faceI << endl;
                printTriangle(Warning, "    ", f, points());
                Warning
                  << endl
                  << "    face 2 :"
                  << neighbour << endl;
                printTriangle(Warning, "    ", n, points());
              }
              break;
            }
          }
        }
      }
    }
  }
  if (hasInvalid)
  {
    // Pack
    label newFaceI = 0;
    FOR_ALL(*this, faceI)
    {
      if (valid[faceI])
      {
        const labelledTri& f = (*this)[faceI];
        (*this)[newFaceI++] = f;
      }
    }
    if (verbose)
    {
      WARNING_IN
      (
        "triSurface::checkTriangles(bool verbose)"
      )   << "Removing " << size() - newFaceI
        << " illegal faces." << endl;
    }
    (*this).setSize(newFaceI);
    // Topology can change because of renumbering
    clearOut();
  }
}
// Check/fix edges with more than two triangles
void mousse::triSurface::checkEdges(const bool verbose)
{
  const labelListList& eFaces = edgeFaces();
  FOR_ALL(eFaces, edgeI)
  {
    const labelList& myFaces = eFaces[edgeI];
    if (myFaces.empty())
    {
      FATAL_ERROR_IN("triSurface::checkEdges(bool verbose)")
        << "Edge " << edgeI << " with vertices " << edges()[edgeI]
        << " has no edgeFaces"
        << exit(FatalError);
    }
    else if (myFaces.size() > 2 && verbose)
    {
      WARNING_IN
      (
        "triSurface::checkEdges(bool verbose)"
      )   << "Edge " << edgeI << " with vertices " << edges()[edgeI]
        << " has more than 2 faces connected to it : " << myFaces
        << endl;
    }
  }
}
// Read triangles, points from Istream
bool mousse::triSurface::read(Istream& is)
{
  is  >> patches_ >> storedPoints() >> storedFaces();
  return true;
}
// Read from file in given format
bool mousse::triSurface::read
(
  const fileName& name,
  const word& ext,
  const bool check
)
{
  if (check && !exists(name))
  {
    FATAL_ERROR_IN
    (
      "triSurface::read(const fileName&, const word&, const bool)"
    )   << "Cannnot read " << name << exit(FatalError);
  }
  if (ext == "gz")
  {
    fileName unzipName = name.lessExt();
    // Do not check for existence. Let IFstream do the unzipping.
    return read(unzipName, unzipName.ext(), false);
  }
  else if (ext == "ftr")
  {
    return read(IFstream(name)());
  }
  else if (ext == "stl")
  {
    return readSTL(name);
  }
  else if (ext == "stlb")
  {
    return readSTLBINARY(name);
  }
  else if (ext == "gts")
  {
    return readGTS(name);
  }
  else if (ext == "obj")
  {
    return readOBJ(name);
  }
  else if (ext == "off")
  {
    return readOFF(name);
  }
  else if (ext == "tri")
  {
    return readTRI(name);
  }
  else if (ext == "ac")
  {
    return readAC(name);
  }
  else if (ext == "nas")
  {
    return readNAS(name);
  }
  else if (ext == "vtk")
  {
    return readVTK(name);
  }
  else
  {
    FATAL_ERROR_IN
    (
      "triSurface::read(const fileName&, const word&)"
    )   << "unknown file extension " << ext
      << ". Supported extensions are '.ftr', '.stl', '.stlb', '.gts'"
      << ", '.obj', '.ac', '.off', '.nas', '.tri' and '.vtk'"
      << exit(FatalError);
    return false;
  }
}
// Write to file in given format
void mousse::triSurface::write
(
  const fileName& name,
  const word& ext,
  const bool sort
) const
{
  if (ext == "ftr")
  {
    return write(OFstream(name)());
  }
  else if (ext == "stl")
  {
    return writeSTLASCII(sort, OFstream(name)());
  }
  else if (ext == "stlb")
  {
    ofstream outFile(name.c_str(), std::ios::binary);
    writeSTLBINARY(outFile);
  }
  else if (ext == "gts")
  {
    return writeGTS(sort, OFstream(name)());
  }
  else if (ext == "obj")
  {
    writeOBJ(sort, OFstream(name)());
  }
  else if (ext == "off")
  {
    writeOFF(sort, OFstream(name)());
  }
  else if (ext == "vtk")
  {
    writeVTK(sort, OFstream(name)());
  }
  else if (ext == "tri")
  {
    writeTRI(sort, OFstream(name)());
  }
  else if (ext == "dx")
  {
    writeDX(sort, OFstream(name)());
  }
  else if (ext == "ac")
  {
    writeAC(OFstream(name)());
  }
  else if (ext == "smesh")
  {
    writeSMESH(sort, OFstream(name)());
  }
  else
  {
    FATAL_ERROR_IN
    (
      "triSurface::write(const fileName&, const word&, const bool)"
    )   << "unknown file extension " << ext
      << " for file " << name
      << ". Supported extensions are '.ftr', '.stl', '.stlb', "
      << "'.gts', '.obj', '.vtk'"
      << ", '.off', '.dx', '.smesh', '.ac' and '.tri'"
      << exit(FatalError);
  }
}
// Returns patch info. Sets faceMap to the indexing according to patch
// numbers. Patch numbers start at 0.
mousse::surfacePatchList mousse::triSurface::calcPatches(labelList& faceMap) const
{
  // Sort according to region numbers of labelledTri
  SortableList<label> sortedRegion(size());
  FOR_ALL(sortedRegion, faceI)
  {
    sortedRegion[faceI] = operator[](faceI).region();
  }
  sortedRegion.sort();
  faceMap = sortedRegion.indices();
  // Extend regions
  label maxRegion = patches_.size()-1;    // for non-compacted regions
  if (faceMap.size())
  {
    maxRegion = max
    (
      maxRegion,
      operator[](faceMap.last()).region()
    );
  }
  // Get new region list
  surfacePatchList newPatches(maxRegion + 1);
  // Fill patch sizes
  FOR_ALL(*this, faceI)
  {
    label region = operator[](faceI).region();
    newPatches[region].size()++;
  }
  // Fill rest of patch info
  label startFaceI = 0;
  FOR_ALL(newPatches, newPatchI)
  {
    surfacePatch& newPatch = newPatches[newPatchI];
    newPatch.index() = newPatchI;
    label oldPatchI = newPatchI;
    // start of patch
    newPatch.start() = startFaceI;
    // Take over any information from existing patches
    if ((oldPatchI < patches_.size()) && (patches_[oldPatchI].name() != ""))
    {
      newPatch.name() = patches_[oldPatchI].name();
    }
    else
    {
      newPatch.name() = word("patch") + name(newPatchI);
    }
    if
    (
      (oldPatchI < patches_.size())
    && (patches_[oldPatchI].geometricType() != "")
    )
    {
      newPatch.geometricType() = patches_[oldPatchI].geometricType();
    }
    else
    {
      newPatch.geometricType() = "empty";
    }
    startFaceI += newPatch.size();
  }
  return newPatches;
}
void mousse::triSurface::setDefaultPatches()
{
  labelList faceMap;
  // Get names, types and sizes
  surfacePatchList newPatches(calcPatches(faceMap));
  // Take over names and types (but not size)
  patches_.setSize(newPatches.size());
  FOR_ALL(newPatches, patchI)
  {
    patches_[patchI].index() = patchI;
    patches_[patchI].name() = newPatches[patchI].name();
    patches_[patchI].geometricType() = newPatches[patchI].geometricType();
  }
}
// Constructors 
mousse::triSurface::triSurface()
:
  ParentType(List<Face>(), pointField()),
  patches_(0),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{}
mousse::triSurface::triSurface
(
  const List<labelledTri>& triangles,
  const geometricSurfacePatchList& patches,
  const pointField& points
)
:
  ParentType(triangles, points),
  patches_(patches),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{}
mousse::triSurface::triSurface
(
  List<labelledTri>& triangles,
  const geometricSurfacePatchList& patches,
  pointField& points,
  const bool reUse
)
:
  ParentType(triangles, points, reUse),
  patches_(patches),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{}
mousse::triSurface::triSurface
(
  const Xfer<List<labelledTri> >& triangles,
  const geometricSurfacePatchList& patches,
  const Xfer<List<point> >& points
)
:
  ParentType(triangles, points),
  patches_(patches),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{}
mousse::triSurface::triSurface
(
  const List<labelledTri>& triangles,
  const pointField& points
)
:
  ParentType(triangles, points),
  patches_(),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{
  setDefaultPatches();
}
mousse::triSurface::triSurface
(
  const triFaceList& triangles,
  const pointField& points
)
:
  ParentType(convertToTri(triangles, 0), points),
  patches_(),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{
  setDefaultPatches();
}
mousse::triSurface::triSurface(const fileName& name)
:
  ParentType(List<Face>(), pointField()),
  patches_(),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{
  word ext = name.ext();
  read(name, ext);
  setDefaultPatches();
}
mousse::triSurface::triSurface(Istream& is)
:
  ParentType(List<Face>(), pointField()),
  patches_(),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{
  read(is);
  setDefaultPatches();
}
mousse::triSurface::triSurface(const Time& d)
:
  ParentType(List<Face>(), pointField()),
  patches_(),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{
  fileName foamFile(d.caseName() + ".ftr");
  fileName foamPath(d.path()/triSurfInstance(d)/typeName/foamFile);
  IFstream foamStream(foamPath);
  read(foamStream);
  setDefaultPatches();
}
mousse::triSurface::triSurface(const triSurface& ts)
:
  ParentType(ts, ts.points()),
  patches_(ts.patches()),
  sortedEdgeFacesPtr_(NULL),
  edgeOwnerPtr_(NULL)
{}
// Destructor 
mousse::triSurface::~triSurface()
{
  clearOut();
}
// Member Functions 
void mousse::triSurface::clearTopology()
{
  ParentType::clearTopology();
  deleteDemandDrivenData(sortedEdgeFacesPtr_);
  deleteDemandDrivenData(edgeOwnerPtr_);
}
void mousse::triSurface::clearPatchMeshAddr()
{
  ParentType::clearPatchMeshAddr();
}
void mousse::triSurface::clearOut()
{
  ParentType::clearOut();
  clearTopology();
  clearPatchMeshAddr();
}
const mousse::labelListList& mousse::triSurface::sortedEdgeFaces() const
{
  if (!sortedEdgeFacesPtr_)
  {
    calcSortedEdgeFaces();
  }
  return *sortedEdgeFacesPtr_;
}
const mousse::labelList& mousse::triSurface::edgeOwner() const
{
  if (!edgeOwnerPtr_)
  {
    calcEdgeOwner();
  }
  return *edgeOwnerPtr_;
}
void mousse::triSurface::movePoints(const pointField& newPoints)
{
  // Remove all geometry dependent data
  deleteDemandDrivenData(sortedEdgeFacesPtr_);
  // Adapt for new point position
  ParentType::movePoints(newPoints);
  // Copy new points
  storedPoints() = newPoints;
}
void mousse::triSurface::scalePoints(const scalar scaleFactor)
{
  // avoid bad scaling
  if (scaleFactor > 0 && scaleFactor != 1.0)
  {
    // Remove all geometry dependent data
    clearTopology();
    // Adapt for new point position
    ParentType::movePoints(pointField());
    storedPoints() *= scaleFactor;
  }
}
// Remove non-triangles, double triangles.
void mousse::triSurface::cleanup(const bool verbose)
{
  // Merge points (already done for STL, TRI)
  stitchTriangles(SMALL, verbose);
  // Merging points might have changed geometric factors
  clearOut();
  checkTriangles(verbose);
  checkEdges(verbose);
}
// Finds area, starting at faceI, delimited by borderEdge. Marks all visited
// faces (from face-edge-face walk) with currentZone.
void mousse::triSurface::markZone
(
  const boolList& borderEdge,
  const label faceI,
  const label currentZone,
  labelList& faceZone
) const
{
  // List of faces whose faceZone has been set.
  labelList changedFaces(1, faceI);
  while (true)
  {
    // Pick up neighbours of changedFaces
    DynamicList<label> newChangedFaces(2*changedFaces.size());
    FOR_ALL(changedFaces, i)
    {
      label faceI = changedFaces[i];
      const labelList& fEdges = faceEdges()[faceI];
      FOR_ALL(fEdges, i)
      {
        label edgeI = fEdges[i];
        if (!borderEdge[edgeI])
        {
          const labelList& eFaces = edgeFaces()[edgeI];
          FOR_ALL(eFaces, j)
          {
            label nbrFaceI = eFaces[j];
            if (faceZone[nbrFaceI] == -1)
            {
              faceZone[nbrFaceI] = currentZone;
              newChangedFaces.append(nbrFaceI);
            }
            else if (faceZone[nbrFaceI] != currentZone)
            {
              FATAL_ERROR_IN
              (
                "triSurface::markZone(const boolList&,"
                "const label, const label, labelList&) const"
              )
                << "Zones " << faceZone[nbrFaceI]
                << " at face " << nbrFaceI
                << " connects to zone " << currentZone
                << " at face " << faceI
                << abort(FatalError);
            }
          }
        }
      }
    }
    if (newChangedFaces.empty())
    {
      break;
    }
    changedFaces.transfer(newChangedFaces);
  }
}
// Finds areas delimited by borderEdge (or 'real' edges).
// Fills faceZone accordingly
mousse::label mousse::triSurface::markZones
(
  const boolList& borderEdge,
  labelList& faceZone
) const
{
  faceZone.setSize(size());
  faceZone = -1;
  if (borderEdge.size() != nEdges())
  {
    FATAL_ERROR_IN
    (
      "triSurface::markZones"
      "(const boolList&, labelList&)"
    )
      << "borderEdge boolList not same size as number of edges" << endl
      << "borderEdge:" << borderEdge.size() << endl
      << "nEdges    :" << nEdges()
      << exit(FatalError);
  }
  label zoneI = 0;
  for (label startFaceI = 0;; zoneI++)
  {
    // Find first non-coloured face
    for (; startFaceI < size(); startFaceI++)
    {
      if (faceZone[startFaceI] == -1)
      {
        break;
      }
    }
    if (startFaceI >= size())
    {
      break;
    }
    faceZone[startFaceI] = zoneI;
    markZone(borderEdge, startFaceI, zoneI, faceZone);
  }
  return zoneI;
}
void mousse::triSurface::subsetMeshMap
(
  const boolList& include,
  labelList& pointMap,
  labelList& faceMap
) const
{
  const List<labelledTri>& locFaces = localFaces();
  label faceI = 0;
  label pointI = 0;
  faceMap.setSize(locFaces.size());
  pointMap.setSize(nPoints());
  boolList pointHad(nPoints(), false);
  FOR_ALL(include, oldFaceI)
  {
    if (include[oldFaceI])
    {
      // Store new faces compact
      faceMap[faceI++] = oldFaceI;
      // Renumber labels for face
      const triSurface::FaceType& f = locFaces[oldFaceI];
      FOR_ALL(f, fp)
      {
        label labI = f[fp];
        if (!pointHad[labI])
        {
          pointHad[labI] = true;
          pointMap[pointI++] = labI;
        }
      }
    }
  }
  // Trim
  faceMap.setSize(faceI);
  pointMap.setSize(pointI);
}
mousse::triSurface mousse::triSurface::subsetMesh
(
  const boolList& include,
  labelList& pointMap,
  labelList& faceMap
) const
{
  const pointField& locPoints = localPoints();
  const List<labelledTri>& locFaces = localFaces();
  // Fill pointMap, faceMap
  subsetMeshMap(include, pointMap, faceMap);
  // Create compact coordinate list and forward mapping array
  pointField newPoints(pointMap.size());
  labelList oldToNew(locPoints.size());
  FOR_ALL(pointMap, pointi)
  {
    newPoints[pointi] = locPoints[pointMap[pointi]];
    oldToNew[pointMap[pointi]] = pointi;
  }
  // Renumber triangle node labels and compact
  List<labelledTri> newTriangles(faceMap.size());
  FOR_ALL(faceMap, facei)
  {
    // Get old vertex labels
    const labelledTri& tri = locFaces[faceMap[facei]];
    newTriangles[facei][0] = oldToNew[tri[0]];
    newTriangles[facei][1] = oldToNew[tri[1]];
    newTriangles[facei][2] = oldToNew[tri[2]];
    newTriangles[facei].region() = tri.region();
  }
  // Construct subsurface
  return triSurface(newTriangles, patches(), newPoints, true);
}
void mousse::triSurface::write
(
  const fileName& name,
  const bool sortByRegion
) const
{
  write(name, name.ext(), sortByRegion);
}
void mousse::triSurface::write(Ostream& os) const
{
  os  << patches() << endl;
  //Note: Write with global point numbering
  os  << points() << nl
    << static_cast<const List<labelledTri>&>(*this) << endl;
  // Check state of Ostream
  os.check("triSurface::write(Ostream&)");
}
void mousse::triSurface::write(const Time& d) const
{
  fileName foamFile(d.caseName() + ".ftr");
  fileName foamPath(d.path()/triSurfInstance(d)/typeName/foamFile);
  OFstream foamStream(foamPath);
  write(foamStream);
}
void mousse::triSurface::writeStats(Ostream& os) const
{
  // Unfortunately nPoints constructs meshPoints() so do compact version
  // ourselves.
  PackedBoolList pointIsUsed(points().size());
  label nPoints = 0;
  boundBox bb = boundBox::invertedBox;
  FOR_ALL(*this, faceI)
  {
    const triSurface::FaceType& f = operator[](faceI);
    FOR_ALL(f, fp)
    {
      label pointI = f[fp];
      if (pointIsUsed.set(pointI, 1))
      {
        bb.min() = ::mousse::min(bb.min(), points()[pointI]);
        bb.max() = ::mousse::max(bb.max(), points()[pointI]);
        nPoints++;
      }
    }
  }
  os<< "Triangles    : " << size() << endl
    << "Vertices     : " << nPoints << endl
    << "Bounding Box : " << bb << endl;
}
// Member Operators 
void mousse::triSurface::operator=(const triSurface& ts)
{
  List<labelledTri>::operator=(ts);
  clearOut();
  storedPoints() = ts.points();
  patches_ = ts.patches();
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const triSurface& sm)
{
  sm.write(os);
  return os;
}
