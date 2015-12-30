// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentMixingLengthDissipationRateInletFvPatchScalarField
// Group
//   grpRASBoundaryConditions grpInletBoundaryConditions
// Description
//   This boundary condition provides a turbulence dissipation, \f$\epsilon\f$
//   (epsilon) inlet condition based on a specified mixing length.  The patch
//   values are calculated using:
//     \f[
//       \epsilon_p = \frac{C_{\mu}^{0.75} k^{1.5}}{L}
//     \f]
//   where
//   \vartable
//     \epsilon_p | patch epsilon values
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
//     type            turbulentMixingLengthDissipationRateInlet;
//     mixingLength    0.005;
//     value           uniform 200;   // placeholder
//   }
//   \endverbatim
// Note
//   In the event of reverse flow, a zero-gradient condition is applied
// SeeAlso
//   mousse::inletOutletFvPatchField
// SourceFiles
//   turbulent_mixing_length_dissipation_rate_inlet_fv_patch_scalar_field.cpp
#ifndef turbulentMixingLengthDissipationRateInlet_H
#define turbulentMixingLengthDissipationRateInlet_H
#include "inlet_outlet_fv_patch_fields.hpp"
namespace mousse
{
class turbulentMixingLengthDissipationRateInletFvPatchScalarField
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
  TypeName("turbulentMixingLengthDissipationRateInlet");
  // Constructors
    //- Construct from patch and internal field
    turbulentMixingLengthDissipationRateInletFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentMixingLengthDissipationRateInletFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  turbulentMixingLengthDissipationRateInletFvPatchScalarField
    //  onto a new patch
    turbulentMixingLengthDissipationRateInletFvPatchScalarField
    (
      const turbulentMixingLengthDissipationRateInletFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    turbulentMixingLengthDissipationRateInletFvPatchScalarField
    (
      const turbulentMixingLengthDissipationRateInletFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new turbulentMixingLengthDissipationRateInletFvPatchScalarField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    turbulentMixingLengthDissipationRateInletFvPatchScalarField
    (
      const turbulentMixingLengthDissipationRateInletFvPatchScalarField&,
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
        new turbulentMixingLengthDissipationRateInletFvPatchScalarField
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
