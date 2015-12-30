// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_jump_fv_patch_field.hpp"
// Constructors 
template<class Type>
mousse::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  jumpCyclicFvPatchField<Type>(p, iF),
  jump_(this->size(), pTraits<Type>::zero)
{}
template<class Type>
mousse::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
  const fixedJumpFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  jumpCyclicFvPatchField<Type>(ptf, p, iF, mapper),
  jump_(ptf.jump_, mapper)
{}
template<class Type>
mousse::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  jumpCyclicFvPatchField<Type>(p, iF),
  jump_(p.size(), pTraits<Type>::zero)
{
  if (this->cyclicPatch().owner())
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
mousse::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
  const fixedJumpFvPatchField<Type>& ptf
)
:
  jumpCyclicFvPatchField<Type>(ptf),
  jump_(ptf.jump_)
{}
template<class Type>
mousse::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
  const fixedJumpFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  jumpCyclicFvPatchField<Type>(ptf, iF),
  jump_(ptf.jump_)
{}
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::fixedJumpFvPatchField<Type>::jump() const
{
  if (this->cyclicPatch().owner())
  {
    return jump_;
  }
  else
  {
    return refCast<const fixedJumpFvPatchField<Type> >
    (
      this->neighbourPatchField()
    ).jump();
  }
}
template<class Type>
void mousse::fixedJumpFvPatchField<Type>::autoMap
(
  const fvPatchFieldMapper& m
)
{
  jumpCyclicFvPatchField<Type>::autoMap(m);
  jump_.autoMap(m);
}
template<class Type>
void mousse::fixedJumpFvPatchField<Type>::rmap
(
  const fvPatchField<Type>& ptf,
  const labelList& addr
)
{
  jumpCyclicFvPatchField<Type>::rmap(ptf, addr);
  const fixedJumpFvPatchField<Type>& tiptf =
    refCast<const fixedJumpFvPatchField<Type> >(ptf);
  jump_.rmap(tiptf.jump_, addr);
}
template<class Type>
void mousse::fixedJumpFvPatchField<Type>::write(Ostream& os) const
{
  fvPatchField<Type>::write(os);
  os.writeKeyword("patchType") << this->interfaceFieldType()
    << token::END_STATEMENT << nl;
  if (this->cyclicPatch().owner())
  {
    jump_.writeEntry("jump", os);
  }
  this->writeEntry("value", os);
}
