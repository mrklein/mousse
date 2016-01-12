// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::convectiveHeatTransferFvPatchScalarField
// Group
//   grpCmpBoundaryConditions
// Description
//   This boundary condition provides a convective heat transfer coefficient
//   condition
//   if Re > 500000
//   \f[
//     htc_p = \frac{0.664 Re^{0.5} Pr^{0.333} \kappa_p}{L}
//   \f]
//   else
//   \f[
//     htc_p = \frac{0.037 Re^{0.8} Pr^{0.333} \kappa_p}{L}
//   \f]
//   where
//   \vartable
//     htc_p   | patch convective heat transfer coefficient
//     Re      | Reynolds number
//     Pr      | Prandtl number
//     \kappa_p | thermal conductivity
//     L       | length scale
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     L            | Length scale [m]        | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            convectiveHeatTransfer;
//     L               0.1;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   convective_heat_transfer_fv_patch_scalar_field.cpp
#ifndef compressibleMutRoughWallFunctionFvPatchScalarField_H
#define compressibleMutRoughWallFunctionFvPatchScalarField_H
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
namespace compressible
{
class convectiveHeatTransferFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
protected:
  // Protected data
    //- L Length scale [m]
    const scalar L_;
public:
  //- Runtime type information
  TYPE_NAME("convectiveHeatTransfer");
  // Constructors
    //- Construct from patch and internal field
    convectiveHeatTransferFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    convectiveHeatTransferFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  convectiveHeatTransferFvPatchScalarField
    //  onto a new patch
    convectiveHeatTransferFvPatchScalarField
    (
      const convectiveHeatTransferFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    convectiveHeatTransferFvPatchScalarField
    (
      const convectiveHeatTransferFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new convectiveHeatTransferFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    convectiveHeatTransferFvPatchScalarField
    (
      const convectiveHeatTransferFvPatchScalarField&,
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
        new convectiveHeatTransferFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
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
