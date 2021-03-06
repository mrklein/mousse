#ifndef THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_GREY_DIFFUSIVE_VIEW_FACTOR_FIXED_VALUE_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_GREY_DIFFUSIVE_VIEW_FACTOR_FIXED_VALUE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::greyDiffusiveViewFactorFixedValueFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a grey-diffuse condition for radiative
//   heat flux, \c Qr, for use with the view factor model
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Qro          | external radiative heat flux | yes    |
//     emissivityMode | emissivity mode: solidRadiation or lookup | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            greyDiffusiveRadiationViewFactor;
//     Qro             uniform 0;
//     emissivityMode  solidRadiation;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::radiationCoupledBase
//   mousse::radiation::radiationModel
//   mousse::radiation::viewFactor
//   mousse::fixedValueFvPatchField

#include "radiation_coupled_base.hpp"
#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {
namespace radiation {

class greyDiffusiveViewFactorFixedValueFvPatchScalarField
:
  public fixedValueFvPatchScalarField,
  public radiationCoupledBase
{
  // Private data
    //- External radiative heat flux
    scalarField Qro_;
public:
  //- Runtime type information
  TYPE_NAME("greyDiffusiveRadiationViewFactor");
  // Constructors
    //- Construct from patch and internal field
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given a
    // greyDiffusiveViewFactorFixedValueFvPatchScalarField onto a new patch
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
      const greyDiffusiveViewFactorFixedValueFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
      const greyDiffusiveViewFactorFixedValueFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new greyDiffusiveViewFactorFixedValueFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    greyDiffusiveViewFactorFixedValueFvPatchScalarField
    (
      const greyDiffusiveViewFactorFixedValueFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      {
        new greyDiffusiveViewFactorFixedValueFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return external radiative heat flux
      const scalarList& Qro()
      {
        return Qro_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};

}  // namespace radiation
}  // namespace mousse

#endif

