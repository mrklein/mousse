// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rotatingTotalPressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a total pressure condition for patches
//   in a rotating frame.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velocity field name     | no          | U
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | none
//     psi          | compressibility field name | no       | none
//     gamma        | ratio of specific heats (Cp/Cv) | yes |
//     p0           | static pressure reference | yes       |
//     omega        | angular velocty of the frame [rad/s] | yes    |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            rotatingTotalPressure;
//     U               U;
//     phi             phi;
//     rho             rho;
//     psi             psi;
//     gamma           1.4;
//     p0              uniform 1e5;
//     omega           100;
//   }
//   \endverbatim
//   The \c omega entry is a DataEntry type, able to describe time varying
//   functions.
// SeeAlso
//   mousse::totalPressureFvPatchScalarField
// SourceFiles
//   rotating_total_pressure_fv_patch_scalar_field.cpp
#ifndef rotating_total_pressure_fv_patch_scalar_field_hpp_
#define rotating_total_pressure_fv_patch_scalar_field_hpp_
#include "total_pressure_fv_patch_scalar_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
class rotatingTotalPressureFvPatchScalarField
:
  public totalPressureFvPatchScalarField
{
  // Private data
    //- Angular velocity of the frame
    autoPtr<DataEntry<vector> > omega_;
public:
  //- Runtime type information
  TypeName("rotatingTotalPressure");
  // Constructors
    //- Construct from patch and internal field
    rotatingTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    rotatingTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given rotatingTotalPressureFvPatchScalarField
    //  onto a new patch
    rotatingTotalPressureFvPatchScalarField
    (
      const rotatingTotalPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    rotatingTotalPressureFvPatchScalarField
    (
      const rotatingTotalPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new rotatingTotalPressureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    rotatingTotalPressureFvPatchScalarField
    (
      const rotatingTotalPressureFvPatchScalarField&,
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
        new rotatingTotalPressureFvPatchScalarField(*this, iF)
      );
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
