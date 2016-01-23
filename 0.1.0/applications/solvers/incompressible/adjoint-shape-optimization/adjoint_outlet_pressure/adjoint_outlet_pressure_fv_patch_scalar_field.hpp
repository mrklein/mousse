// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::adjointOutletPressureFvPatchScalarField
// Description
// SourceFiles
//   adjoint_outlet_pressure_fv_patch_scalar_field.cpp
#ifndef adjoint_outlet_pressure_fv_patch_scalar_field_hpp_
#define adjoint_outlet_pressure_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class adjointOutletPressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("adjointOutletPressure");
  // Constructors
    //- Construct from patch and internal field
    adjointOutletPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    adjointOutletPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given adjointOutletPressureFvPatchScalarField
    //  onto a new patch
    adjointOutletPressureFvPatchScalarField
    (
      const adjointOutletPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new adjointOutletPressureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    adjointOutletPressureFvPatchScalarField
    (
      const adjointOutletPressureFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      (
        new adjointOutletPressureFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
