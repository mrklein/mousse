#ifndef REGION_MODELS_SURFACE_FILM_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_FILM_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_FILM_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::RASModels::alphatFilmWallFunctionFvPatchScalarField
// Group
//   grpSurfaceFilmBoundaryConditions grpCmpWallFunctions
// Description
//   This boundary condition provides a turbulent thermal diffusivity condition
//   when using wall functions, for use with surface film models.  This
//   condition varies from the standard wall function by taking into account any
//   mass released from the film model.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     B            | model coefficient       | no          | 5.5
//     yPlusCrit    | critical y+ for transition to turbulent flow | no|11.05
//     Cmu          | model coefficient       | no          | 0.09
//     kappa        | Von-Karman constant     | no          | 0.41
//     Prt          | turbulent Prandtl number | no         | 0.85
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            alphatFilmWallFunction;
//     B               5.5;
//     yPlusCrit       11.05;
//     Cmu             0.09;
//     kappa           0.41;
//     Prt             0.85;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {
namespace compressible {
namespace RASModels {

class alphatFilmWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
protected:
  // Protected data
    //- B Coefficient (default = 5.5)
    scalar B_;
    //- y+ value for laminar -> turbulent transition (default = 11.05)
    scalar yPlusCrit_;
    //- Turbulent Cmu coefficient (default = 0.09)
    scalar Cmu_;
    //- Von-Karman constant (default = 0.41)
    scalar kappa_;
    //- Turbulent Prandtl number (default = 0.85)
    scalar Prt_;
public:
  //- Runtime type information
  TYPE_NAME("alphatFilmWallFunction");
  // Constructors
    //- Construct from patch and internal field
    alphatFilmWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphatFilmWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  alphatFilmWallFunctionFvPatchScalarField
    //  onto a new patch
    alphatFilmWallFunctionFvPatchScalarField
    (
      const alphatFilmWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphatFilmWallFunctionFvPatchScalarField
    (
      const alphatFilmWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new alphatFilmWallFunctionFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    alphatFilmWallFunctionFvPatchScalarField
    (
      const alphatFilmWallFunctionFvPatchScalarField&,
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
          new alphatFilmWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};

}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse

#endif

