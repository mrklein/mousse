// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::fvPatch::patchInternalField
(
  const UList<Type>& f
) const
{
  tmp<Field<Type> > tpif(new Field<Type>(size()));
  Field<Type>& pif = tpif();
  const labelUList& faceCells = this->faceCells();
  FOR_ALL(pif, facei)
  {
    pif[facei] = f[faceCells[facei]];
  }
  return tpif;
}
template<class Type>
void mousse::fvPatch::patchInternalField
(
  const UList<Type>& f,
  Field<Type>& pif
) const
{
  pif.setSize(size());
  const labelUList& faceCells = this->faceCells();
  FOR_ALL(pif, facei)
  {
    pif[facei] = f[faceCells[facei]];
  }
}
template<class GeometricField, class Type>
const typename GeometricField::PatchFieldType& mousse::fvPatch::patchField
(
  const GeometricField& gf
) const
{
  return gf.boundaryField()[index()];
}
