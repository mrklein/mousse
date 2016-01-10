// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "demand_driven_data.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(primitiveMesh, 0);

}

// Constructors 
mousse::primitiveMesh::primitiveMesh()
:
  nInternalPoints_(0),    // note: points are considered ordered on empty mesh
  nPoints_(0),
  nInternal0Edges_(-1),
  nInternal1Edges_(-1),
  nInternalEdges_(-1),
  nEdges_(-1),
  nInternalFaces_(0),
  nFaces_(0),
  nCells_(0),
  cellShapesPtr_(NULL),
  edgesPtr_(NULL),
  ccPtr_(NULL),
  ecPtr_(NULL),
  pcPtr_(NULL),
  cfPtr_(NULL),
  efPtr_(NULL),
  pfPtr_(NULL),
  cePtr_(NULL),
  fePtr_(NULL),
  pePtr_(NULL),
  ppPtr_(NULL),
  cpPtr_(NULL),
  labels_(0),
  cellCentresPtr_(NULL),
  faceCentresPtr_(NULL),
  cellVolumesPtr_(NULL),
  faceAreasPtr_(NULL)
{}
// Construct from components
// WARNING: ASSUMES CORRECT ORDERING OF DATA.
mousse::primitiveMesh::primitiveMesh
(
  const label nPoints,
  const label nInternalFaces,
  const label nFaces,
  const label nCells
)
:
  nInternalPoints_(-1),
  nPoints_(nPoints),
  nEdges_(-1),
  nInternalFaces_(nInternalFaces),
  nFaces_(nFaces),
  nCells_(nCells),
  cellShapesPtr_(NULL),
  edgesPtr_(NULL),
  ccPtr_(NULL),
  ecPtr_(NULL),
  pcPtr_(NULL),
  cfPtr_(NULL),
  efPtr_(NULL),
  pfPtr_(NULL),
  cePtr_(NULL),
  fePtr_(NULL),
  pePtr_(NULL),
  ppPtr_(NULL),
  cpPtr_(NULL),
  labels_(0),
  cellCentresPtr_(NULL),
  faceCentresPtr_(NULL),
  cellVolumesPtr_(NULL),
  faceAreasPtr_(NULL)
{}
// Destructor 
mousse::primitiveMesh::~primitiveMesh()
{
  clearOut();
}
// Member Functions 
bool mousse::primitiveMesh::calcPointOrder
(
  label& nInternalPoints,
  labelList& oldToNew,
  const faceList& faces,
  const label nInternalFaces,
  const label nPoints
)
{
  // Internal points are points that are not used by a boundary face.
  // Map from old to new position
  oldToNew.setSize(nPoints);
  oldToNew = -1;
  // 1. Create compact addressing for boundary points. Start off by indexing
  // from 0 inside oldToNew. (shifted up later on)
  label nBoundaryPoints = 0;
  for (label faceI = nInternalFaces; faceI < faces.size(); faceI++)
  {
    const face& f = faces[faceI];
    FOR_ALL(f, fp)
    {
      label pointI = f[fp];
      if (oldToNew[pointI] == -1)
      {
        oldToNew[pointI] = nBoundaryPoints++;
      }
    }
  }
  // Now we know the number of boundary and internal points
  nInternalPoints = nPoints - nBoundaryPoints;
  // Move the boundary addressing up
  FOR_ALL(oldToNew, pointI)
  {
    if (oldToNew[pointI] != -1)
    {
      oldToNew[pointI] += nInternalPoints;
    }
  }
  // 2. Compact the internal points. Detect whether internal and boundary
  // points are mixed.
  label internalPointI = 0;
  bool ordered = true;
  for (label faceI = 0; faceI < nInternalFaces; faceI++)
  {
    const face& f = faces[faceI];
    FOR_ALL(f, fp)
    {
      label pointI = f[fp];
      if (oldToNew[pointI] == -1)
      {
        if (pointI >= nInternalPoints)
        {
          ordered = false;
        }
        oldToNew[pointI] = internalPointI++;
      }
    }
  }
  return ordered;
}
void mousse::primitiveMesh::reset
(
  const label nPoints,
  const label nInternalFaces,
  const label nFaces,
  const label nCells
)
{
  clearOut();
  nPoints_ = nPoints;
  nEdges_ = -1;
  nInternal0Edges_ = -1;
  nInternal1Edges_ = -1;
  nInternalEdges_ = -1;
  nInternalFaces_ = nInternalFaces;
  nFaces_ = nFaces;
  nCells_ = nCells;
  // Check if points are ordered
  label nInternalPoints;
  labelList pointMap;
  bool isOrdered = calcPointOrder
  (
    nInternalPoints,
    pointMap,
    faces(),
    nInternalFaces_,
    nPoints_
  );
  if (isOrdered)
  {
    nInternalPoints_ = nInternalPoints;
  }
  else
  {
    nInternalPoints_ = -1;
  }
  if (debug)
  {
    Pout<< "primitiveMesh::reset : mesh reset to"
      << " nInternalPoints:" << nInternalPoints_
      << " nPoints:" << nPoints_
      << " nEdges:" << nEdges_
      << " nInternalFaces:" << nInternalFaces_
      << " nFaces:" << nFaces_
      << " nCells:" << nCells_
      << endl;
  }
}
void mousse::primitiveMesh::reset
(
  const label nPoints,
  const label nInternalFaces,
  const label nFaces,
  const label nCells,
  cellList& clst
)
{
  reset
  (
    nPoints,
    nInternalFaces,
    nFaces,
    nCells
  );
  cfPtr_ = new cellList(clst, true);
}
void mousse::primitiveMesh::reset
(
  const label nPoints,
  const label nInternalFaces,
  const label nFaces,
  const label nCells,
  const Xfer<cellList>& clst
)
{
  reset
  (
    nPoints,
    nInternalFaces,
    nFaces,
    nCells
  );
  cfPtr_ = new cellList(clst);
}
mousse::tmp<mousse::scalarField> mousse::primitiveMesh::movePoints
(
  const pointField& newPoints,
  const pointField& oldPoints
)
{
  if (newPoints.size() <  nPoints() || oldPoints.size() < nPoints())
  {
    FATAL_ERROR_IN
    (
      "primitiveMesh::movePoints(const pointField& newPoints, "
      "const pointField& oldPoints)"
    )   << "Cannot move points: size of given point list smaller "
      << "than the number of active points"
      << abort(FatalError);
  }
  // Create swept volumes
  const faceList& f = faces();
  tmp<scalarField> tsweptVols(new scalarField(f.size()));
  scalarField& sweptVols = tsweptVols();
  FOR_ALL(f, faceI)
  {
    sweptVols[faceI] = f[faceI].sweptVol(oldPoints, newPoints);
  }
  // Force recalculation of all geometric data with new points
  clearGeom();
  return tsweptVols;
}
const mousse::cellShapeList& mousse::primitiveMesh::cellShapes() const
{
  if (!cellShapesPtr_)
  {
    calcCellShapes();
  }
  return *cellShapesPtr_;
}
