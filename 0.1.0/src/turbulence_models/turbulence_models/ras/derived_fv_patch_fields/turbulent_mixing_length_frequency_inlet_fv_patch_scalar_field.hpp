// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentMixingLengthFrequencyInletFvPatchScalarField
// Group
//   grpRASBoundaryConditions grpInletBoundaryConditions
// Description
//   This boundary condition provides a turbulence specific dissipation,
//   \f$\omega\f$ (omega) inlet condition based on a specified mixing length.
//   The patch values are calculated using:
//     \f[
//       \omega_p = \frac{k^{0.5}}{C_{\mu}^{0.25} L}
//     \f]
//   where
//   \vartable
//     \omega_p | patch omega values
//     C_{\mu} | Model coefficient, set to 0.09
//     k       | turbulence kinetic energy
//     L       | length scale
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     mixingLength | Length scale [m]        | yes         |
//     phi          | flux field name         | no          | phi
//     k            | turbulence kinetic energy field name | no | k
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            turbulentMixingLengthFrequencyInlet;
//     mixingLength    0.005;
//     value           uniform 200;   // placeholder
//   }
//   \endverbatim
// Note
//   In the event of reverse flow, a zero-gradient condition is applied
// SeeAlso
//   mousse::inletOutletFvPatchField
// SourceFiles
//   turbulent_mixing_length_frequency_inlet_fv_patch_scalar_field.cpp
#ifndef turbulent_mixing_length_frequency_inlet_fv_patch_scalar_field_hpp_
#define turbulent_mixing_length_frequency_inlet_fv_patch_scalar_field_hpp_
#include "inlet_outlet_fv_patch_fields.hpp"
namespace mousse
{
class turbulentMixingLengthFrequencyInletFvPatchScalarField
:
  public inletOutletFvPatchScalarField
{
  // Private data
    //- Turbulent length scale
    scalar mixingLength_;
    //- Name of the turbulent kinetic energy field
    word kName_;
public:
  //- Runtime type information
  TYPE_NAME("turbulentMixingLengthFrequencyInlet");
  // Constructors
    //- Construct from patch and internal field
    turbulentMixingLengthFrequencyInletFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentMixingLengthFrequencyInletFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  turbulentMixingLengthFrequencyInletFvPatchScalarField
    //  onto a new patch
    turbulentMixingLengthFrequencyInletFvPatchScalarField
    (
      const turbulentMixingLengthFrequencyInletFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    turbulentMixingLengthFrequencyInletFvPatchScalarField
    (
      const turbulentMixingLengthFrequencyInletFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    turbulentMixingLengthFrequencyInletFvPatchScalarField
    (
      const turbulentMixingLengthFrequencyInletFvPatchScalarField&,
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
        new turbulentMixingLengthFrequencyInletFvPatchScalarField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
