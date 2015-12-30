// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cutting_plane.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cuttingPlane::sample
(
  const Field<Type>& fld
) const
{
  return tmp<Field<Type> >(new Field<Type>(fld, cutCells()));
}
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::cuttingPlane::sample
(
  const tmp<Field<Type> >& tfld
) const
{
  tmp<Field<Type> > tsf = sample(tfld());
  tfld.clear();
  return tsf;
}
