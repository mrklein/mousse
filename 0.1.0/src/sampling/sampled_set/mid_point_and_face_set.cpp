// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mid_point_and_face_set.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(midPointAndFaceSet, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, midPointAndFaceSet, word);
}
// Private Member Functions 
// Rework faceOnlySet samples.
// Take two consecutive samples
void mousse::midPointAndFaceSet::genSamples()
{
  // Generate midpoints and add to face points
  List<point> newSamplePoints(3*size());
  labelList newSampleCells(3*size());
  labelList newSampleFaces(3*size());
  labelList newSampleSegments(3*size());
  scalarList newSampleCurveDist(3*size());
  label newSampleI = 0;
  label sampleI = 0;
  while(true && size()>0)
  {
    // sampleI is start of segment
    // Add sampleI
    newSamplePoints[newSampleI] = operator[](sampleI);
    newSampleCells[newSampleI] = cells_[sampleI];
    newSampleFaces[newSampleI] = faces_[sampleI];
    newSampleSegments[newSampleI] = segments_[sampleI];
    newSampleCurveDist[newSampleI] = curveDist_[sampleI];
    newSampleI++;
    while
    (
      (sampleI < size() - 1)
    && (segments_[sampleI] == segments_[sampleI+1])
    )
    {
      // Add mid point
      const point mid = 0.5*(operator[](sampleI) + operator[](sampleI+1));
      label cell1 = getCell(faces_[sampleI], mid);
      label cell2 = getCell(faces_[sampleI+1], mid);
      if (cell1 != cell2)
      {
        FATAL_ERROR_IN("midPointAndFaceSet::genSamples()")
          << "  sampleI:" << sampleI
          << "  newSampleI:" << newSampleI
          << "  pts[sampleI]:" << operator[](sampleI)
          << "  face[sampleI]:" << faces_[sampleI]
          << "  pts[sampleI+1]:" << operator[](sampleI+1)
          << "  face[sampleI+1]:" << faces_[sampleI+1]
          << "  cell1:" << cell1
          << "  cell2:" << cell2
          << abort(FatalError);
      }
      newSamplePoints[newSampleI] = mid;
      newSampleCells[newSampleI] = cell1;
      newSampleFaces[newSampleI] = -1;
      newSampleSegments[newSampleI] = segments_[sampleI];
      newSampleCurveDist[newSampleI] =
        mag(newSamplePoints[newSampleI] - start());
      newSampleI++;
      // Add sampleI+1
      newSamplePoints[newSampleI] = operator[](sampleI+1);
      newSampleCells[newSampleI] = cells_[sampleI+1];
      newSampleFaces[newSampleI] = faces_[sampleI+1];
      newSampleSegments[newSampleI] = segments_[sampleI+1];
      newSampleCurveDist[newSampleI] =
        mag(newSamplePoints[newSampleI] - start());
      newSampleI++;
      sampleI++;
    }
    if (sampleI == size() - 1)
    {
      break;
    }
    sampleI++;
  }
  newSamplePoints.setSize(newSampleI);
  newSampleCells.setSize(newSampleI);
  newSampleFaces.setSize(newSampleI);
  newSampleSegments.setSize(newSampleI);
  newSampleCurveDist.setSize(newSampleI);
  setSamples
  (
    newSamplePoints,
    newSampleCells,
    newSampleFaces,
    newSampleSegments,
    newSampleCurveDist
  );
}
// Constructors 
mousse::midPointAndFaceSet::midPointAndFaceSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis,
  const point& start,
  const point& end
)
:
  faceOnlySet(name, mesh, searchEngine, axis, start, end)
{
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
mousse::midPointAndFaceSet::midPointAndFaceSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  faceOnlySet(name, mesh, searchEngine, dict)
{
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
// Destructor 
mousse::midPointAndFaceSet::~midPointAndFaceSet()
{}
