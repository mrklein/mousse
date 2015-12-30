// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fluxCorrectedVelocityFvPatchVectorField
// Group
//   grpOutletBoundaryConditions
// Description
//   This boundary condition provides a velocity outlet boundary condition for
//   patches where the pressure is specified.  The outflow velocity is obtained
//   by "zeroGradient" and then corrected from the flux:
//     \f[
//       U_p = U_c - n (n \cdot U_c) + \frac{n \phi_p}{|S_f|}
//     \f]
//   where
//   \vartable
//     U_p | velocity at the patch [m/s]
//     U_c | velocity in cells adjacent to the patch [m/s]
//     n   | patch normal vectors
//     \phi_p | flux at the patch [m3/s or kg/s]
//     S_f | patch face area vectors [m2]
//   \endvartable
//   where
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | name of flux field      | no          | phi
//     rho          | name of density field   | no          | rho
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fluxCorrectedVelocity;
//     phi             phi;
//     rho             rho;
//   }
//   \endverbatim
// Note
//   If reverse flow is possible or expected use the
//   pressureInletOutletVelocity condition instead.
// SeeAlso
//   mousse::zeroGradientFvPatchField
//   mousse::pressureInletOutletVelocityFvPatchVectorField
// SourceFiles
//   flux_corrected_velocity_fv_patch_vector_field.cpp
#ifndef flux_corrected_velocity_fv_patch_vector_field_hpp_
#define flux_corrected_velocity_fv_patch_vector_field_hpp_
#include "fv_patch_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
class fluxCorrectedVelocityFvPatchVectorField
:
  public zeroGradientFvPatchVectorField
{
  // Private data
    //- Name of flux field
    word phiName_;
    //- Name of density field
    word rhoName_;
public:
  //- Runtime type information
  TypeName("fluxCorrectedVelocity");
  // Constructors
    //- Construct from patch and internal field
    fluxCorrectedVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fluxCorrectedVelocityFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fluxCorrectedVelocityFvPatchVectorField
    //  onto a new patch
    fluxCorrectedVelocityFvPatchVectorField
    (
      const fluxCorrectedVelocityFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new fluxCorrectedVelocityFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fluxCorrectedVelocityFvPatchVectorField
    (
      const fluxCorrectedVelocityFvPatchVectorField&,
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
        new fluxCorrectedVelocityFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Evaluate the patch field
      virtual void evaluate
      (
        const Pstream::commsTypes commsType=Pstream::blocking
      );
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
