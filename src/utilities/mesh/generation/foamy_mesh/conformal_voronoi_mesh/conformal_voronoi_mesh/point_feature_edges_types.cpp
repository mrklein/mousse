// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_feature_edges_types.hpp"


// Constructors 
mousse::pointFeatureEdgesTypes::pointFeatureEdgesTypes
(
  const extendedFeatureEdgeMesh& feMesh,
  const label pointLabel
)
:
  HashTable<label, extendedFeatureEdgeMesh::edgeStatus>{},
  feMesh_{feMesh},
  pointLabel_{pointLabel}
{}


// Destructor 
mousse::pointFeatureEdgesTypes::~pointFeatureEdgesTypes()
{}


// Member Functions 
mousse::List<mousse::extendedFeatureEdgeMesh::edgeStatus>
mousse::pointFeatureEdgesTypes::calcPointFeatureEdgesTypes()
{
  const labelList& pEds = feMesh_.pointEdges()[pointLabel_];
  List<extendedFeatureEdgeMesh::edgeStatus> allEdStat{pEds.size()};
  FOR_ALL(pEds, i) {
    label edgeI = pEds[i];
    extendedFeatureEdgeMesh::edgeStatus& eS = allEdStat[i];
    eS = feMesh_.getEdgeStatus(edgeI);
    this->operator()(eS)++;
  }
  return allEdStat;
}


// Friend Functions 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const pointFeatureEdgesTypes& p
)
{
  os << "Point = " << p.pointLabel_ << endl;
  for (auto iter = p.cbegin();
       iter != p.cend();
       ++iter) {
    os << "    "
      << extendedFeatureEdgeMesh::edgeStatusNames_[iter.key()]
      << " = "
      << iter()
      << endl;
  }
  return os;
}

