#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRESSURE_INLET_UNIFORM_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRESSURE_INLET_UNIFORM_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureInletUniformVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions
// Description
//   This velocity inlet boundary condition is applied to patches where the
//   pressure is specified.  The uniform inflow velocity is obtained by
//   averaging the flux over the patch, and then applying it in the direction
//   normal to the patch faces.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            pressureInletUniformVelocity;
//     value           uniform 0;
//   }
//   \endverbatim

#include "pressure_inlet_velocity_fv_patch_vector_field.hpp"


namespace mousse {

class pressureInletUniformVelocityFvPatchVectorField
:
  public pressureInletVelocityFvPatchVectorField
{
public:
  //- Runtime type information
  TYPE_NAME("pressureInletUniformVelocity");
  // Constructors
    //- Construct from patch and internal field
    pressureInletUniformVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    pressureInletUniformVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  pressureInletUniformVelocityFvPatchVectorField onto a new patch
    pressureInletUniformVelocityFvPatchVectorField
    (
      const pressureInletUniformVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    pressureInletUniformVelocityFvPatchVectorField
    (
      const pressureInletUniformVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      {
        new pressureInletUniformVelocityFvPatchVectorField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    pressureInletUniformVelocityFvPatchVectorField
    (
      const pressureInletUniformVelocityFvPatchVectorField&,
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
        new pressureInletUniformVelocityFvPatchVectorField{*this, iF}
      };
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
  // Member operators
    virtual void operator=(const fvPatchField<vector>& pvf);
};
}  // namespace mousse
#endif
