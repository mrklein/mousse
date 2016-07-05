#ifndef REGION_MODELS_SURFACE_FILM_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUTK_FILM_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUTK_FILM_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::RASModels::nutkFilmWallFunctionFvPatchScalarField
// Group
//   grpSurfaceFilmBoundaryConditions grpCmpWallFunctions
// Description
//   This boundary condition provides a turbulent viscosity condition when
//   using wall functions, based on turbulence kinetic energy, for use with
//   surface film models.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutkFilmWallFunction;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutkWallFunctionFvPatchScalarField

#include "nutk_wall_function_fv_patch_scalar_field.hpp"


namespace mousse {
namespace compressible {
namespace RASModels {

class nutkFilmWallFunctionFvPatchScalarField
:
  public nutkWallFunctionFvPatchScalarField
{
protected:
  // Protected data
    //- B Coefficient (default = 5.5)
    scalar B_;
    //- y+ value for laminar -> turbulent transition (default = 11.05)
    scalar yPlusCrit_;
  // Protected member functions
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
    //- Calculate the friction velocity
    virtual tmp<scalarField> calcUTau(const scalarField& magGradU) const;
public:
  //- Runtime type information
  TYPE_NAME("nutkFilmWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutkFilmWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutkFilmWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutkFilmWallFunctionFvPatchScalarField
    //  onto a new patch
    nutkFilmWallFunctionFvPatchScalarField
    (
      const nutkFilmWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutkFilmWallFunctionFvPatchScalarField
    (
      const nutkFilmWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new nutkFilmWallFunctionFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    nutkFilmWallFunctionFvPatchScalarField
    (
      const nutkFilmWallFunctionFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new nutkFilmWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Calculate and return the yPlus at the boundary
      virtual tmp<scalarField> yPlus() const;
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};

}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse

#endif

