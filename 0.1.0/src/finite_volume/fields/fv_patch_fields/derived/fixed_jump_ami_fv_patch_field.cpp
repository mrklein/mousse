// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_jump_ami_fv_patch_field.hpp"
// Constructors 
template<class Type>
mousse::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  jumpCyclicAMIFvPatchField<Type>(p, iF),
  jump_(this->size(), pTraits<Type>::zero)
{}
template<class Type>
mousse::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
  const fixedJumpAMIFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  jumpCyclicAMIFvPatchField<Type>(ptf, p, iF, mapper),
  jump_(ptf.jump_, mapper)
{}
template<class Type>
mousse::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  jumpCyclicAMIFvPatchField<Type>(p, iF),
  jump_(p.size(), pTraits<Type>::zero)
{
  if (this->cyclicAMIPatch().owner())
  {
    jump_ = Field<Type>("jump", dict, p.size());
  }
  if (dict.found("value"))
  {
    fvPatchField<Type>::operator=
    (
      Field<Type>("value", dict, p.size())
    );
  }
  else
  {
    this->evaluate(Pstream::blocking);
  }
}
template<class Type>
mousse::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
  const fixedJumpAMIFvPatchField<Type>& ptf
)
:
  jumpCyclicAMIFvPatchField<Type>(ptf),
  jump_(ptf.jump_)
{}
template<class Type>
mousse::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
  const fixedJumpAMIFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  jumpCyclicAMIFvPatchField<Type>(ptf, iF),
  jump_(ptf.jump_)
{}
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::fixedJumpAMIFvPatchField<Type>::jump() const
{
  if (this->cyclicAMIPatch().owner())
  {
    return jump_;
  }
  else
  {
    const fixedJumpAMIFvPatchField& nbrPatch =
      refCast<const fixedJumpAMIFvPatchField<Type> >
      (
        this->neighbourPatchField()
      );
    if (this->cyclicAMIPatch().applyLowWeightCorrection())
    {
      return this->cyclicAMIPatch().interpolate
      (
        nbrPatch.jump(),
        Field<Type>(this->size(), pTraits<Type>::zero)
      );
    }
    else
    {
      return this->cyclicAMIPatch().interpolate(nbrPatch.jump());
    }
  }
}
template<class Type>
void mousse::fixedJumpAMIFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  jumpCyclicAMIFvPatchField<Type>::autoMap(m);
  jump_.autoMap(m);
}
template<class Type>
void mousse::fixedJumpAMIFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  jumpCyclicAMIFvPatchField<Type>::rmap(ptf, addr);
  const fixedJumpAMIFvPatchField<Type>& tiptf =
    refCast<const fixedJumpAMIFvPatchField<Type> >(ptf);
  jump_.rmap(tiptf.jump_, addr);
}
template<class Type>
void mousse::fixedJumpAMIFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  os.writeKeyword("patchType") << this->interfaceFieldType()
    << token::END_STATEMENT << nl;
  if (this->cyclicAMIPatch().owner())
  {
    jump_.writeEntry("jump", os);
  }
  this->writeEntry("value", os);
}
