#ifndef THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_MARSHAK_RADIATION_FIXED_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_DERIVED_FV_PATCH_FIELDS_MARSHAK_RADIATION_FIXED_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MarshakRadiationFixedTemperatureFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   A 'mixed' boundary condition that implements a Marshak condition for the
//   incident radiation field (usually written as G)
//   The radiation temperature field across the patch is supplied by the user
//   using the \c Trad entry.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     T            | temperature field name  | no          | T
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            MarshakRadiationFixedTemperature;
//     Trad            uniform 1000;       // radiation temperature field
//     value           uniform 0;          // place holder
//   }
//   \endverbatim
// SeeAlso
//   mousse::radiationCoupledBase
//   mousse::mixedFvPatchField

#include "mixed_fv_patch_fields.hpp"
#include "radiation_coupled_base.hpp"


namespace mousse {

class MarshakRadiationFixedTemperatureFvPatchScalarField
:
  public mixedFvPatchScalarField,
  public radiationCoupledBase
{
  // Private data
    //- Radiation temperature field
    scalarField Trad_;
public:
  //- Runtime type information
  TYPE_NAME("MarshakRadiationFixedTemperature");
  // Constructors
    //- Construct from patch and internal field
    MarshakRadiationFixedTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    MarshakRadiationFixedTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given MarshakRadiationFvPatchField onto a new
    //  patch
    MarshakRadiationFixedTemperatureFvPatchScalarField
    (
      const MarshakRadiationFixedTemperatureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    MarshakRadiationFixedTemperatureFvPatchScalarField
    (
      const MarshakRadiationFixedTemperatureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new MarshakRadiationFixedTemperatureFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    MarshakRadiationFixedTemperatureFvPatchScalarField
    (
      const MarshakRadiationFixedTemperatureFvPatchScalarField&,
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
        new MarshakRadiationFixedTemperatureFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the radiation temperature
      const scalarField& Trad() const { return Trad_; }
      //- Return reference to the radiation temperature to allow adjustment
      scalarField& Trad() { return Trad_; }
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
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

