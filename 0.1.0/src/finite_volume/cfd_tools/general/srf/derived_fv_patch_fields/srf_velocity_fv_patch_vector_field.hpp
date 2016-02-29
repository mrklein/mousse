#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_SRF_DERIVED_FV_PATCH_FIELDS_SRF_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_SRF_DERIVED_FV_PATCH_FIELDS_SRF_VELOCITY_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRFVelocityFvPatchVectorField
// Group
//   grpInletBoundaryConditions grpWallBoundaryConditions
// Description
//   Velocity condition to be used in conjunction with the single
//   rotating frame (SRF) model (see: SRFModel class)
//   Given the free stream velocity in the absolute frame, the condition
//   applies the appropriate rotation transformation in time and space to
//   determine the local velocity.
//   The optional \c relative flag switches the behaviour of the patch
//   such that:
//     - relative = yes: inlet velocity applied 'as is':
//     \f[
//       U_p = U_{in}
//     \f]
//     - relative = no : SRF velocity is subtracted from the inlet velocity:
//     \f[
//       U_p = U_{in} - U_{p,srf}
//     \f]
//   where
//   \vartable
//     U_p     = patch velocity [m/s]
//     U_{in}  = user-specified inlet velocity
//     U_{p,srf} = SRF velocity
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     inletValue   | inlet velocity          | yes         |
//     relative     | inletValue relative motion to the SRF? | yes     |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            SRFVelocity;
//     inletValue      uniform (0 0 0);
//     relative        yes;
//     value           uniform (0 0 0);    // initial value
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   srf_velocity_fv_patch_vector_field.cpp
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "switch.hpp"
namespace mousse
{
class SRFVelocityFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Is the supplied inlet value relative to the SRF
    Switch relative_;
    //- Inlet value [m/s]
    vectorField inletValue_;
public:
  //- Runtime type information
  TYPE_NAME("SRFVelocity");
  // Constructors
    //- Construct from patch and internal field
    SRFVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    SRFVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given SRFVelocityFvPatchVectorField
    //  onto a new patch
    SRFVelocityFvPatchVectorField
    (
      const SRFVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    SRFVelocityFvPatchVectorField
    (
      const SRFVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new SRFVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    SRFVelocityFvPatchVectorField
    (
      const SRFVelocityFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new SRFVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return const access to the relative flag
      const Switch& relative() const
      {
        return relative_;
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchVectorField&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
