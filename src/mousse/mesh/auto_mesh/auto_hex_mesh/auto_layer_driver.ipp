// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "auto_layer_driver.hpp"
#include "sync_tools.hpp"


// Private Member Functions 
template<class Type>
void mousse::autoLayerDriver::averageNeighbours
(
  const polyMesh& mesh,
  const PackedBoolList& isMasterEdge,
  const labelList& meshEdges,
  const labelList& meshPoints,
  const edgeList& edges,
  const scalarField& invSumWeight,
  const Field<Type>& data,
  Field<Type>& average
)
{
  const pointField& pts = mesh.points();
  average = pTraits<Type>::zero;
  FOR_ALL(edges, edgeI) {
    if (isMasterEdge.get(meshEdges[edgeI]) == 1) {
      const edge& e = edges[edgeI];
      //scalar eWeight = edgeWeights[edgeI];
      //scalar eWeight =  1.0;
      scalar eMag =
        max(VSMALL, mag(pts[meshPoints[e[1]]] - pts[meshPoints[e[0]]]));
      scalar eWeight = 1.0/eMag;
      label v0 = e[0];
      label v1 = e[1];
      average[v0] += eWeight*data[v1];
      average[v1] += eWeight*data[v0];
    }
  }
  syncTools::syncPointList
  (
    mesh,
    meshPoints,
    average,
    plusEqOp<Type>(),
    pTraits<Type>::zero     // null value
  );
  average *= invSumWeight;
}

