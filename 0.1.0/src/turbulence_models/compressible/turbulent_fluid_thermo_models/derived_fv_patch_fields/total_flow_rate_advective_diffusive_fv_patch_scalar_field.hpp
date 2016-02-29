#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_TOTAL_FLOW_RATE_ADVECTIVE_DIFFUSIVE_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_TOTAL_FLOW_RATE_ADVECTIVE_DIFFUSIVE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::totalFlowRateAdvectiveDiffusiveFvPatchScalarField
// Description
//   This BC is used for species inlets. The diffusion and advection fluxes are
//   considered to calculate the inlet value for the species
//   The massFluxFraction sets the fraction of the flux of each particular
//   species.
// SourceFiles
//   total_flow_rate_advective_diffusive_fv_patch_scalar_field.cpp
#include "mixed_fv_patch_field.hpp"
namespace mousse
{
class totalFlowRateAdvectiveDiffusiveFvPatchScalarField
:
  public mixedFvPatchField<scalar>
{
  // Private data
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of the density field used to normalise the mass flux
    //  if neccessary
    word rhoName_;
    //- Mass flux fraction
    scalar massFluxFraction_;
public:
  //- Runtime type information
  TYPE_NAME("totalFlowRateAdvectiveDiffusive");
  // Constructors
    //- Construct from patch and internal field
    totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    //  onto a new patch
    totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    (
      const totalFlowRateAdvectiveDiffusiveFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    (
      const totalFlowRateAdvectiveDiffusiveFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar> > clone() const
    {
      return tmp<fvPatchField<scalar> >
      (
        new
        totalFlowRateAdvectiveDiffusiveFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    totalFlowRateAdvectiveDiffusiveFvPatchScalarField
    (
      const totalFlowRateAdvectiveDiffusiveFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar> > clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<scalar> >
      (
        new
        totalFlowRateAdvectiveDiffusiveFvPatchScalarField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
