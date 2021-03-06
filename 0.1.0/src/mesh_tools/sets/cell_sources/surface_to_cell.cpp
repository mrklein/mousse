// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_to_cell.hpp"
#include "poly_mesh.hpp"
#include "mesh_search.hpp"
#include "tri_surface.hpp"
#include "tri_surface_search.hpp"
#include "cell_classification.hpp"
#include "cpu_time.hpp"
#include "demand_driven_data.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(surfaceToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, surfaceToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, surfaceToCell, istream);

}

mousse::topoSetSource::addToUsageTable mousse::surfaceToCell::usage_
{
  surfaceToCell::typeName,
  "\n    Usage: surfaceToCell"
  "<surface> <outsidePoints> <cut> <inside> <outside> <near> <curvature>\n\n"
  "    <surface> name of triSurface\n"
  "    <outsidePoints> list of points that define outside\n"
  "    <cut> boolean whether to include cells cut by surface\n"
  "    <inside>   ,,                 ,,       inside surface\n"
  "    <outside>  ,,                 ,,       outside surface\n"
  "    <near> scalar; include cells with centre <= near to surface\n"
  "    <curvature> scalar; include cells close to strong curvature"
  " on surface\n"
  "    (curvature defined as difference in surface normal at nearest"
  " point on surface for each vertex of cell)\n\n"
};


// Private Member Functions
mousse::label mousse::surfaceToCell::getNearest
(
  const triSurfaceSearch& querySurf,
  const label pointI,
  const point& pt,
  const vector& span,
  Map<label>& cache
)
{
  Map<label>::const_iterator iter = cache.find(pointI);
  if (iter != cache.end()) {
    // Found cached answer
    return iter();
  } else {
    pointIndexHit inter = querySurf.nearest(pt, span);
    // Triangle label (can be -1)
    label triI = inter.index();
    // Store triangle on point
    cache.insert(pointI, triI);
    return triI;
  }
}


bool mousse::surfaceToCell::differingPointNormals
(
  const triSurfaceSearch& querySurf,
  const vector& span,         // Current search span
  const label cellI,
  const label cellTriI,       // Nearest (to cell centre) surface triangle
  Map<label>& pointToNearest  // Cache for nearest triangle to point
) const
{
  const triSurface& surf = querySurf.surface();
  const vectorField& normals = surf.faceNormals();
  const faceList& faces = mesh().faces();
  const pointField& points = mesh().points();
  const labelList& cFaces = mesh().cells()[cellI];
  FOR_ALL(cFaces, cFaceI) {
    const face& f = faces[cFaces[cFaceI]];
    FOR_ALL(f, fp) {
      label pointI = f[fp];
      label pointTriI =
        getNearest
        (
          querySurf,
          pointI,
          points[pointI],
          span,
          pointToNearest
        );
      if (pointTriI != -1 && pointTriI != cellTriI) {
        scalar cosAngle = normals[pointTriI] & normals[cellTriI];
        if (cosAngle < 0.9) {
          return true;
        }
      }
    }
  }
  return false;
}


void mousse::surfaceToCell::combine(topoSet& set, const bool add) const
{
  cpuTime timer;
  if (useSurfaceOrientation_ && (includeInside_ || includeOutside_)) {
    const meshSearch queryMesh{mesh_};
    //- Calculate for each searchPoint inside/outside status.
    boolList isInside{querySurf().calcInside(mesh_.cellCentres())};
    Info << "    Marked inside/outside using surface orientation in = "
      << timer.cpuTimeIncrement() << " s" << endl << endl;
    FOR_ALL(isInside, cellI) {
      if (isInside[cellI] && includeInside_) {
        addOrDelete(set, cellI, add);
      } else if (!isInside[cellI] && includeOutside_) {
        addOrDelete(set, cellI, add);
      }
    }
  } else if (includeCut_ || includeInside_ || includeOutside_) {
    //
    // Cut cells with surface and classify cells
    //
    // Construct search engine on mesh
    const meshSearch queryMesh{mesh_};
    // Check all 'outside' points
    FOR_ALL(outsidePoints_, outsideI) {
      const point& outsidePoint = outsidePoints_[outsideI];
      // Find cell point is in. Linear search.
      label cellI = queryMesh.findCell(outsidePoint, -1, false);
      if (returnReduce(cellI, maxOp<label>()) == -1) {
        FATAL_ERROR_IN("surfaceToCell::combine(topoSet&, const bool)")
          << "outsidePoint " << outsidePoint
          << " is not inside any cell"
          << exit(FatalError);
      }
    }
    // Cut faces with surface and classify cells
    cellClassification cellType
    {
      mesh_,
      queryMesh,
      querySurf(),
      outsidePoints_
    };
    Info << "    Marked inside/outside using surface intersection in = "
      << timer.cpuTimeIncrement() << " s" << endl << endl;
    //- Add/remove cells using set
    FOR_ALL(cellType, cellI) {
      label cType = cellType[cellI];
      if ((includeCut_ && (cType == cellClassification::CUT))
          || (includeInside_ && (cType == cellClassification::INSIDE))
          || (includeOutside_ && (cType == cellClassification::OUTSIDE))) {
        addOrDelete(set, cellI, add);
      }
    }
  }
  if (nearDist_ > 0) {
    //
    // Determine distance to surface
    //
    const pointField& ctrs = mesh_.cellCentres();
    // Box dimensions to search in octree.
    const vector span{nearDist_, nearDist_, nearDist_};
    if (curvature_ < -1) {
      Info << "    Selecting cells with cellCentre closer than "
        << nearDist_ << " to surface" << endl;
      // No need to test curvature. Insert near cells into set.
      FOR_ALL(ctrs, cellI) {
        const point& c = ctrs[cellI];
        pointIndexHit inter = querySurf().nearest(c, span);
        if (inter.hit() && (mag(inter.hitPoint() - c) < nearDist_)) {
          addOrDelete(set, cellI, add);
        }
      }
      Info << "    Determined nearest surface point in = "
        << timer.cpuTimeIncrement() << " s" << endl << endl;
    } else {
      // Test near cells for curvature
      Info << "    Selecting cells with cellCentre closer than "
        << nearDist_ << " to surface and curvature factor"
        << " less than " << curvature_ << endl;
      // Cache for nearest surface triangle for a point
      Map<label> pointToNearest{mesh_.nCells()/10};
      FOR_ALL(ctrs, cellI) {
        const point& c = ctrs[cellI];
        pointIndexHit inter = querySurf().nearest(c, span);
        if (inter.hit() && (mag(inter.hitPoint() - c) < nearDist_)) {
          if (differingPointNormals
              (
                querySurf(),
                span,
                cellI,
                inter.index(),      // nearest surface triangle
                pointToNearest
              )) {
            addOrDelete(set, cellI, add);
          }
        }
      }
      Info << "    Determined nearest surface point in = "
        << timer.cpuTimeIncrement() << " s" << endl << endl;
    }
  }
}


void mousse::surfaceToCell::checkSettings() const
{
  if ((nearDist_ < 0) && (curvature_ < -1)
      && ((includeCut_ && includeInside_ && includeOutside_)
          || (!includeCut_ && !includeInside_ && !includeOutside_))) {
    FATAL_ERROR_IN
    (
      "surfaceToCell:checkSettings()"
    )
    << "Illegal include cell specification."
    << " Result would be either all or no cells." << endl
    << "Please set one of includeCut, includeInside, includeOutside"
    << " to true, set nearDistance to a value > 0"
    << " or set curvature to a value -1 .. 1."
    << exit(FatalError);
  }
  if (useSurfaceOrientation_ && includeCut_) {
    FATAL_ERROR_IN
    (
      "surfaceToCell:checkSettings()"
    )
    << "Illegal include cell specification."
    << " You cannot specify both 'useSurfaceOrientation'"
    << " and 'includeCut'"
    << " since 'includeCut' specifies a topological split"
    << exit(FatalError);
  }
}


// Constructors
mousse::surfaceToCell::surfaceToCell
(
  const polyMesh& mesh,
  const fileName& surfName,
  const pointField& outsidePoints,
  const bool includeCut,
  const bool includeInside,
  const bool includeOutside,
  const bool useSurfaceOrientation,
  const scalar nearDist,
  const scalar curvature
)
:
  topoSetSource{mesh},
  surfName_{surfName},
  outsidePoints_{outsidePoints},
  includeCut_{includeCut},
  includeInside_{includeInside},
  includeOutside_{includeOutside},
  useSurfaceOrientation_{useSurfaceOrientation},
  nearDist_{nearDist},
  curvature_{curvature},
  surfPtr_{new triSurface{surfName_}},
  querySurfPtr_{new triSurfaceSearch{*surfPtr_}},
  IOwnPtrs_{true}
{
  checkSettings();
}


mousse::surfaceToCell::surfaceToCell
(
  const polyMesh& mesh,
  const fileName& surfName,
  const triSurface& surf,
  const triSurfaceSearch& querySurf,
  const pointField& outsidePoints,
  const bool includeCut,
  const bool includeInside,
  const bool includeOutside,
  const bool useSurfaceOrientation,
  const scalar nearDist,
  const scalar curvature
)
:
  topoSetSource{mesh},
  surfName_{surfName},
  outsidePoints_{outsidePoints},
  includeCut_{includeCut},
  includeInside_{includeInside},
  includeOutside_{includeOutside},
  useSurfaceOrientation_{useSurfaceOrientation},
  nearDist_{nearDist},
  curvature_{curvature},
  surfPtr_{&surf},
  querySurfPtr_{&querySurf},
  IOwnPtrs_{false}
{
  checkSettings();
}


mousse::surfaceToCell::surfaceToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  surfName_{fileName(dict.lookup("file")).expand()},
  outsidePoints_{dict.lookup("outsidePoints")},
  includeCut_{readBool(dict.lookup("includeCut"))},
  includeInside_{readBool(dict.lookup("includeInside"))},
  includeOutside_{readBool(dict.lookup("includeOutside"))},
  useSurfaceOrientation_
  {
    dict.lookupOrDefault<bool>("useSurfaceOrientation", false)
  },
  nearDist_{readScalar(dict.lookup("nearDistance"))},
  curvature_{readScalar(dict.lookup("curvature"))},
  surfPtr_{new triSurface(surfName_)},
  querySurfPtr_{new triSurfaceSearch(*surfPtr_)},
  IOwnPtrs_{true}
{
  checkSettings();
}


mousse::surfaceToCell::surfaceToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  surfName_{checkIs(is)},
  outsidePoints_{checkIs(is)},
  includeCut_{readBool(checkIs(is))},
  includeInside_{readBool(checkIs(is))},
  includeOutside_{readBool(checkIs(is))},
  useSurfaceOrientation_{false},
  nearDist_{readScalar(checkIs(is))},
  curvature_{readScalar(checkIs(is))},
  surfPtr_{new triSurface{surfName_}},
  querySurfPtr_{new triSurfaceSearch{*surfPtr_}},
  IOwnPtrs_{true}
{
  checkSettings();
}


// Destructor
mousse::surfaceToCell::~surfaceToCell()
{
  if (IOwnPtrs_) {
    deleteDemandDrivenData(surfPtr_);
    deleteDemandDrivenData(querySurfPtr_);
  }
}


// Member Functions
void mousse::surfaceToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding cells in relation to surface " << surfName_
      << " ..." << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing cells in relation to surface " << surfName_
      << " ..." << endl;
    combine(set, false);
  }
}
