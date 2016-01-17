// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureInletVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This velocity inlet boundary condition is applied to patches where the
//   pressure is specified.  The inflow velocity is obtained from the flux with
//   a direction normal to the patch faces.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            pressureInletVelocity;
//     phi             phi;
//     rho             rho;
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   If reverse flow is possible or expected use
//   the pressureInletOutletVelocityFvPatchVectorField condition instead.
// SeeAlso
//   mousse::fixedValueFvPatchField
//   mousse::pressureInletOutletVelocityFvPatchVectorField
// SourceFiles
//   pressure_inlet_velocity_fv_patch_vector_field.cpp
#ifndef pressure_inlet_velocity_fv_patch_vector_field_hpp_
#define pressure_inlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class pressureInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Flux field name
    word phiName_;
    //- Density field name
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("pressureInletVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    pressureInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given pressureInletVelocityFvPatchVectorField
    //  onto a new patch
    pressureInletVelocityFvPatchVectorField
    (
      const pressureInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    pressureInletVelocityFvPatchVectorField
    (
      const pressureInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new pressureInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    pressureInletVelocityFvPatchVectorField
    (
      const pressureInletVelocityFvPatchVectorField&,
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
        new pressureInletVelocityFvPatchVectorField(*this, iF)
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
