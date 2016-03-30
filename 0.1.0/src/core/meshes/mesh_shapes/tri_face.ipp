// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_face.hpp"


// Member Functions 
template<class Type>
Type mousse::triFace::average
(
  const pointField& /*meshPoints*/,
  const Field<Type>& fld
) const
{
  // a triangle, do a direct calculation
  return
    (fld[operator[](0)] + fld[operator[](1)] + fld[operator[](2)])/3.0;
}
