// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_mesh_point_set.hpp"
#include "mesh_search.hpp"
#include "dynamic_list.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(triSurfaceMeshPointSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, triSurfaceMeshPointSet, word);

}


// Private Member Functions 
void mousse::triSurfaceMeshPointSet::calcSamples
(
  DynamicList<point>& samplingPts,
  DynamicList<label>& samplingCells,
  DynamicList<label>& samplingFaces,
  DynamicList<label>& samplingSegments,
  DynamicList<scalar>& samplingCurveDist
) const
{
  FOR_ALL(sampleCoords_, sampleI) {
    label cellI = searchEngine().findCell(sampleCoords_[sampleI]);
    if (cellI == -1)
      continue;
    samplingPts.append(sampleCoords_[sampleI]);
    samplingCells.append(cellI);
    samplingFaces.append(-1);
    samplingSegments.append(0);
    samplingCurveDist.append(1.0 * sampleI);
  }
}


void mousse::triSurfaceMeshPointSet::genSamples()
{
  // Storage for sample points
  DynamicList<point> samplingPts;
  DynamicList<label> samplingCells;
  DynamicList<label> samplingFaces;
  DynamicList<label> samplingSegments;
  DynamicList<scalar> samplingCurveDist;
  calcSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
  samplingPts.shrink();
  samplingCells.shrink();
  samplingFaces.shrink();
  samplingSegments.shrink();
  samplingCurveDist.shrink();
  setSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
}


// Constructors 
mousse::triSurfaceMeshPointSet::triSurfaceMeshPointSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  sampledSet{name, mesh, searchEngine, dict},
  surface_{dict.lookup("surface")}
{
  // Load surface.
  if (mesh.time().foundObject<triSurfaceMesh>(surface_)) {
    // Note: should use localPoints() instead of points() but assume
    // trisurface is compact.
    sampleCoords_ = mesh.time().lookupObject<triSurfaceMesh>
    (
      surface_
    ).points();
  } else {
    sampleCoords_ =
      triSurfaceMesh
      {
        IOobject
        {
          surface_,
          mesh.time().constant(),     // instance
          "triSurface",               // local
          mesh.time(),
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        }
      }.points();
  }
  genSamples();
  if (debug) {
    write(Info);
  }
}


// Destructor 
mousse::triSurfaceMeshPointSet::~triSurfaceMeshPointSet()
{}


// Member Functions 
mousse::point mousse::triSurfaceMeshPointSet::getRefPoint(const List<point>& pts)
const
{
  if (pts.size()) {
    // Use first samplePt as starting point
    return pts[0];
  } else {
    return vector::zero;
  }
}

