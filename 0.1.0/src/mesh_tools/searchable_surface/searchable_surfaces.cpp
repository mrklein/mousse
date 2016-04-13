// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "searchable_surfaces.hpp"
#include "searchable_surfaces_queries.hpp"
#include "list_ops.hpp"
#include "time.hpp"
#include "dynamic_field.hpp"
#include "patch_tools.hpp"
#include "tri_surface_mesh.hpp"
#include "pstream_reduce_ops.hpp"
#include "coord_set.hpp"
#include "ofstream.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(searchableSurfaces, 0);

}


// Private Member Functions
//- Is edge connected to triangle
bool mousse::searchableSurfaces::connected
(
  const triSurface& s,
  const label edgeI,
  const pointIndexHit& hit
)
{
  const triFace& localFace = s.localFaces()[hit.index()];
  const edge& e = s.edges()[edgeI];
  FOR_ALL(localFace, i) {
    if (e.otherVertex(localFace[i]) != -1) {
      return true;
    }
  }
  return false;
}


// Constructors

// Construct with length.
mousse::searchableSurfaces::searchableSurfaces(const label size)
:
  PtrList<searchableSurface>{size},
  regionNames_{size},
  allSurfaces_{identity(size)}
{}


mousse::searchableSurfaces::searchableSurfaces
(
  const IOobject& io,
  const dictionary& topDict,
  const bool singleRegionName
)
:
  PtrList<searchableSurface>{topDict.size()},
  names_{topDict.size()},
  regionNames_{topDict.size()},
  allSurfaces_{identity(topDict.size())}
{
  label surfI = 0;
  FOR_ALL_CONST_ITER(dictionary, topDict, iter) {
    const word& key = iter().keyword();
    if (!topDict.isDict(key)) {
      FATAL_ERROR_IN
      (
        "searchableSurfaces::searchableSurfaces"
        "( const IOobject&, const dictionary&)"
      )
      << "Found non-dictionary entry " << iter()
      << " in top-level dictionary " << topDict
      << exit(FatalError);
    }
    const dictionary& dict = topDict.subDict(key);
    names_[surfI] = key;
    dict.readIfPresent("name", names_[surfI]);
    // Make IOobject with correct name
    autoPtr<IOobject> namedIO{io.clone()};
    // Note: we would like to e.g. register triSurface 'sphere.stl' as
    // 'sphere'. Unfortunately
    // no support for having object read from different location than
    // their object name. Maybe have stlTriSurfaceMesh which appends .stl
    // when reading/writing?
    namedIO().rename(key);  // names_[surfI]
    // Create and hook surface
    set
    (
      surfI,
      searchableSurface::New
      (
        dict.lookup("type"),
        namedIO(),
        dict
      )
    );
    const searchableSurface& s = operator[](surfI);
    // Construct default region names by prepending surface name
    // to region name.
    const wordList& localNames = s.regions();
    wordList& rNames = regionNames_[surfI];
    rNames.setSize(localNames.size());
    if (singleRegionName && localNames.size() == 1) {
      rNames[0] = names_[surfI];
    } else {
      FOR_ALL(localNames, regionI) {
        rNames[regionI] = names_[surfI] + '_' + localNames[regionI];
      }
    }
    // See if dictionary provides any global region names.
    if (dict.found("regions")) {
      const dictionary& regionsDict = dict.subDict("regions");
      FOR_ALL_CONST_ITER(dictionary, regionsDict, iter) {
        const word& key = iter().keyword();
        if (regionsDict.isDict(key)) {
          // Get the dictionary for region iter.keyword()
          const dictionary& regionDict = regionsDict.subDict(key);
          label index = findIndex(localNames, key);
          if (index == -1) {
            FATAL_ERROR_IN
            (
              "searchableSurfaces::searchableSurfaces"
              "( const IOobject&, const dictionary&)"
            )
            << "Unknown region name " << key
            << " for surface " << s.name() << endl
            << "Valid region names are " << localNames
            << exit(FatalError);
          }
          rNames[index] = word(regionDict.lookup("name"));
        }
      }
    }
    surfI++;
  }
  // Trim (not really necessary since we don't allow non-dictionary entries)
  PtrList<searchableSurface>::setSize(surfI);
  names_.setSize(surfI);
  regionNames_.setSize(surfI);
  allSurfaces_.setSize(surfI);
}


// Member Functions
mousse::label mousse::searchableSurfaces::findSurfaceID
(
  const word& wantedName
) const
{
  return findIndex(names_, wantedName);
}


mousse::label mousse::searchableSurfaces::findSurfaceRegionID
(
  const word& surfaceName,
  const word& regionName
) const
{
  label surfaceIndex = findSurfaceID(surfaceName);
  return findIndex(this->operator[](surfaceIndex).regions(), regionName);
}


// Find any intersection
void mousse::searchableSurfaces::findAnyIntersection
(
  const pointField& start,
  const pointField& end,
  labelList& hitSurfaces,
  List<pointIndexHit>& hitInfo
) const
{
  searchableSurfacesQueries::findAnyIntersection
  (
    *this,
    allSurfaces_,
    start,
    end,
    hitSurfaces,
    hitInfo
  );
}


//- Find all intersections in order from start to end. Returns for
//  every hit the surface and the hit info.
void mousse::searchableSurfaces::findAllIntersections
(
  const pointField& start,
  const pointField& end,
  labelListList& hitSurfaces,
  List<List<pointIndexHit> >& hitInfo
) const
{
  searchableSurfacesQueries::findAllIntersections
  (
    *this,
    allSurfaces_,
    start,
    end,
    hitSurfaces,
    hitInfo
  );
}


//Find intersections of edge nearest to both endpoints.
void mousse::searchableSurfaces::findNearestIntersection
(
  const pointField& start,
  const pointField& end,
  labelList& surface1,
  List<pointIndexHit>& hit1,
  labelList& surface2,
  List<pointIndexHit>& hit2
) const
{
  searchableSurfacesQueries::findNearestIntersection
  (
    *this,
    allSurfaces_,
    start,
    end,
    surface1,
    hit1,
    surface2,
    hit2
  );
}


// Find nearest. Return -1 or nearest point
void mousse::searchableSurfaces::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  labelList& nearestSurfaces,
  List<pointIndexHit>& nearestInfo
) const
{
  searchableSurfacesQueries::findNearest
  (
    *this,
    allSurfaces_,
    samples,
    nearestDistSqr,
    nearestSurfaces,
    nearestInfo
  );
}


// Find nearest. Return -1 or nearest point
void mousse::searchableSurfaces::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  const labelList& regionIndices,
  labelList& nearestSurfaces,
  List<pointIndexHit>& nearestInfo
) const
{
  searchableSurfacesQueries::findNearest
  (
    *this,
    allSurfaces_,
    samples,
    nearestDistSqr,
    regionIndices,
    nearestSurfaces,
    nearestInfo
  );
}


//- Calculate bounding box
mousse::boundBox mousse::searchableSurfaces::bounds() const
{
  return searchableSurfacesQueries::bounds
  (
    *this,
    allSurfaces_
  );
}


//- Calculate point which is on a set of surfaces.
mousse::pointIndexHit mousse::searchableSurfaces::facesIntersection
(
  const scalar initDistSqr,
  const scalar convergenceDistSqr,
  const point& start
) const
{
  return searchableSurfacesQueries::facesIntersection
  (
    *this,
    allSurfaces_,
    initDistSqr,
    convergenceDistSqr,
    start
  );
}


bool mousse::searchableSurfaces::checkClosed(const bool report) const
{
  if (report) {
    Info << "Checking for closedness." << endl;
  }
  bool hasError = false;
  FOR_ALL(*this, surfI) {
    if (!operator[](surfI).hasVolumeType()) {
      hasError = true;
      if (report) {
        Info << "    " << names()[surfI] << " : not closed" << endl;
      }
      if (isA<triSurface>(operator[](surfI))) {
        const triSurface& s = dynamic_cast<const triSurface&>
        (
          operator[](surfI)
        );
        const labelListList& edgeFaces = s.edgeFaces();
        label nSingleEdges = 0;
        FOR_ALL(edgeFaces, edgeI) {
          if (edgeFaces[edgeI].size() == 1) {
            nSingleEdges++;
          }
        }
        label nMultEdges = 0;
        FOR_ALL(edgeFaces, edgeI) {
          if (edgeFaces[edgeI].size() > 2) {
            nMultEdges++;
          }
        }
        if (report && (nSingleEdges != 0 || nMultEdges != 0)) {
          Info << "        connected to one face : "
            << nSingleEdges << nl
            << "        connected to >2 faces : "
            << nMultEdges << endl;
        }
      }
    }
  }
  if (report) {
    Info << endl;
  }
  return returnReduce(hasError, orOp<bool>());
}


bool mousse::searchableSurfaces::checkNormalOrientation(const bool report) const
{
  if (report) {
    Info << "Checking for normal orientation." << endl;
  }
  bool hasError = false;
  FOR_ALL(*this, surfI) {
    if (isA<triSurface>(operator[](surfI))) {
      const triSurface& s = dynamic_cast<const triSurface&>
      (
        operator[](surfI)
      );
      labelHashSet borderEdge{s.size()/1000};
      PatchTools::checkOrientation(s, false, &borderEdge);
      // Colour all faces into zones using borderEdge
      labelList normalZone;
      label nZones = PatchTools::markZones(s, borderEdge, normalZone);
      if (nZones > 1) {
        hasError = true;
        if (report) {
          Info << "    " << names()[surfI]
            << " : has multiple orientation zones ("
            << nZones << ")" << endl;
        }
      }
    }
  }
  if (report) {
    Info << endl;
  }
  return returnReduce(hasError, orOp<bool>());
}


bool mousse::searchableSurfaces::checkSizes
(
  const scalar maxRatio,
  const bool report
) const
{
  if (report) {
    Info << "Checking for size." << endl;
  }
  bool hasError = false;
  FOR_ALL(*this, i) {
    const boundBox& bb = operator[](i).bounds();
    for (label j = i+1; j < size(); j++) {
      scalar ratio = bb.mag()/operator[](j).bounds().mag();
      if (ratio > maxRatio || ratio < 1.0/maxRatio) {
        hasError = true;
        if (report) {
          Info << "    " << names()[i]
            << " bounds differ from " << names()[j]
            << " by more than a factor 100:" << nl
            << "        bounding box : " << bb << nl
            << "        bounding box : " << operator[](j).bounds()
            << endl;
        }
        break;
      }
    }
  }
  if (report) {
    Info << endl;
  }
  return returnReduce(hasError, orOp<bool>());
}


bool mousse::searchableSurfaces::checkIntersection
(
  const scalar tolerance,
  const autoPtr<writer<scalar> >& setWriter,
  const bool report
) const
{
  if (report) {
    Info << "Checking for intersection." << endl;
  }
  //cpuTime timer;
  bool hasError = false;
  FOR_ALL(*this, i) {
    if (isA<triSurfaceMesh>(operator[](i))) {
      const triSurfaceMesh& s0 = dynamic_cast<const triSurfaceMesh&>
      (
        operator[](i)
      );
      const edgeList& edges0 = s0.edges();
      const pointField& localPoints0 = s0.localPoints();
      // Collect all the edge vectors
      pointField start{edges0.size()};
      pointField end{edges0.size()};
      FOR_ALL(edges0, edgeI) {
        const edge& e = edges0[edgeI];
        start[edgeI] = localPoints0[e[0]];
        end[edgeI] = localPoints0[e[1]];
      }
      // Test all other surfaces for intersection
      FOR_ALL(*this, j) {
        List<pointIndexHit> hits;
        if (i == j) {
          // Slightly shorten the edges to prevent finding lots of
          // intersections. The fast triangle intersection routine
          // has problems with rays passing through a point of the
          // triangle.
          vectorField d
          {
            max(tolerance, 10*s0.tolerance())*(end-start)
          };
          start += d;
          end -= d;
        }
        operator[](j).findLineAny(start, end, hits);
        label nHits = 0;
        DynamicField<point> intersections{edges0.size()/100};
        DynamicField<scalar> intersectionEdge{intersections.capacity()};
        FOR_ALL(hits, edgeI) {
          if (hits[edgeI].hit()
              && (i != j || !connected(s0, edgeI, hits[edgeI]))) {
            intersections.append(hits[edgeI].hitPoint());
            intersectionEdge.append(1.0*edgeI);
            nHits++;
          }
        }
        if (nHits > 0) {
          if (report) {
            Info << "    " << names()[i]
              << " intersects " << names()[j]
              << " at " << nHits
              << " locations."
              << endl;
            if (setWriter.valid()) {
              scalarField dist(mag(intersections));
              coordSet track
              {
                names()[i] + '_' + names()[j],
                "xyz",
                intersections.xfer(),
                dist
              };
              wordList valueSetNames{1, "edgeIndex"};
              List<const scalarField*> valueSets
              {
                1,
                &intersectionEdge
              };
              fileName fName
              {
                setWriter().getFileName(track, valueSetNames)
              };
              Info << "    Writing intersection locations to "
                << fName << endl;
              OFstream os
              {
                s0.searchableSurface::time().path()/fName
              };
              setWriter().write
              (
                track,
                valueSetNames,
                valueSets,
                os
              );
            }
          }
          hasError = true;
          break;
        }
      }
    }
  }
  if (report) {
    Info << endl;
  }
  return returnReduce(hasError, orOp<bool>());
}


bool mousse::searchableSurfaces::checkQuality
(
  const scalar minQuality,
  const bool report
) const
{
  if (report) {
    Info << "Checking for triangle quality." << endl;
  }
  bool hasError = false;
  FOR_ALL(*this, surfI) {
    if (isA<triSurface>(operator[](surfI))) {
      const triSurface& s = dynamic_cast<const triSurface&>
      (
        operator[](surfI)
      );
      label nBadTris = 0;
      FOR_ALL(s, faceI) {
        const labelledTri& f = s[faceI];
        scalar q = triPointRef
        {
          s.points()[f[0]],
          s.points()[f[1]],
          s.points()[f[2]]
        }.quality();
        if (q < minQuality) {
          nBadTris++;
        }
      }
      if (nBadTris > 0) {
        hasError = true;
        if (report) {
          Info << "    " << names()[surfI]
            << " : has " << nBadTris << " bad quality triangles "
            << " (quality < " << minQuality << ")" << endl;
        }
      }
    }
  }
  if (report) {
    Info << endl;
  }
  return returnReduce(hasError, orOp<bool>());
}


// Check all surfaces. Return number of failures.
mousse::label mousse::searchableSurfaces::checkTopology
(
  const bool report
) const
{
  label noFailedChecks = 0;
  if (checkClosed(report)) {
    noFailedChecks++;
  }
  if (checkNormalOrientation(report)) {
    noFailedChecks++;
  }
  return noFailedChecks;
}


mousse::label mousse::searchableSurfaces::checkGeometry
(
  const scalar maxRatio,
  const scalar tol,
  const autoPtr<writer<scalar> >& setWriter,
  const scalar minQuality,
  const bool report
) const
{
  label noFailedChecks = 0;
  if (maxRatio > 0 && checkSizes(maxRatio, report)) {
    noFailedChecks++;
  }
  if (checkIntersection(tol, setWriter, report)) {
    noFailedChecks++;
  }
  if (checkQuality(minQuality, report)) {
    noFailedChecks++;
  }
  return noFailedChecks;
}


void mousse::searchableSurfaces::writeStats
(
  const List<wordList>& patchTypes,
  Ostream&
) const
{
  Info << "Statistics." << endl;
  FOR_ALL(*this, surfI) {
    Info << "    " << names()[surfI] << ':' << endl;
    const searchableSurface& s = operator[](surfI);
    Info << "        type      : " << s.type() << nl
      << "        size      : " << s.globalSize() << nl;
    if (isA<triSurfaceMesh>(s)) {
      const triSurfaceMesh& ts = dynamic_cast<const triSurfaceMesh&>(s);
      Info << "        edges     : " << ts.nEdges() << nl
        << "        points    : " << ts.points()().size() << nl;
    }
    Info << "        bounds    : " << s.bounds() << nl
      << "        closed    : " << Switch(s.hasVolumeType()) << endl;
    if (patchTypes.size() && patchTypes[surfI].size() >= 1) {
      wordList unique{HashSet<word>(patchTypes[surfI]).sortedToc()};
      Info << "        patches   : ";
      FOR_ALL(unique, i) {
        Info << unique[i];
        if (i < unique.size()-1) {
          Info << ',';
        }
      }
      Info << endl;
    }
  }
  Info << endl;
}


// Member Operators
const mousse::searchableSurface& mousse::searchableSurfaces::operator[]
(
  const word& surfName
) const
{
  const label surfI = findSurfaceID(surfName);
  if (surfI < 0) {
    FATAL_ERROR_IN
    (
      "searchableSurfaces::operator[](const word&) const"
    )
    << "Surface named " << surfName << " not found." << nl
    << "Available surface names: " << names_ << endl
    << abort(FatalError);
  }
  return operator[](surfI);
}


mousse::searchableSurface& mousse::searchableSurfaces::operator[]
(
  const word& surfName
)
{
  const label surfI = findSurfaceID(surfName);
  if (surfI < 0) {
    FATAL_ERROR_IN
    (
      "searchableSurfaces::operator[](const word&)"
    )
    << "Surface named " << surfName << " not found." << nl
    << "Available surface names: " << names_ << endl
    << abort(FatalError);
  }
  return operator[](surfI);
}

