// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRFVelocityFvPatchVectorField
// Description
//   Freestream velocity condition to be used in conjunction with the single
//   rotating frame (SRF) model (see: SRFModel class)
//   Given the free stream velocity in the absolute frame, the condition
//   applies the appropriate rotation transformation in time and space to
//   determine the local velocity using:
//     \f[
//       U_p = cos(\theta)*U_{Inf} + sin(theta) (n^UInf) - U_{p,srf}
//     \f]
//   where
//   \vartable
//     U_p     = patch velocity [m/s]
//     U_{Inf} = free stream velocity in the absolute frame [m/s]
//     theta   = swept angle [rad]
//     n       = axis direction of the SRF
//     U_{p,srf} = SRF velocity of the patch
//   \endvartable
//   \heading Patch usage
//   \table
//     Property        | Description               | Required | Default value
//     UInf            | freestream velocity       | yes      |
//     relative        | UInf relative to the SRF? | no       |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            SRFFreestreamVelocity;
//     UInf            uniform (0 0 0);
//     relative        no;
//     value           uniform (0 0 0);    // initial value
//   }
//   \endverbatim
// SeeAlso
//   mousse::freestreamFvPatchField
//   mousse::SRFVelocityFvPatchVectorField
// SourceFiles
//   srf_freestream_velocity_fv_patch_vector_field.cpp
#ifndef srf_freestream_velocity_fv_patch_vector_field_hpp_
#define srf_freestream_velocity_fv_patch_vector_field_hpp_
#include "inlet_outlet_fv_patch_fields.hpp"
#include "switch.hpp"
namespace mousse
{
class SRFFreestreamVelocityFvPatchVectorField
:
  public inletOutletFvPatchVectorField
{
  // Private data
    //- Is the supplied inlet value relative to the SRF
    Switch relative_;
    //- Velocity of the free stream in the absolute frame [m/s]
    vector UInf_;
public:
  //- Runtime type information
  TypeName("SRFFreestreamVelocity");
  // Constructors
    //- Construct from patch and internal field
    SRFFreestreamVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    SRFFreestreamVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given SRFFreestreamVelocityFvPatchVectorField
    //  onto a new patch
    SRFFreestreamVelocityFvPatchVectorField
    (
      const SRFFreestreamVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    SRFFreestreamVelocityFvPatchVectorField
    (
      const SRFFreestreamVelocityFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new SRFFreestreamVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    SRFFreestreamVelocityFvPatchVectorField
    (
      const SRFFreestreamVelocityFvPatchVectorField&,
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
        new SRFFreestreamVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the velocity at infinity
      const vector& UInf() const
      {
        return UInf_;
      }
      //- Return reference to the velocity at infinity to allow adjustment
      vector& UInf()
      {
        return UInf_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
