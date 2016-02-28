// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
// Group
//   grpCmpWallFunctions
// Description
//   A simple alphatPhaseChangeWallFunctionFvPatchScalarField with a fixed
//   volumetric phase-change mass flux.
// SeeAlso
//   mousse::compressible::alphatPhaseChangeWallFunctionFvPatchScalarField
// SourceFiles
//   alphat_fixed_dmdt_wall_boiling_wall_function_fv_patch_scalar_field.cpp
#ifndef compressibleAlphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField_H
#define compressibleAlphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField_H
#include "alphat_phase_change_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
namespace compressible
{
class alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
:
  public alphatPhaseChangeWallFunctionFvPatchScalarField
{
  // Private data
    //- Turbulent Prandtl number
    scalar Prt_;
    //- Cmu coefficient
    scalar Cmu_;
    //- Von Karman constant
    scalar kappa_;
    //- E coefficient
    scalar E_;
    //- E coefficient
    scalar fixedDmdt_;
    // Solution parameters
      static scalar maxExp_;
      static scalar tolerance_;
      static label maxIters_;
  // Private Member Functions
    //- Check the type of the patch
    void checkType();
    //- 'P' function
    tmp<scalarField> Psmooth(const scalarField& Prat) const;
    //- Calculate y+ at the edge of the thermal laminar sublayer
    tmp<scalarField> yPlusTherm
    (
      const scalarField& P,
      const scalarField& Prat
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("compressible::alphatFixedDmdtWallBoilingWallFunction");
  // Constructors
    //- Construct from patch and internal field
    alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    //  onto a new patch
    alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    (
      const alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    (
      const alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
        {
          *this
        }
      };
    }
    //- Construct as copy setting internal field reference
    alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
    (
      const alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      {
        new alphatFixedDmdtWallBoilingWallFunctionFvPatchScalarField
        {
          *this,
          iF
        }
      };
    }
  // Member functions
    //- Return the rate of phase-change
    virtual const scalarField& dmdt() const
    {
      return dmdt_;
    }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace compressible
}  // namespace mousse
#endif
