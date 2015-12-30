// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformTotalPressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a time-varying form of the uniform total
//   pressure boundary condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velocity field name     | no          | U
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | none
//     psi          | compressibility field name | no       | none
//     gamma        | ratio of specific heats (Cp/Cv) | yes |
//     pressure     | total pressure as a function of time | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            uniformTotalPressure;
//     U               U;
//     phi             phi;
//     rho             rho;
//     psi             psi;
//     gamma           1.4;
//     pressure        uniform 1e5;
//   }
//   \endverbatim
//   The \c pressure entry is specified as a DataEntry type, able to describe
//   time varying functions.
// Note
//   The default boundary behaviour is for subsonic, incompressible flow.
// SeeAlso
//   mousse::DataEntry
//   mousse::uniformFixedValueFvPatchField
//   mousse::totalPressureFvPatchField
// SourceFiles
//   uniform_total_pressure_fv_patch_scalar_field.cpp
#ifndef uniform_total_pressure_fv_patch_scalar_field_hpp_
#define uniform_total_pressure_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "data_entry.hpp"
namespace mousse
{
class uniformTotalPressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Name of the velocity field
    word UName_;
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of the density field used to normalise the mass flux
    //  if neccessary
    word rhoName_;
    //- Name of the compressibility field used to calculate the wave speed
    word psiName_;
    //- Heat capacity ratio
    scalar gamma_;
    //- Table of time vs total pressure, including the bounding treatment
    autoPtr<DataEntry<scalar> > pressure_;
public:
  //- Runtime type information
  TypeName("uniformTotalPressure");
  // Constructors
    //- Construct from patch and internal field
    uniformTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given patch field onto a new patch
    uniformTotalPressureFvPatchScalarField
    (
      const uniformTotalPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformTotalPressureFvPatchScalarField
    (
      const uniformTotalPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new uniformTotalPressureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    uniformTotalPressureFvPatchScalarField
    (
      const uniformTotalPressureFvPatchScalarField&,
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
        new uniformTotalPressureFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the name of the velocity field
      const word& UName() const
      {
        return UName_;
      }
      //- Return reference to the name of the velocity field
      //  to allow adjustment
      word& UName()
      {
        return UName_;
      }
      //- Return the heat capacity ratio
      scalar gamma() const
      {
        return gamma_;
      }
      //- Return reference to the heat capacity ratio to allow adjustment
      scalar& gamma()
      {
        return gamma_;
      }
    // Evaluation functions
      //- Inherit updateCoeffs from fixedValueFvPatchScalarField
      using fixedValueFvPatchScalarField::updateCoeffs;
      //- Update the coefficients associated with the patch field
      //  using the given patch velocity field
      virtual void updateCoeffs(const vectorField& Up);
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
