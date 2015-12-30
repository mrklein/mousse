// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gradientEnergyFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a gradient condition for internal energy,
//   where the gradient is calculated using:
//     \f[
//       \nabla(e_p) = \nabla_\perp C_p(p, T) + \frac{e_p - e_c}{\Delta}
//     \f]
//   where
//   \vartable
//     e_p     | energy at patch faces [J]
//     e_c     | energy at patch internal cells [J]
//     p       | pressure [bar]
//     T       | temperature [K]
//     C_p     | specific heat [J/kg/K]
//     \Delta  | distance between patch face and internal cell centres [m]
//   \endvartable
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            gradientEnergy;
//     gradient        uniform 10;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedGradientFvPatchField
// SourceFiles
//   gradient_energy_fv_patch_scalar_field.cpp
#ifndef gradient_energy_fv_patch_scalar_field_hpp_
#define gradient_energy_fv_patch_scalar_field_hpp_
#include "fixed_gradient_fv_patch_fields.hpp"
namespace mousse
{
class gradientEnergyFvPatchScalarField
:
  public fixedGradientFvPatchScalarField
{
public:
  //- Runtime type information
  TypeName("gradientEnergy");
  // Constructors
    //- Construct from patch and internal field
    gradientEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    gradientEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given gradientEnergyFvPatchScalarField
    // onto a new patch
    gradientEnergyFvPatchScalarField
    (
      const gradientEnergyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    gradientEnergyFvPatchScalarField
    (
      const gradientEnergyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new gradientEnergyFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    gradientEnergyFvPatchScalarField
    (
      const gradientEnergyFvPatchScalarField&,
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
        new gradientEnergyFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};
}  // namespace mousse
#endif
