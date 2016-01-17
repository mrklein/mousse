// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::vectorTensorTransform::transform
(
  const Field<Type>& fld
) const
{
  if (hasR_)
  {
    return R() & fld;
  }
  else
  {
    return fld;
  }
}
