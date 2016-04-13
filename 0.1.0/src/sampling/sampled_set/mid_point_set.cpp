// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mid_point_set.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(midPointSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, midPointSet, word);

}


// Private Member Functions 
void mousse::midPointSet::genSamples()
{
  // Generate midpoints.
  List<point> midPoints{2*size()};
  labelList midCells{2*size()};
  labelList midSegments{2*size()};
  scalarList midCurveDist{2*size()};
  label midI = 0;
  label sampleI = 0;
  while(true && size()>0) {
    // calculate midpoint between sampleI and sampleI+1 (if in same segment)
    while ((sampleI < size() - 1)
           && (segments_[sampleI] == segments_[sampleI+1])) {
      midPoints[midI] = 0.5*(operator[](sampleI) + operator[](sampleI+1));
      label cell1 = getCell(faces_[sampleI], midPoints[midI]);
      label cell2 = getCell(faces_[sampleI+1], midPoints[midI]);
      if (cell1 != cell2) {
        FATAL_ERROR_IN("midPointSet::genSamples()")
          << "  sampleI:" << sampleI
          << "  midI:" << midI
          << "  sampleI:" << sampleI
          << "  pts[sampleI]:" << operator[](sampleI)
          << "  face[sampleI]:" << faces_[sampleI]
          << "  pts[sampleI+1]:" << operator[](sampleI+1)
          << "  face[sampleI+1]:" << faces_[sampleI+1]
          << "  cell1:" << cell1
          << "  cell2:" << cell2
          << abort(FatalError);
      }
      midCells[midI] = cell1;
      midSegments[midI] = segments_[sampleI];
      midCurveDist[midI] = mag(midPoints[midI] - start());
      midI++;
      sampleI++;
    }
    if (sampleI == size() - 1) {
      break;
    }
    sampleI++;
  }
  midPoints.setSize(midI);
  midCells.setSize(midI);
  midSegments.setSize(midI);
  midCurveDist.setSize(midI);
  setSamples
  (
    midPoints,
    midCells,
    labelList(midCells.size(), -1),
    midSegments,
    midCurveDist
  );
}


// Constructors 
mousse::midPointSet::midPointSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis,
  const point& start,
  const point& end
)
:
  faceOnlySet{name, mesh, searchEngine, axis, start, end}
{
  genSamples();
  if (debug) {
    write(Info);
  }
}


mousse::midPointSet::midPointSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  faceOnlySet{name, mesh, searchEngine, dict}
{
  genSamples();
  if (debug) {
    write(Info);
  }
}


// Destructor 
mousse::midPointSet::~midPointSet()
{}

