// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_set.hpp"
#include "sampled_set.hpp"
#include "mesh_search.hpp"
#include "dynamic_list.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "word.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(cloudSet, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, cloudSet, word);
}
// Private Member Functions 
void mousse::cloudSet::calcSamples
(
  DynamicList<point>& samplingPts,
  DynamicList<label>& samplingCells,
  DynamicList<label>& samplingFaces,
  DynamicList<label>& samplingSegments,
  DynamicList<scalar>& samplingCurveDist
) const
{
  const meshSearch& queryMesh = searchEngine();
  FOR_ALL(sampleCoords_, sampleI)
  {
    label cellI = queryMesh.findCell(sampleCoords_[sampleI]);
    if (cellI != -1)
    {
      samplingPts.append(sampleCoords_[sampleI]);
      samplingCells.append(cellI);
      samplingFaces.append(-1);
      samplingSegments.append(0);
      samplingCurveDist.append(1.0 * sampleI);
    }
  }
}
void mousse::cloudSet::genSamples()
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
mousse::cloudSet::cloudSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis,
  const List<point>& sampleCoords
)
:
  sampledSet(name, mesh, searchEngine, axis),
  sampleCoords_(sampleCoords)
{
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
mousse::cloudSet::cloudSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  sampledSet(name, mesh, searchEngine, dict),
  sampleCoords_(dict.lookup("points"))
{
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
// Destructor 
mousse::cloudSet::~cloudSet()
{}
