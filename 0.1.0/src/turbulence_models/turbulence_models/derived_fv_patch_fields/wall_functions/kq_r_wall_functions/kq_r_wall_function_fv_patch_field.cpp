// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kq_r_wall_function_fv_patch_field.hpp"
#include "fv_patch_field_mapper.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "wall_fv_patch.hpp"
namespace mousse
{
// Private Member Functions 
template<class Type>
void kqRWallFunctionFvPatchField<Type>::checkType()
{
  if (!isA<wallFvPatch>(this->patch()))
  {
    FATAL_ERROR_IN("kqRWallFunctionFvPatchField::checkType()")
      << "Invalid wall function specification" << nl
      << "    Patch type for patch " << this->patch().name()
      << " must be wall" << nl
      << "    Current patch type is " << this->patch().type()
      << nl << endl
      << abort(FatalError);
  }
}
// Constructors 
template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  zeroGradientFvPatchField<Type>(p, iF)
{
  checkType();
}
template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
  const kqRWallFunctionFvPatchField& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  zeroGradientFvPatchField<Type>(ptf, p, iF, mapper)
{
  checkType();
}
template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  zeroGradientFvPatchField<Type>(p, iF, dict)
{
  checkType();
}
template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
  const kqRWallFunctionFvPatchField& tkqrwfpf
)
:
  zeroGradientFvPatchField<Type>(tkqrwfpf)
{
  checkType();
}
template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
  const kqRWallFunctionFvPatchField& tkqrwfpf,
  const DimensionedField<Type, volMesh>& iF
)
:
  zeroGradientFvPatchField<Type>(tkqrwfpf, iF)
{
  checkType();
}
// Member Functions 
template<class Type>
void kqRWallFunctionFvPatchField<Type>::evaluate
(
  const Pstream::commsTypes commsType
)
{
  zeroGradientFvPatchField<Type>::evaluate(commsType);
}
template<class Type>
void kqRWallFunctionFvPatchField<Type>::write(Ostream& os) const
{
  zeroGradientFvPatchField<Type>::write(os);
  this->writeEntry("value", os);
}
}  // namespace mousse
