// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coupled_fv_patch_field.hpp"
// Member Functions 
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(p)),
  fvPatchField<Type>(p, iF)
{}
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const Field<Type>& f
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(p)),
  fvPatchField<Type>(p, iF, f)
{}
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const coupledFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(p)),
  fvPatchField<Type>(ptf, p, iF, mapper)
{}
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(p)),
  fvPatchField<Type>(p, iF, dict)
{}
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const coupledFvPatchField<Type>& ptf
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(ptf.patch())),
  fvPatchField<Type>(ptf)
{}
template<class Type>
mousse::coupledFvPatchField<Type>::coupledFvPatchField
(
  const coupledFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  LduInterfaceField<Type>(refCast<const lduInterface>(ptf.patch())),
  fvPatchField<Type>(ptf, iF)
{}
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::coupledFvPatchField<Type>::snGrad
(
  const scalarField& deltaCoeffs
) const
{
  return
    deltaCoeffs
   *(this->patchNeighbourField() - this->patchInternalField());
}
template<class Type>
void mousse::coupledFvPatchField<Type>::initEvaluate(const Pstream::commsTypes)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
}
template<class Type>
void mousse::coupledFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
  Field<Type>::operator=
  (
    this->patch().weights()*this->patchInternalField()
   + (1.0 - this->patch().weights())*this->patchNeighbourField()
  );
  fvPatchField<Type>::evaluate();
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::valueInternalCoeffs
(
  const tmp<scalarField>& w
) const
{
  return Type(pTraits<Type>::one)*w;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::valueBoundaryCoeffs
(
  const tmp<scalarField>& w
) const
{
  return Type(pTraits<Type>::one)*(1.0 - w);
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::gradientInternalCoeffs
(
  const scalarField& deltaCoeffs
) const
{
  return -Type(pTraits<Type>::one)*deltaCoeffs;
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::gradientInternalCoeffs() const
{
  notImplemented("coupledFvPatchField<Type>::gradientInternalCoeffs()");
  return -Type(pTraits<Type>::one)*this->patch().deltaCoeffs();
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::gradientBoundaryCoeffs
(
  const scalarField& deltaCoeffs
) const
{
  return -this->gradientInternalCoeffs(deltaCoeffs);
}
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::coupledFvPatchField<Type>::gradientBoundaryCoeffs() const
{
  notImplemented("coupledFvPatchField<Type>::gradientBoundaryCoeffs()");
  return -this->gradientInternalCoeffs();
}
template<class Type>
void mousse::coupledFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}
