// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface.hpp"
#include "unsorted_meshed_surface.hpp"
#include "meshed_surface_proxy.hpp"
#include "merge_points.hpp"
#include "time.hpp"
#include "list_ops.hpp"
#include "poly_boundary_mesh.hpp"
#include "poly_mesh.hpp"
#include "surf_mesh.hpp"
#include "primitive_patch.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
template<class Face>
inline bool mousse::MeshedSurface<Face>::isTri()
{
  return false;
}


template<class Face>
mousse::wordHashSet mousse::MeshedSurface<Face>::readTypes()
{
  return wordHashSet(*fileExtensionConstructorTablePtr_);
}


template<class Face>
mousse::wordHashSet mousse::MeshedSurface<Face>::writeTypes()
{
  return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}


// Static Member Functions
template<class Face>
bool mousse::MeshedSurface<Face>::canReadType
(
  const word& ext,
  const bool verbose
)
{
  return fileFormats::surfaceFormatsCore::checkSupport
  (
    readTypes() | FriendType::readTypes(),
    ext,
    verbose,
    "reading"
 );
}


template<class Face>
bool mousse::MeshedSurface<Face>::canWriteType
(
  const word& ext,
  const bool verbose
)
{
  return fileFormats::surfaceFormatsCore::checkSupport
  (
    writeTypes() | ProxyType::writeTypes(),
    ext,
    verbose,
    "writing"
  );
}


template<class Face>
bool mousse::MeshedSurface<Face>::canRead
(
  const fileName& name,
  const bool verbose
)
{
  word ext = name.ext();
  if (ext == "gz") {
    ext = name.lessExt().ext();
  }
  return canReadType(ext, verbose);
}


template<class Face>
void mousse::MeshedSurface<Face>::write
(
  const fileName& name,
  const MeshedSurface<Face>& surf
)
{
  if (debug) {
    Info << "MeshedSurface::write"
      "(const fileName&, const MeshedSurface&) : "
      "writing to " << name << endl;
  }
  const word ext = name.ext();
  typename writefileExtensionMemberFunctionTable::iterator mfIter =
    writefileExtensionMemberFunctionTablePtr_->find(ext);
  if (mfIter == writefileExtensionMemberFunctionTablePtr_->end()) {
    // no direct writer, delegate to proxy if possible
    wordHashSet supported = ProxyType::writeTypes();
    if (supported.found(ext)) {
      MeshedSurfaceProxy<Face>(surf).write(name);
    } else {
      FATAL_ERROR_IN
      (
        "MeshedSurface::write"
        "(const fileName&, const MeshedSurface&)"
      )
      << "Unknown file extension " << ext << nl << nl
      << "Valid types are :" << endl
      << (supported | writeTypes())
      << exit(FatalError);
    }
  } else {
    mfIter()(name, surf);
  }
}


// Constructors 
template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface()
:
  ParentType{List<Face>(), pointField()}
{}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const Xfer<pointField>& pointLst,
  const Xfer<List<Face>>& faceLst,
  const Xfer<surfZoneList>& zoneLst
)
:
  ParentType{List<Face>(), pointField()},
  zones_{}
{
  reset(pointLst, faceLst, zoneLst);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const Xfer<pointField>& pointLst,
  const Xfer<List<Face> >& faceLst,
  const labelUList& zoneSizes,
  const UList<word>& zoneNames
)
:
  ParentType{List<Face>(), pointField()}
{
  reset(pointLst, faceLst, Xfer<surfZoneList>());
  if (zoneSizes.size()) {
    if (zoneNames.size()) {
      addZones(zoneSizes, zoneNames);
    } else {
      addZones(zoneSizes);
    }
  }
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const MeshedSurface<Face>& surf
)
:
  ParentType{surf.faces(), surf.points()},
  zones_{surf.surfZones()}
{}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const UnsortedMeshedSurface<Face>& surf
)
:
  ParentType{List<Face>(), surf.points()}
{
  labelList faceMap;
  this->storedZones() = surf.sortedZones(faceMap);
  const List<Face>& origFaces = surf.faces();
  List<Face> newFaces{origFaces.size()};
  FOR_ALL(newFaces, faceI) {
    newFaces[faceMap[faceI]] = origFaces[faceI];
  }
  this->storedFaces().transfer(newFaces);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface(const surfMesh& mesh)
:
  ParentType{List<Face>(), pointField()}
{
  // same face type as surfMesh
  MeshedSurface<face> surf
  (
    xferCopy(mesh.points()),
    xferCopy(mesh.faces()),
    xferCopy(mesh.surfZones())
  );
  this->transcribe(surf);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const polyBoundaryMesh& bMesh,
  const bool useGlobalPoints
)
:
  ParentType{List<Face>(), pointField()}
{
  const polyMesh& mesh = bMesh.mesh();
  const polyPatchList& bPatches = bMesh;
  // Get a single patch for all boundaries
  primitivePatch allBoundary
  {
    SubList<face>
    {
      mesh.faces(),
      mesh.nFaces() - mesh.nInternalFaces(),
      mesh.nInternalFaces()
    },
    mesh.points()
  };
  // use global/local points:
  const pointField& bPoints =
  (
    useGlobalPoints ? mesh.points() : allBoundary.localPoints()
  );
  // global/local face addressing:
  const List<Face>& bFaces =
  (
    useGlobalPoints ? allBoundary : allBoundary.localFaces()
  );
  // create zone list
  surfZoneList newZones{bPatches.size()};
  label startFaceI = 0;
  label nZone = 0;
  FOR_ALL(bPatches, patchI) {
    const polyPatch& p = bPatches[patchI];
    if (p.size()) {
      newZones[nZone] = surfZone{p.name(), p.size(), startFaceI, nZone};
      nZone++;
      startFaceI += p.size();
    }
  }
  newZones.setSize(nZone);
  // same face type as the polyBoundaryMesh
  MeshedSurface<face> surf
  {
    xferCopy(bPoints),
    xferCopy(bFaces),
    xferMove(newZones)
  };
  this->transcribe(surf);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const fileName& name,
  const word& ext
)
:
  ParentType{List<Face>(), pointField()}
{
  read(name, ext);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface(const fileName& name)
:
  ParentType{List<Face>(), pointField()}
{
  read(name);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const Time& t,
  const word& surfName
)
:
  ParentType{List<Face>(), pointField()}
{
  surfMesh mesh
  {
    {
      "dummyName",
      t.timeName(),
      t,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    },
    surfName
  };
  // same face type as surfMesh
  MeshedSurface<face> surf
  {
    xferMove(mesh.storedPoints()),
    xferMove(mesh.storedFaces()),
    xferMove(mesh.storedZones())
  };
  this->transcribe(surf);
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const Xfer<UnsortedMeshedSurface<Face> >& surf
)
:
  ParentType{List<Face>(), pointField()}
{
  transfer(surf());
}


template<class Face>
mousse::MeshedSurface<Face>::MeshedSurface
(
  const Xfer<MeshedSurface<Face> >& surf
)
:
  ParentType{List<Face>(), pointField()}
{
  transfer(surf());
}


// Destructor 
template<class Face>
mousse::MeshedSurface<Face>::~MeshedSurface()
{}


// Protected Member Functions 
template<class Face>
void mousse::MeshedSurface<Face>::remapFaces
(
  const labelUList& faceMap
)
{
  // recalculate the zone start/size
  if (notNull(faceMap) && faceMap.size()) {
    surfZoneList& zones = storedZones();
    if (zones.size() == 1) {
      // optimized for single zone case
      zones[0].size() = faceMap.size();
    } else if (zones.size()) {
      label newFaceI = 0;
      label origEndI = 0;
      FOR_ALL(zones, zoneI) {
        surfZone& zone = zones[zoneI];
        // adjust zone start
        zone.start() = newFaceI;
        origEndI += zone.size();
        for (label faceI = newFaceI; faceI < faceMap.size(); ++faceI) {
          if (faceMap[faceI] < origEndI) {
            ++newFaceI;
          } else {
            break;
          }
        }
        // adjust zone size
        zone.size() = newFaceI - zone.start();
      }
    }
  }
}


// Member Functions 
template<class Face>
void mousse::MeshedSurface<Face>::clear()
{
  ParentType::clearOut();
  storedPoints().clear();
  storedFaces().clear();
  storedZones().clear();
}


template<class Face>
void mousse::MeshedSurface<Face>::movePoints(const pointField& newPoints)
{
  // Adapt for new point position
  ParentType::movePoints(newPoints);
  // Copy new points
  storedPoints() = newPoints;
}


template<class Face>
void mousse::MeshedSurface<Face>::scalePoints(const scalar scaleFactor)
{
  // avoid bad scaling
  if (scaleFactor > 0 && scaleFactor != 1.0) {
    pointField newPoints(scaleFactor*this->points());
    // Adapt for new point position
    ParentType::movePoints(newPoints);
    storedPoints() = newPoints;
  }
}


template<class Face>
void mousse::MeshedSurface<Face>::reset
(
  const Xfer<pointField>& pointLst,
  const Xfer<List<Face> >& faceLst,
  const Xfer<surfZoneList>& zoneLst
)
{
  ParentType::clearOut();
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(pointLst)) {
    storedPoints().transfer(pointLst());
  }
  if (notNull(faceLst)) {
    storedFaces().transfer(faceLst());
  }
  if (notNull(zoneLst)) {
    storedZones().transfer(zoneLst());
  }
}


template<class Face>
void mousse::MeshedSurface<Face>::reset
(
  const Xfer<List<point> >& pointLst,
  const Xfer<List<Face> >& faceLst,
  const Xfer<surfZoneList>& zoneLst
)
{
  ParentType::clearOut();
  // Take over new primitive data.
  // Optimized to avoid overwriting data at all
  if (notNull(pointLst)) {
    storedPoints().transfer(pointLst());
  }
  if (notNull(faceLst)) {
    storedFaces().transfer(faceLst());
  }
  if (notNull(zoneLst)) {
    storedZones().transfer(zoneLst());
  }
}


// Remove badly degenerate faces, double faces.
template<class Face>
void mousse::MeshedSurface<Face>::cleanup(const bool verbose)
{
  // merge points (already done for STL, TRI)
  stitchFaces(SMALL, verbose);
  checkFaces(verbose);
  this->checkTopology(verbose);
}


template<class Face>
bool mousse::MeshedSurface<Face>::stitchFaces
(
  const scalar tol,
  const bool verbose
)
{
  pointField& pointLst = this->storedPoints();
  // Merge points
  labelList  pointMap(pointLst.size());
  pointField newPoints(pointLst.size());
  bool hasMerged = mergePoints(pointLst, tol, verbose, pointMap, newPoints);
  if (!hasMerged) {
    return false;
  }
  if (verbose) {
    Info << "MeshedSurface::stitchFaces : Renumbering all faces"
      << endl;
  }
  // Set the coordinates to the merged ones
  pointLst.transfer(newPoints);
  List<Face>& faceLst = this->storedFaces();
  List<label> faceMap{faceLst.size()};
  // Reset the point labels to the unique points array
  label newFaceI = 0;
  FOR_ALL(faceLst, faceI) {
    Face& f = faceLst[faceI];
    FOR_ALL(f, fp) {
      f[fp] = pointMap[f[fp]];
    }
    // for extra safety: collapse face as well
    if (f.collapse() >= 3) {
      if (newFaceI != faceI) {
        faceLst[newFaceI] = f;
      }
      faceMap[newFaceI] = faceI;
      newFaceI++;
    } else if (verbose) {
      Pout << "MeshedSurface::stitchFaces : "
        << "Removing collapsed face " << faceI << endl
        << "    vertices   :" << f << endl;
    }
  }
  pointMap.clear();
  if (newFaceI != faceLst.size()) {
    if (verbose) {
      Pout << "MeshedSurface::stitchFaces : "
        << "Removed " << faceLst.size() - newFaceI
        << " faces" << endl;
    }
    faceLst.setSize(newFaceI);
    faceMap.setSize(newFaceI);
    remapFaces(faceMap);
  }
  faceMap.clear();
  // Merging points might have changed geometric factors
  ParentType::clearOut();
  return true;
}


// Remove badly degenerate faces and double faces.
template<class Face>
bool mousse::MeshedSurface<Face>::checkFaces
(
  const bool verbose
)
{
  bool changed = false;
  List<Face>& faceLst = this->storedFaces();
  List<label> faceMap{faceLst.size()};
  label newFaceI = 0;
  // Detect badly labelled faces and mark degenerate faces
  const label maxPointI = this->points().size() - 1;
  FOR_ALL(faceLst, faceI) {
    Face& f = faceLst[faceI];
    // avoid degenerate faces
    if (f.collapse() >= 3) {
      FOR_ALL(f, fp) {
        if (f[fp] < 0 || f[fp] > maxPointI) {
          FATAL_ERROR_IN("MeshedSurface::checkFaces(bool)")
            << "face " << f
            << " uses point indices outside point range 0.."
            << maxPointI
            << exit(FatalError);
        }
      }
      faceMap[faceI] = faceI;
      newFaceI++;
    } else {
      // mark as bad face
      faceMap[faceI] = -1;
      changed = true;
      if (verbose) {
        WARNING_IN
        (
          "MeshedSurface::checkFaces(bool verbose)"
        )
        << "face[" << faceI << "] = " << f
        << " does not have three unique vertices" << endl;
      }
    }
  }
  // Detect doubled faces
  // do not touch the faces
  const labelListList& fFaces = this->faceFaces();
  newFaceI = 0;
  FOR_ALL(faceLst, faceI) {
    // skip already collapsed faces:
    if (faceMap[faceI] < 0) {
      continue;
    }
    const Face& f = faceLst[faceI];
    // duplicate face check
    bool okay = true;
    const labelList& neighbours = fFaces[faceI];
    // Check if faceNeighbours use same points as this face.
    // Note: discards normal information - sides of baffle are merged.
    FOR_ALL(neighbours, neighI) {
      const label neiFaceI = neighbours[neighI];
      if (neiFaceI <= faceI || faceMap[neiFaceI] < 0) {
        // lower numbered faces already checked
        // skip neighbours that are themselves collapsed
        continue;
      }
      const Face& nei = faceLst[neiFaceI];
      if (f == nei) {
        okay = false;
        if (verbose) {
          WARNING_IN
          (
            "MeshedSurface::checkFaces(bool verbose)"
          )
          << "faces share the same vertices:" << nl
          << "    face[" << faceI << "] : " << f << nl
          << "    face[" << neiFaceI << "] : " << nei << endl;
          // printFace(Warning, "    ", f, points());
          // printFace(Warning, "    ", nei, points());
        }
        break;
      }
    }
    if (okay) {
      faceMap[faceI] = faceI;
      newFaceI++;
    } else {
      faceMap[faceI] = -1;
    }
  }
  // Phase 1: pack
  // Done to keep numbering constant in phase 1
  if (changed || newFaceI < faceLst.size()) {
    changed = true;
    if (verbose) {
      WARNING_IN
      (
        "MeshedSurface::checkFaces(bool verbose)"
      )
      << "Removed " << faceLst.size() - newFaceI
      << " illegal faces." << endl;
    }
    // compress the face list
    newFaceI = 0;
    FOR_ALL(faceLst, faceI) {
      if (faceMap[faceI] >= 0) {
        if (newFaceI != faceI) {
          faceLst[newFaceI] = faceLst[faceI];
        }
        faceMap[newFaceI] = faceI;
        newFaceI++;
      }
    }
    faceLst.setSize(newFaceI);
    remapFaces(faceMap);
  }
  faceMap.clear();
  // Topology can change because of renumbering
  ParentType::clearOut();
  return changed;
}


template<class Face>
mousse::label mousse::MeshedSurface<Face>::triangulate()
{
  return triangulate
  (
    const_cast<List<label>&>(List<label>::null())
  );
}


template<class Face>
mousse::label mousse::MeshedSurface<Face>::triangulate
(
  List<label>& faceMapOut
)
{
  label nTri = 0;
  label maxTri = 0;  // the maximum number of triangles for any single face
  List<Face>& faceLst = this->storedFaces();
  // determine how many triangles will be needed
  FOR_ALL(faceLst, faceI) {
    const label n = faceLst[faceI].nTriangles();
    if (maxTri < n) {
      maxTri = n;
    }
    nTri += n;
  }
  // nothing to do
  if (nTri <= faceLst.size()) {
    if (notNull(faceMapOut)) {
      faceMapOut.clear();
    }
    return 0;
  }
  List<Face> newFaces{nTri};
  List<label> faceMap;
  // reuse storage from optional faceMap
  if (notNull(faceMapOut)) {
    faceMap.transfer(faceMapOut);
  }
  faceMap.setSize(nTri);
  // remember the number of *additional* faces
  nTri -= faceLst.size();
  if (this->points().empty()) {
    // triangulate without points
    // simple face triangulation around f[0]
    label newFaceI = 0;
    FOR_ALL(faceLst, faceI) {
      const Face& f = faceLst[faceI];
      for (label fp = 1; fp < f.size() - 1; ++fp) {
        label fp1 = f.fcIndex(fp);
        newFaces[newFaceI] = triFace(f[0], f[fp], f[fp1]);
        faceMap[newFaceI] = faceI;
        newFaceI++;
      }
    }
  } else {
    // triangulate with points
    List<face> tmpTri{maxTri};
    label newFaceI = 0;
    FOR_ALL(faceLst, faceI) {
      // 'face' not '<Face>'
      const face& f = faceLst[faceI];
      label nTmp = 0;
      f.triangles(this->points(), nTmp, tmpTri);
      for (label triI = 0; triI < nTmp; triI++) {
        newFaces[newFaceI] = Face{static_cast<labelUList&>(tmpTri[triI])};
        faceMap[newFaceI] = faceI;
        newFaceI++;
      }
    }
  }
  faceLst.transfer(newFaces);
  remapFaces(faceMap);
  // optionally return the faceMap
  if (notNull(faceMapOut)) {
    faceMapOut.transfer(faceMap);
  }
  faceMap.clear();
  // Topology can change because of renumbering
  ParentType::clearOut();
  return nTri;
}


template<class Face>
mousse::MeshedSurface<Face> mousse::MeshedSurface<Face>::subsetMesh
(
  const labelHashSet& include,
  labelList& pointMap,
  labelList& faceMap
) const
{
  const pointField& locPoints = this->localPoints();
  const List<Face>& locFaces  = this->localFaces();
  // Fill pointMap, faceMap
  PatchTools::subsetMap(*this, include, pointMap, faceMap);
  // Create compact coordinate list and forward mapping array
  pointField newPoints{pointMap.size()};
  labelList oldToNew{locPoints.size()};
  FOR_ALL(pointMap, pointI) {
    newPoints[pointI] = locPoints[pointMap[pointI]];
    oldToNew[pointMap[pointI]] = pointI;
  }
  // create/copy a new zones list, each zone with zero size
  surfZoneList newZones{this->surfZones()};
  FOR_ALL(newZones, zoneI) {
    newZones[zoneI].size() = 0;
  }
  // Renumber face node labels
  List<Face> newFaces{faceMap.size()};
  FOR_ALL(faceMap, faceI) {
    const label origFaceI = faceMap[faceI];
    newFaces[faceI] = Face(locFaces[origFaceI]);
    // Renumber labels for face
    Face& f = newFaces[faceI];
    FOR_ALL(f, fp) {
      f[fp] = oldToNew[f[fp]];
    }
  }
  oldToNew.clear();
  // recalculate the zones start/size
  label newFaceI = 0;
  label origEndI = 0;
  // adjust zone sizes
  FOR_ALL(newZones, zoneI) {
    surfZone& zone = newZones[zoneI];
    // adjust zone start
    zone.start() = newFaceI;
    origEndI += zone.size();
    for (label faceI = newFaceI; faceI < faceMap.size(); ++faceI) {
      if (faceMap[faceI] < origEndI) {
        ++newFaceI;
      } else {
        break;
      }
    }
    // adjust zone size
    zone.size() = newFaceI - zone.start();
  }
  // construct a sub-surface
  return MeshedSurface
  (
    xferMove(newPoints),
    xferMove(newFaces),
    xferMove(newZones)
  );
}


template<class Face>
mousse::MeshedSurface<Face> mousse::MeshedSurface<Face>::subsetMesh
(
  const labelHashSet& include
) const
{
  labelList pointMap, faceMap;
  return subsetMesh(include, pointMap, faceMap);
}


template<class Face>
void mousse::MeshedSurface<Face>::transfer
(
  MeshedSurface<Face>& surf
)
{
  reset
  (
    xferMove(surf.storedPoints()),
    xferMove(surf.storedFaces()),
    xferMove(surf.storedZones())
  );
}


template<class Face>
void mousse::MeshedSurface<Face>::transfer
(
  UnsortedMeshedSurface<Face>& surf
)
{
  clear();
  labelList faceMap;
  surfZoneList zoneLst = surf.sortedZones(faceMap);
  if (zoneLst.size() <= 1) {
    reset
    (
      xferMove(surf.storedPoints()),
      xferMove(surf.storedFaces()),
      Xfer<surfZoneList>()
    );
  } else {
    List<Face>& oldFaces = surf.storedFaces();
    List<Face> newFaces{faceMap.size()};
    FOR_ALL(faceMap, faceI) {
      newFaces[faceMap[faceI]].transfer(oldFaces[faceI]);
    }
    reset
    (
      xferMove(surf.storedPoints()),
      xferMove(newFaces),
      xferMove(zoneLst)
    );
  }
  faceMap.clear();
  surf.clear();
}


template<class Face>
mousse::Xfer<mousse::MeshedSurface<Face> > mousse::MeshedSurface<Face>::xfer()
{
  return xferMove(*this);
}


// Read from file, determine format from extension
template<class Face>
bool mousse::MeshedSurface<Face>::read(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz") {
    fileName unzipName = name.lessExt();
    return read(unzipName, unzipName.ext());
  } else {
    return read(name, ext);
  }
}


// Read from file in given format
template<class Face>
bool mousse::MeshedSurface<Face>::read
(
  const fileName& name,
  const word& ext
)
{
  clear();
  // read via selector mechanism
  transfer(New(name, ext)());
  return true;
}


template<class Face>
void mousse::MeshedSurface<Face>::write
(
  const Time& t,
  const word& surfName
) const
{
  MeshedSurfaceProxy<Face>(*this).write(t, surfName);
}


// Member Operators 
template<class Face>
void mousse::MeshedSurface<Face>::operator=(const MeshedSurface& surf)
{
  clear();
  this->storedPoints() = surf.points();
  this->storedFaces()  = surf.faces();
  this->storedZones()  = surf.surfZones();
}


template<class Face>
mousse::MeshedSurface<Face>::operator mousse::MeshedSurfaceProxy<Face>() const
{
  return MeshedSurfaceProxy<Face>
  (
    this->points(),
    this->faces(),
    this->surfZones()
  );
}

#include "meshed_surface_zones.ipp"
#include "meshed_surface_io.ipp"
#include "meshed_surface_new.ipp"
