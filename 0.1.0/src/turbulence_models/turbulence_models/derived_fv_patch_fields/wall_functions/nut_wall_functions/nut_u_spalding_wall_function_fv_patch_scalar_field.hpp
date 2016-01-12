// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutUSpaldingWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions for rough walls, based on velocity,  using
//   Spalding's law to give a continuous nut profile to the wall (y+ = 0)
//     \f[
//       y^+ = u^+ + \frac{1}{E} \left[exp(\kappa u^+) - 1 - \kappa u^+\,
//         - 0.5 (\kappa u^+)^2 - \frac{1}{6} (\kappa u^+)^3\right]
//     \f]
//   where
//   \vartable
//     y^+     | non-dimensional position
//     u^+     | non-dimensional velocity
//     \kappa  | Von Karman constant
//   \endvartable
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutUSpaldingWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField
// SourceFiles
//   nut_u_spalding_wall_function_fv_patch_scalar_field.cpp
#ifndef nut_u_spalding_wall_function_fv_patch_scalar_field_hpp_
#define nut_u_spalding_wall_function_fv_patch_scalar_field_hpp_
#include "nut_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
class nutUSpaldingWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
protected:
  // Protected Member Functions
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
    //- Calculate the friction velocity
    virtual tmp<scalarField> calcUTau(const scalarField& magGradU) const;
public:
  //- Runtime type information
  TYPE_NAME("nutUSpaldingWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutUSpaldingWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutUSpaldingWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutUSpaldingWallFunctionFvPatchScalarField
    //  onto a new patch
    nutUSpaldingWallFunctionFvPatchScalarField
    (
      const nutUSpaldingWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutUSpaldingWallFunctionFvPatchScalarField
    (
      const nutUSpaldingWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new nutUSpaldingWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nutUSpaldingWallFunctionFvPatchScalarField
    (
      const nutUSpaldingWallFunctionFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      (
        new nutUSpaldingWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Calculate and return the yPlus at the boundary
      virtual tmp<scalarField> yPlus() const;
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};
}  // namespace mousse
#endif
