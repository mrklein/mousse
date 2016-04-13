#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRESSURE_INLET_OUTLET_PAR_SLIP_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRESSURE_INLET_OUTLET_PAR_SLIP_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureInletOutletParSlipVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This velocity inlet/outlet boundary condition for pressure boundary where
//   the pressure is specified.  A zero-gradient is applied for outflow (as
//   defined by the flux); for inflow, the velocity is obtained from the flux
//   with the specified inlet direction.
//   A slip condition is applied tangential to the patch.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            pressureInletOutletParSlipVelocity;
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   Sign conventions:
//   - positive flux (out of domain): apply zero-gradient condition
//   - negative flux (into of domain): derive from the flux with specified
//    direction
// SeeAlso
//   mousse::mixedFvPatchVectorField
//   mousse::pressureDirectedInletOutletVelocityFvPatchVectorField

#include "fv_patch_fields.hpp"
#include "mixed_fv_patch_fields.hpp"


namespace mousse {

class pressureInletOutletParSlipVelocityFvPatchVectorField
:
  public mixedFvPatchVectorField
{
  // Private data
    //- Flux field name
    word phiName_;
    //- Density field name
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("pressureInletOutletParSlipVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureInletOutletParSlipVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct by mapping given
    //  pressureInletOutletParSlipVelocityFvPatchVectorField
    //  onto a new patch
    pressureInletOutletParSlipVelocityFvPatchVectorField
    (
      const pressureInletOutletParSlipVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct from patch, internal field and dictionary
    pressureInletOutletParSlipVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct as copy
    pressureInletOutletParSlipVelocityFvPatchVectorField
    (
      const pressureInletOutletParSlipVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      {
        new pressureInletOutletParSlipVelocityFvPatchVectorField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    pressureInletOutletParSlipVelocityFvPatchVectorField
    (
      const pressureInletOutletParSlipVelocityFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      {
        new pressureInletOutletParSlipVelocityFvPatchVectorField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the name of rho
      const word& rhoName() const
      {
        return rhoName_;
      }
      //- Return reference to the name of rho to allow adjustment
      word& rhoName()
      {
        return rhoName_;
      }
      //- Return the name of phi
      const word& phiName() const
      {
        return phiName_;
      }
      //- Return reference to the name of phi to allow adjustment
      word& phiName()
      {
        return phiName_;
      }
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const fvPatchField<vector>& pvf);
};
}  // namespace mousse
#endif
