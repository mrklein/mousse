// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::outletPhaseMeanVelocityFvPatchVectorField
// Group
//   grpOutletBoundaryConditions
// Description
//   This boundary condition adjusts the velocity for the given phase to achieve
//   the specified mean thus causing the phase-fraction to adjust according to
//   the mass flow rate.
//   Typical usage is as the outlet condition for a towing-tank ship simulation
//   to maintain the outlet water level at the level as the inlet.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Umean        | mean velocity normal to the boundary [m/s] | yes |
//     alpha        | phase-fraction field    | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            outletPhaseMeanVelocity;
//     Umean           1.2;
//     alpha           alpha.water;
//     value           uniform (1.2 0 0);
//   }
//   \endverbatim
// SeeAlso
//   mousse::mixedFvPatchField
//   mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
// SourceFiles
//   outlet_phase_mean_velocity_fv_patch_vector_field.cpp
#ifndef outlet_phase_mean_velocity_fv_patch_vector_field_hpp_
#define outlet_phase_mean_velocity_fv_patch_vector_field_hpp_
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class outletPhaseMeanVelocityFvPatchVectorField
:
  public mixedFvPatchVectorField
{
  // Private data
    //- Inlet integral flow rate
    scalar Umean_;
    //- Name of the phase-fraction field
    word alphaName_;
public:
 //- Runtime type information
 TYPE_NAME("outletPhaseMeanVelocity");
 // Constructors
    //- Construct from patch and internal field
    outletPhaseMeanVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    outletPhaseMeanVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  outletPhaseMeanVelocityFvPatchVectorField
    //  onto a new patch
    outletPhaseMeanVelocityFvPatchVectorField
    (
      const outletPhaseMeanVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    outletPhaseMeanVelocityFvPatchVectorField
    (
      const outletPhaseMeanVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new outletPhaseMeanVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    outletPhaseMeanVelocityFvPatchVectorField
    (
      const outletPhaseMeanVelocityFvPatchVectorField&,
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
        new outletPhaseMeanVelocityFvPatchVectorField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Access
      //- Return the flux
      scalar Umean() const
      {
        return Umean_;
      }
      //- Return reference to the flux to allow adjustment
      scalar& Umean()
      {
        return Umean_;
      }
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
