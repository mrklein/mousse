// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedNormalInletOutletVelocityFvPatchVectorField
// Group
//   grpInletletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This velocity inlet/outlet boundary condition combines a fixed normal
//   component obtained from the "normalVelocity" patchField supplied with a
//   fixed or zero-gradiented tangential component depending on the direction
//   of the flow and the setting of "fixTangentialInflow":
//   - Outflow: apply zero-gradient condition to tangential components
//   - Inflow:
//    - fixTangentialInflow is true
//     apply value provided by the normalVelocity patchField to the
//     tangential components
//    - fixTangentialInflow is false
//     apply zero-gradient condition to tangential components.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//  fixTangentialInflow | If true fix the tangential component for inflow | yes |
//     normalVelocity | patchField providing the normal velocity field | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedNormalInletOutletVelocity;
//     fixTangentialInflow false;
//     normalVelocity
//     {
//       type            oscillatingFixedValue;
//       refValue        uniform (0 1 0);
//       offset          (0 -1 0);
//       amplitude       table
//       (
//         (   0     0)
//         (   2 0.088)
//         (   8 0.088)
//       );
//       frequency       constant 1;
//     }
//     value           uniform (0 0 0);
//   }
//   \endverbatim
// SourceFiles
//   fixed_normal_inlet_outlet_velocity_fv_patch_vector_field.cpp
#ifndef fixed_normal_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#define fixed_normal_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "direction_mixed_fv_patch_fields.hpp"
#include "switch.hpp"
namespace mousse
{
class fixedNormalInletOutletVelocityFvPatchVectorField
:
  public directionMixedFvPatchVectorField
{
  // Private data
    //- Flux field name
    word phiName_;
    //- Set true to fix the tangential component for inflow
    Switch fixTangentialInflow_;
    //- BC which provided the normal component of the velocity
    tmp<fvPatchVectorField> normalVelocity_;
public:
  //- Runtime type information
  TypeName("fixedNormalInletOutletVelocity");
  // Constructors
    //- Construct from patch and internal field
    fixedNormalInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedNormalInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  fixedNormalInletOutletVelocityFvPatchVectorField onto a new patch
    fixedNormalInletOutletVelocityFvPatchVectorField
    (
      const fixedNormalInletOutletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedNormalInletOutletVelocityFvPatchVectorField
    (
      const fixedNormalInletOutletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new fixedNormalInletOutletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedNormalInletOutletVelocityFvPatchVectorField
    (
      const fixedNormalInletOutletVelocityFvPatchVectorField&,
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
        new fixedNormalInletOutletVelocityFvPatchVectorField(*this, iF)
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
      Switch fixTangentialInflow() const
      {
        return fixTangentialInflow_;
      }
      //- Return the BC which provides the normal component of velocity
      const fvPatchVectorField& normalVelocity() const
      {
        return normalVelocity_();
      }
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
