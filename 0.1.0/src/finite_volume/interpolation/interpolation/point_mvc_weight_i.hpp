// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_fields.hpp"
// Member Functions 
template<class Type>
inline Type mousse::pointMVCWeight::interpolate
(
  const GeometricField<Type, pointPatchField, pointMesh>& psip
) const
{
  const labelList& vertices = psip.mesh()().cellPoints()[cellIndex_];
  Type t = pTraits<Type>::zero;
  forAll(vertices, i)
  {
    t += psip[vertices[i]]*weights_[i];
  }
  return t;
}
