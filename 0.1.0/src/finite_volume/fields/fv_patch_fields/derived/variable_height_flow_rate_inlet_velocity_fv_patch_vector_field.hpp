#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_VARIABLE_HEIGHT_FLOW_RATE_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_VARIABLE_HEIGHT_FLOW_RATE_INLET_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::variableHeightFlowRateInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition provides a velocity boundary condition for
//   multphase flow based on a user-specified volumetric flow rate.
//   The flow rate is made proportional to the phase fraction alpha at each
//   face of the patch and alpha is ensured to be bound between 0 and 1.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     flowRate     | volumetric flow rate [m3/s] | yes |
//     alpha        | phase-fraction field    | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            variableHeightFlowRateInletVelocity;
//     flowRate        0.2;
//     alpha           alpha.water;
//     value           uniform (0 0 0); // placeholder
//   }
//   \endverbatim
// Note
//   - the value is positive into the domain
//   - may not work correctly for transonic inlets
//   - strange behaviour with mousse-potential since the momentum equation is
//     not solved
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   variable_height_flow_rate_inlet_velocity_fv_patch_vector_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class variableHeightFlowRateInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Inlet integral flow rate
    scalar flowRate_;
    //- Name of the phase-fraction field
    word alphaName_;
public:
 //- Runtime type information
 TYPE_NAME("variableHeightFlowRateInletVelocity");
 // Constructors
    //- Construct from patch and internal field
    variableHeightFlowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    variableHeightFlowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  variableHeightFlowRateInletVelocityFvPatchVectorField
    //  onto a new patch
    variableHeightFlowRateInletVelocityFvPatchVectorField
    (
      const variableHeightFlowRateInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    variableHeightFlowRateInletVelocityFvPatchVectorField
    (
      const variableHeightFlowRateInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new variableHeightFlowRateInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    variableHeightFlowRateInletVelocityFvPatchVectorField
    (
      const variableHeightFlowRateInletVelocityFvPatchVectorField&,
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
        new variableHeightFlowRateInletVelocityFvPatchVectorField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Access
      //- Return the flux
      scalar flowRate() const
      {
        return flowRate_;
      }
      //- Return reference to the flux to allow adjustment
      scalar& flowRate()
      {
        return flowRate_;
      }
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
