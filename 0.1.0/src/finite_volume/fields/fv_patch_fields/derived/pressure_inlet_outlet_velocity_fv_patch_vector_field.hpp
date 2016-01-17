// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureInletOutletVelocityFvPatchVectorField
// Group
//   grpInletletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This velocity inlet/outlet boundary condition is applied to pressure
//   boundaries where the pressure is specified.  A zero-gradient condition is
//   applied for outflow (as defined by the flux); for inflow, the velocity is
//   obtained from the patch-face normal component of the internal-cell value.
//   The tangential patch velocity can be optionally specified.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     tangentialVelocity | tangential velocity field | no  |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            pressureInletOutletVelocity;
//     phi             phi;
//     tangentialVelocity uniform (0 0 0);
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   Sign conventions:
//   - positive flux (out of domain): apply zero-gradient condition
//   - negative flux (into of domain): derive from the flux in the patch-normal
//    direction
// SourceFiles
//   pressure_inlet_outlet_velocity_fv_patch_vector_field.cpp
#ifndef pressure_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#define pressure_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "direction_mixed_fv_patch_fields.hpp"
namespace mousse
{
class pressureInletOutletVelocityFvPatchVectorField
:
  public directionMixedFvPatchVectorField
{
  // Private data
    //- Flux field name
    word phiName_;
    //- Optional tangential velocity component
    vectorField tangentialVelocity_;
public:
  //- Runtime type information
  TYPE_NAME("pressureInletOutletVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    pressureInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  pressureInletOutletVelocityFvPatchVectorField onto a new patch
    pressureInletOutletVelocityFvPatchVectorField
    (
      const pressureInletOutletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    pressureInletOutletVelocityFvPatchVectorField
    (
      const pressureInletOutletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new pressureInletOutletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    pressureInletOutletVelocityFvPatchVectorField
    (
      const pressureInletOutletVelocityFvPatchVectorField&,
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
        new pressureInletOutletVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access
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
      //- Return the tangential velocity
      const vectorField& tangentialVelocity() const
      {
        return tangentialVelocity_;
      }
      //- Reset the tangential velocity
      void setTangentialVelocity(const vectorField& tangentialVelocity);
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
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const fvPatchField<vector>& pvf);
};
}  // namespace mousse
#endif
