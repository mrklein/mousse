#ifndef THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_WIDE_BAND_DIFFUSIVE_RADIATION_MIXED_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_WIDE_BAND_DIFFUSIVE_RADIATION_MIXED_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::wideBandDiffusiveRadiationMixedFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a wide-band, diffusive radiation
//   condition, where the patch temperature is specified.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     T            | temperature field name  | no          | T
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            wideBandDiffusiveRadiation;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::mixedFvPatchScalarField
//   mousse::radiationCoupledBase

#include "mixed_fv_patch_fields.hpp"
#include "radiation_coupled_base.hpp"


namespace mousse {
namespace radiation {

class wideBandDiffusiveRadiationMixedFvPatchScalarField
:
  public mixedFvPatchScalarField,
  public radiationCoupledBase
{
  // Private data
    //- Name of temperature field
    word TName_;
public:
  //- Runtime type information
  TYPE_NAME("wideBandDiffusiveRadiation");
  // Constructors
    //- Construct from patch and internal field
    wideBandDiffusiveRadiationMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    wideBandDiffusiveRadiationMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given GreyDiffusiveRadiationMixedFvPatchField
    //  onto a new patch
    wideBandDiffusiveRadiationMixedFvPatchScalarField
    (
      const wideBandDiffusiveRadiationMixedFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    wideBandDiffusiveRadiationMixedFvPatchScalarField
    (
      const wideBandDiffusiveRadiationMixedFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new wideBandDiffusiveRadiationMixedFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    wideBandDiffusiveRadiationMixedFvPatchScalarField
    (
      const wideBandDiffusiveRadiationMixedFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new wideBandDiffusiveRadiationMixedFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Access
      //- Return the temperature field name
      const word& TName() const
      {
        return TName_;
      }
      //- Return reference to the temperature field name to allow
      //  adjustment
      word& TName()
      {
        return TName_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};

}  // namespace mousse
}

#endif

