// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RASModels::v2WallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulence stress normal to streamlines
//   wall function condition for low- and high-Reynolds number, turbulent flow
//   cases.
//   The model operates in two modes, based on the computed laminar-to-turbulent
//   switch-over y+ value derived from kappa and E.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Cmu          | model coefficient       | no          | 0.09
//     kappa        | Von Karman constant     | no          | 0.41
//     E            | model coefficient       | no          | 9.8
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            v2WallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   v2_wall_function_fv_patch_scalar_field.cpp
#ifndef v2_wall_function_fv_patch_scalar_field_hpp_
#define v2_wall_function_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_field.hpp"
namespace mousse
{
namespace RASModels
{
class v2WallFunctionFvPatchScalarField
:
  public fixedValueFvPatchField<scalar>
{
protected:
  // Protected data
    //- Cmu coefficient
    scalar Cmu_;
    //- Von Karman constant
    scalar kappa_;
    //- E coefficient
    scalar E_;
    //- Y+ at the edge of the laminar sublayer
    scalar yPlusLam_;
  // Protected Member Functions
    //- Check the type of the patch
    virtual void checkType();
    //- Write local wall function variables
    virtual void writeLocalEntries(Ostream&) const;
    //- Calculate the Y+ at the edge of the laminar sublayer
    scalar yPlusLam(const scalar kappa, const scalar E);
public:
  //- Runtime type information
  TypeName("v2WallFunction");
  // Constructors
    //- Construct from patch and internal field
    v2WallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    v2WallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given v2WallFunctionFvPatchScalarField
    //  onto a new patch
    v2WallFunctionFvPatchScalarField
    (
      const v2WallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    v2WallFunctionFvPatchScalarField
    (
      const v2WallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new v2WallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    v2WallFunctionFvPatchScalarField
    (
      const v2WallFunctionFvPatchScalarField&,
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
        new v2WallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
      //- Evaluate the patchField
      virtual void evaluate(const Pstream::commsTypes);
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace RASModels
}  // namespace mousse
#endif
