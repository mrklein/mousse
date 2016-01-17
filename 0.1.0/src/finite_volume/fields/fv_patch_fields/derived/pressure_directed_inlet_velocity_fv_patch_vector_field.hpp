// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureDirectedInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This velocity inlet boundary condition is applied to patches where the
//   pressure is specified.  The inflow velocity is obtained from the flux with
//   the specified inlet direction" direction.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     inletDirection | inlet direction per patch face | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            pressureDirectedInletVelocity;
//     phi             phi;
//     rho             rho;
//     inletDirection  uniform (1 0 0);
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   If reverse flow is possible or expected use the
//   pressureDirectedInletOutletVelocityFvPatchVectorField condition instead.
// SeeAlso
//   mousse::fixedValueFvPatchField
//   mousse::pressureDirectedInletOutletVelocityFvPatchVectorField
// SourceFiles
//   pressure_directed_inlet_velocity_fv_patch_vector_field.cpp
#ifndef pressure_directed_inlet_velocity_fv_patch_vector_field_hpp_
#define pressure_directed_inlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class pressureDirectedInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Flux field name
    word phiName_;
    //- Density field name
    word rhoName_;
    //- Inlet direction
    vectorField inletDir_;
public:
  //- Runtime type information
  TYPE_NAME("pressureDirectedInletVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureDirectedInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    pressureDirectedInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  pressureDirectedInletVelocityFvPatchVectorField
    //  onto a new patch
    pressureDirectedInletVelocityFvPatchVectorField
    (
      const pressureDirectedInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    pressureDirectedInletVelocityFvPatchVectorField
    (
      const pressureDirectedInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new pressureDirectedInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    pressureDirectedInletVelocityFvPatchVectorField
    (
      const pressureDirectedInletVelocityFvPatchVectorField&,
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
        new pressureDirectedInletVelocityFvPatchVectorField
        (
          *this,
          iF
        )
      );
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
