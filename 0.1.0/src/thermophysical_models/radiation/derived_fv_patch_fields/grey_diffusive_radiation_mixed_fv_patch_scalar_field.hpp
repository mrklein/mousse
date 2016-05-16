#ifndef THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_GREY_DIFFUSIVE_RADIATION_MIXED_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_GREY_DIFFUSIVE_RADIATION_MIXED_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::greyDiffusiveRadiationMixedFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a grey-diffuse condition for radiation
//   intensity, \c I, for use with the finite-volume discrete-ordinates model
//   (fvDOM), in which the radiation temperature is retrieved from the
//   temperature field boundary condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     T            | temperature field name  | no          | T
//     emissivityMode | emissivity mode: solidRadiation or lookup | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            greyDiffusiveRadiation;
//     T               T;
//     emissivityMode  solidRadiation;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::radiationCoupledBase
//   mousse::radiation::radiationModel
//   mousse::radiation::fvDOM
//   mousse::mixedFvPatchField

#include "mixed_fv_patch_fields.hpp"
#include "radiation_coupled_base.hpp"


namespace mousse {
namespace radiation {

class greyDiffusiveRadiationMixedFvPatchScalarField
:
  public mixedFvPatchScalarField,
  public radiationCoupledBase
{
  // Private data
    //- Name of temperature field
    word TName_;
public:
  //- Runtime type information
  TYPE_NAME("greyDiffusiveRadiation");
  // Constructors
    //- Construct from patch and internal field
    greyDiffusiveRadiationMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    greyDiffusiveRadiationMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given a
    //  greyDiffusiveRadiationMixedFvPatchScalarField onto a new patch
    greyDiffusiveRadiationMixedFvPatchScalarField
    (
      const greyDiffusiveRadiationMixedFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    greyDiffusiveRadiationMixedFvPatchScalarField
    (
      const greyDiffusiveRadiationMixedFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new greyDiffusiveRadiationMixedFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    greyDiffusiveRadiationMixedFvPatchScalarField
    (
      const greyDiffusiveRadiationMixedFvPatchScalarField&,
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
        new greyDiffusiveRadiationMixedFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the temperature field name
      const word& TName() const { return TName_; }
      //- Return reference to the temperature field name to allow adjustment
      word& TName() { return TName_; }
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

