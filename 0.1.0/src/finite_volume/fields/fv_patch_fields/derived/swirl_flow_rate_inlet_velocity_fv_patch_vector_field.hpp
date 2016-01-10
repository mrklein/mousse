// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::swirlFlowRateInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition provides a volumetric- OR mass-flow normal vector
//   boundary condition by its magnitude as an integral over its area with a
//   swirl component determined by the angular speed, given in revolutions per
//   minute (RPM)
//   The basis of the patch (volumetric or mass) is determined by the
//   dimensions of the flux, phi. The current density is used to correct the
//   velocity when applying the mass basis.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     flowRate     | flow rate profile       | yes         |
//     rpm          | rotational speed profile | yes        |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            swirlFlowRateInletVelocity;
//     flowRate        constant 0.2;
//     rpm             constant 100;
//   }
//   \endverbatim
// Note
//   - the \c flowRate and \c rpm entries are DataEntry types, able to describe
//    time varying functions.  The example above gives the usage for supplying
//    constant values.
//   - the value is positive into the domain
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   swirl_flow_rate_inlet_velocity_fv_patch_vector_field.cpp
#ifndef swirl_flow_rate_inlet_velocity_fv_patch_vector_field_hpp_
#define swirl_flow_rate_inlet_velocity_fv_patch_vector_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class swirlFlowRateInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of the flux transporting the field
    const word phiName_;
    //- Name of the density field used to normalize the mass flux
    const word rhoName_;
    //- Inlet integral flow rate
    autoPtr<DataEntry<scalar> > flowRate_;
    //- Angular speed in revolutions per minute (RPM)
    autoPtr<DataEntry<scalar> > rpm_;
public:
 //- Runtime type information
 TYPE_NAME("swirlFlowRateInletVelocity");
 // Constructors
    //- Construct from patch and internal field
    swirlFlowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    swirlFlowRateInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  flowRateInletVelocityFvPatchVectorField
    //  onto a new patch
    swirlFlowRateInletVelocityFvPatchVectorField
    (
      const swirlFlowRateInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    swirlFlowRateInletVelocityFvPatchVectorField
    (
      const swirlFlowRateInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new swirlFlowRateInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    swirlFlowRateInletVelocityFvPatchVectorField
    (
      const swirlFlowRateInletVelocityFvPatchVectorField&,
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
        new swirlFlowRateInletVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
