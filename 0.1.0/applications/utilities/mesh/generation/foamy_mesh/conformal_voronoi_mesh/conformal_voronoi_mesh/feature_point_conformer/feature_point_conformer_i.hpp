// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

const mousse::List<Vb>& mousse::featurePointConformer::featurePointVertices() const
{
  return featurePointVertices_;
}
const mousse::pointPairs<Delaunay>&
mousse::featurePointConformer::featurePointPairs() const
{
  return ftPtPairs_;
}
