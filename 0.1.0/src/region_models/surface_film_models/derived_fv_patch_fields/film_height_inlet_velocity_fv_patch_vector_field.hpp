// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmHeightInletVelocityFvPatchVectorField
// Group
//   grpSurfaceFilmBoundaryConditions
// Description
//   This boundary condition is designed to be used in conjunction with
//   surface film modelling.  It provides a velocity inlet boundary condition
//   for patches where the film height is specified.  The inflow velocity is
//   obtained from the flux with a direction normal to the patch faces using:
//   \f[
//     U_p = \frac{n \phi}{\rho |Sf| \delta}
//   \f]
//   where
//   \vartable
//     U_p    | patch velocity [m/s]
//     n      | patch normal vector
//     \phi   | mass flux [kg/s]
//     \rho   | density [kg/m3]
//     Sf     | patch face area vectors [m2]
//     \delta | film height [m]
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | Flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     deltaf       | height field name       | no          | deltaf
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            filmHeightInletVelocity;
//     phi             phi;
//     rho             rho;
//     deltaf          deltaf;
//     value           uniform (0 0 0); // initial velocity / [m/s]
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   film_height_inlet_velocity_fv_patch_vector_field.cpp
#ifndef film_height_inlet_velocity_fv_patch_vector_field_hpp_
#define film_height_inlet_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class filmHeightInletVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Name of flux field
    word phiName_;
    //- Name of density field
    word rhoName_;
    //- Name of film height field
    word deltafName_;
public:
  //- Runtime type information
  TYPE_NAME("filmHeightInletVelocity");
  // Constructors
    //- Construct from patch and internal field
    filmHeightInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    filmHeightInletVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given filmHeightInletVelocityFvPatchVectorField
    //  onto a new patch
    filmHeightInletVelocityFvPatchVectorField
    (
      const filmHeightInletVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    filmHeightInletVelocityFvPatchVectorField
    (
      const filmHeightInletVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new filmHeightInletVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    filmHeightInletVelocityFvPatchVectorField
    (
      const filmHeightInletVelocityFvPatchVectorField&,
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
        new filmHeightInletVelocityFvPatchVectorField(*this, iF)
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
      //- Return the name of deltaf
      const word& deltafName() const
      {
        return deltafName_;
      }
      //- Return reference to the name of df to allow adjustment
      word& deltafName()
      {
        return deltafName_;
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
