// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interstitialInletVelocityFvPatchVectorField
// Description
//   Inlet velocity in which the actual interstitial velocity is calculated
//   by dividing the specified inletVelocity field with the local phase-fraction.
//   Example of the boundary condition specification:
//   \verbatim
//   inlet
//   {
//     type              interstitialInletVelocity;
//     inletVelocity     uniform (0 0.2 0);// Non-interstitial inlet velocity
//     alpha             alpha.particles;  // Name of the phase-fraction field
//     value             uniform (0 0 0);
//   }
//   \endverbatim
// SourceFiles
//   interstitial_inlet_velocity_fv_patch_vector_field.cpp
#ifndef interstitial_inlet_velocity_fv_patch_vector_field_hpp_
#define interstitial_inlet_velocity_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class interstitialInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Inlet velocity
    vectorField inletVelocity_;
    //- Name of the flux transporting the field
    word alphaName_;
public:
 //- Runtime type information
 TYPE_NAME("interstitialInletVelocity");
 // Constructors
    //- Construct from patch and internal field
    interstitialInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    interstitialInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  interstitialInletVelocityFvPatchVectorField
    //  onto a new patch
    interstitialInletVelocityFvPatchVectorField
    (
      const interstitialInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    interstitialInletVelocityFvPatchVectorField
    (
      const interstitialInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new interstitialInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    interstitialInletVelocityFvPatchVectorField
    (
      const interstitialInletVelocityFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new interstitialInletVelocityFvPatchVectorField(*this, iF)
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
        const fvPatchVectorField&,
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
