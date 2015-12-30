// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::epsilonLowReWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulence dissipation wall function
//   condition for low- and high-Reynolds number turbulent flow cases.
//   The condition can be applied to wall boundaries, whereby it inserts near
//   wall epsilon values directly into the epsilon equation to act as a
//   constraint.
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
//     type            epsilonLowReWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::epsilonWallFunctionFvPatchScalarField
// SourceFiles
//   epsilon_low_re_wall_function_fv_patch_scalar_field.cpp
#ifndef epsilon_low_re_wall_function_fv_patch_scalar_field_hpp_
#define epsilon_low_re_wall_function_fv_patch_scalar_field_hpp_
#include "epsilon_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
class epsilonLowReWallFunctionFvPatchScalarField
:
  public epsilonWallFunctionFvPatchScalarField
{
protected:
  // Protected data
    //- Y+ at the edge of the laminar sublayer
    scalar yPlusLam_;
  // Protected Member Functions
    //- Calculate the Y+ at the edge of the laminar sublayer
    scalar yPlusLam(const scalar kappa, const scalar E);
    //- Calculate the epsilon and G
    virtual void calculate
    (
      const turbulenceModel& turbulence,
      const List<scalar>& cornerWeights,
      const fvPatch& patch,
      scalarField& G,
      scalarField& epsilon
    );
public:
  //- Runtime type information
  TypeName("epsilonLowReWallFunction");
  // Constructors
    //- Construct from patch and internal field
    epsilonLowReWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    epsilonLowReWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  epsilonLowReWallFunctionFvPatchScalarField
    //  onto a new patch
    epsilonLowReWallFunctionFvPatchScalarField
    (
      const epsilonLowReWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    epsilonLowReWallFunctionFvPatchScalarField
    (
      const epsilonLowReWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new epsilonLowReWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    epsilonLowReWallFunctionFvPatchScalarField
    (
      const epsilonLowReWallFunctionFvPatchScalarField&,
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
        new epsilonLowReWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  //- Destructor
  virtual ~epsilonLowReWallFunctionFvPatchScalarField()
  {}
};
}  // namespace mousse
#endif
