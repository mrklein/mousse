// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::porousBafflePressureFvPatchField
// Group
//   grpCoupledBoundaryConditions
// Description
//   This boundary condition provides a jump condition, using the \c cyclic
//   condition as a base.
//   The porous baffle introduces a pressure jump defined by:
//     \f[
//       \Delta p = -(D \mu U + 0.5 I \rho |U|^2 )L
//     \f]
//   where
//   \vartable
//     p      | pressure [Pa]
//     \rho   | density [kg/m3]
//     \mu    | laminar viscosity [Pa s]
//     I      | inertial coefficient
//     D      | Darcy coefficient
//     L      | porous media length in the flow direction
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     patchType    | underlying patch type should be \c cyclic| yes |
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     D            | Darcy coefficient       | yes         |
//     I            | inertial coefficient    | yes         |
//     L            | porous media length in the flow direction | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            porousBafflePressure;
//     patchType       cyclic;
//     jump            uniform 0;
//     D               0.001;
//     I               1000000;
//     L               0.1;
//     value           uniform 0;
//   }
//   \endverbatim
// Note
//   The underlying \c patchType should be set to \c cyclic
// SourceFiles
//   porous_baffle_pressure_fv_patch_field.cpp
#ifndef porous_baffle_pressure_fv_patch_field_hpp_
#define porous_baffle_pressure_fv_patch_field_hpp_
#include "fixed_jump_fv_patch_field.hpp"
namespace mousse
{
class porousBafflePressureFvPatchField
:
  public fixedJumpFvPatchField<scalar>
{
  // Private data
    //- Name of flux field (default = phi)
    const word phiName_;
    //- Name of density field (default = rho)
    const word rhoName_;
    //- Darcy pressure loss coefficient
    scalar D_;
    //- Inertia pressure lost coefficient
    scalar I_;
    //- Porous media length
    scalar length_;
public:
  //- Runtime type information
  TypeName("porousBafflePressure");
  // Constructors
    //- Construct from patch and internal field
    porousBafflePressureFvPatchField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    porousBafflePressureFvPatchField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given porousBafflePressureFvPatchField
    // onto a new patch
    porousBafflePressureFvPatchField
    (
      const porousBafflePressureFvPatchField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    porousBafflePressureFvPatchField
    (
      const porousBafflePressureFvPatchField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar> > clone() const
    {
      return tmp<fvPatchField<scalar> >
      (
        new porousBafflePressureFvPatchField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    porousBafflePressureFvPatchField
    (
      const porousBafflePressureFvPatchField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar> > clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<scalar> >
      (
        new porousBafflePressureFvPatchField(*this, iF)
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
