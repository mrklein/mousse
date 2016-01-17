// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureNormalInletOutletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This velocity inlet/outlet boundary condition is applied to patches where
//   the pressure is specified.  A zero-gradient condition is applied for
//   outflow (as defined by the flux); for inflow, the velocity is obtained from
//   the flux with a direction normal to the patch faces.
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
//     type            pressureNormalInletOutletVelocity;
//     phi             phi;
//     rho             rho;
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   Sign conventions:
//   - positive flux (out of domain): apply zero-gradient condition
//   - negative flux (into of domain): derive from the flux and patch-normal
//    direction
// SeeAlso
//   mousse::mixedFvPatchVectorField
// SourceFiles
//   pressure_normal_inlet_outlet_velocity_fv_patch_vector_field.cpp
#ifndef pressure_normal_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#define pressure_normal_inlet_outlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class pressureNormalInletOutletVelocityFvPatchVectorField
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
  TYPE_NAME("pressureNormalInletOutletVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureNormalInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    pressureNormalInletOutletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  pressureNormalInletOutletVelocityFvPatchVectorField
    //  onto a new patch
    pressureNormalInletOutletVelocityFvPatchVectorField
    (
      const pressureNormalInletOutletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    pressureNormalInletOutletVelocityFvPatchVectorField
    (
      const pressureNormalInletOutletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new pressureNormalInletOutletVelocityFvPatchVectorField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    pressureNormalInletOutletVelocityFvPatchVectorField
    (
      const pressureNormalInletOutletVelocityFvPatchVectorField&,
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
        new pressureNormalInletOutletVelocityFvPatchVectorField
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
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
  // Member operators
    virtual void operator=(const fvPatchField<vector>& pvf);
};
}  // namespace mousse
#endif
