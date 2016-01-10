// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_mesh.hpp"
// Member Functions 
template<class T, class BinaryOp>
void mousse::lduMesh::reduce
(
  T& Value,
  const BinaryOp& bop
) const
{
  mousse::reduce(Value, bop, Pstream::msgType(), comm());
}
