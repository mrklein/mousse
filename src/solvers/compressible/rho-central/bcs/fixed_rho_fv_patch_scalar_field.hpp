#ifndef SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_FIXED_RHO_FV_PATCH_SCALAR_FIELD_HPP_
#define SOLVERS_COMPRESSIBLE_RHO_CENTRAL_BCS_FIXED_RHO_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedRhoFvPatchScalarField
// Group
//   grpInletBoundaryConditions
// Description
//   mousse::fixedRhoFvPatchScalarField
//   This boundary condition provides a fixed density inlet condition for
//   compressible solvers, where the density of calculated using:
//   \f{
//     \rho = \psi p
//   \f]
//   where
//   \vartable
//     p     | pressure [Pa]
//     \rho  | density [kg/m3]
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     pName        | Pressure field name     | no          | p
//     psiName      | Compressibility field name | no       | thermo:psi
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedRho;
//   }
//   \endverbatim

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class fixedRhoFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
private:
  // Private data
    //- Pressure field name, default = "p"
    word pName_;
    //- Compressibility field name, default = "thermo:psi"
    word psiName_;
public:
  //- Runtime type information
  TYPE_NAME("fixedRho");
  // Constructors
    //- Construct from patch and internal field
    fixedRhoFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedRhoFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedRhoFvPatchScalarField
    // onto a new patch
    fixedRhoFvPatchScalarField
    (
      const fixedRhoFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedRhoFvPatchScalarField
    (
      const fixedRhoFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new fixedRhoFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    fixedRhoFvPatchScalarField
    (
      const fixedRhoFvPatchScalarField&,
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
          new fixedRhoFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

