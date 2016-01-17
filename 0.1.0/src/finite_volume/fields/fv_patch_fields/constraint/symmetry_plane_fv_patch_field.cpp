// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_plane_fv_patch_field.hpp"

// Constructors 
template<class Type>
mousse::symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF
)
:
  basicSymmetryFvPatchField<Type>{p, iF},
  symmetryPlanePatch_{refCast<const symmetryPlaneFvPatch>(p)}
{}


template<class Type>
mousse::symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField
(
  const symmetryPlaneFvPatchField<Type>& ptf,
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  basicSymmetryFvPatchField<Type>{ptf, p, iF, mapper},
  symmetryPlanePatch_{refCast<const symmetryPlaneFvPatch>(p)}
{
  if (!isType<symmetryPlaneFvPatch>(this->patch()))
  {
    FATAL_ERROR_IN
    (
      "symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField\n"
      "(\n"
      "    const symmetryPlaneFvPatchField<Type>& ptf,\n"
      "    const fvPatch& p,\n"
      "    const DimensionedField<Type, volMesh>& iF,\n"
      "    const fvPatchFieldMapper& mapper\n"
      ")\n"
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField
(
  const fvPatch& p,
  const DimensionedField<Type, volMesh>& iF,
  const dictionary& dict
)
:
  basicSymmetryFvPatchField<Type>{p, iF, dict},
  symmetryPlanePatch_{refCast<const symmetryPlaneFvPatch>(p)}
{
  if (!isType<symmetryPlaneFvPatch>(p))
  {
    FATAL_IO_ERROR_IN
    (
      "symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField\n"
      "(\n"
      "    const fvPatch& p,\n"
      "    const Field<Type>& field,\n"
      "    const dictionary& dict\n"
      ")\n",
      dict
    )
    << "\n    patch type '" << p.type()
    << "' not constraint type '" << typeName << "'"
    << "\n    for patch " << p.name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalIOError);
  }
}


template<class Type>
mousse::symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField
(
  const symmetryPlaneFvPatchField<Type>& ptf
)
:
  basicSymmetryFvPatchField<Type>{ptf},
  symmetryPlanePatch_{ptf.symmetryPlanePatch_}
{}


template<class Type>
mousse::symmetryPlaneFvPatchField<Type>::symmetryPlaneFvPatchField
(
  const symmetryPlaneFvPatchField<Type>& ptf,
  const DimensionedField<Type, volMesh>& iF
)
:
  basicSymmetryFvPatchField<Type>{ptf, iF},
  symmetryPlanePatch_{ptf.symmetryPlanePatch_}
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::symmetryPlaneFvPatchField<Type>::snGrad() const
{
  vector nHat(symmetryPlanePatch_.n());
  const Field<Type> iF(this->patchInternalField());
  return
    (transform(I - 2.0*sqr(nHat), iF) - iF)
   *(this->patch().deltaCoeffs()/2.0);
}


template<class Type>
void mousse::symmetryPlaneFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
  if (!this->updated())
  {
    this->updateCoeffs();
  }
  vector nHat(symmetryPlanePatch_.n());
  const Field<Type> iF(this->patchInternalField());
  Field<Type>::operator=
  (
    (iF + transform(I - 2.0*sqr(nHat), iF))/2.0
  );
  transformFvPatchField<Type>::evaluate();
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::symmetryPlaneFvPatchField<Type>::snGradTransformDiag() const
{
  vector nHat{symmetryPlanePatch_.n()};
  const vector diag
  {
    mag(nHat.component(vector::X)),
    mag(nHat.component(vector::Y)),
    mag(nHat.component(vector::Z))
  };
  return tmp<Field<Type> >
  {
    new Field<Type>
    {
      this->size(),
      transformMask<Type>
      (
        //pow<vector, pTraits<Type>::rank>(diag)
        pow
        (
          diag,
          pTraits<typename powProduct<vector, pTraits<Type>::rank>
          ::type>::zero
        )
      )
    }
  };
}
