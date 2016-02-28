// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::JohnsonJacksonParticleSlipFvPatchVectorField
// Description
//   Partial slip boundary condition for the particulate velocity.
//   References:
//   \verbatim
//     "Multifluid Eulerian modeling of dense gas–solids fluidized bed
//     hydrodynamics: Influence of the dissipation parameters"
//     Reuge, N.,
//     Chemical Engineering Science
//     Volume 63, Issue 22, November 2008, pp. 5540-5551
//   \endverbatim
//   \verbatim
//     "Frictional-collisional constitutive relations for granular materials,
//     with application to plane shearing"
//     Johnson, P.C., and Jackson, R.,
//     Journal of Fluid Mechanics
//     Volume 176, March 1987, pp. 67-93
//   \endverbatim
// SourceFiles
//   johnson_jackson_particle_slip_fv_patch_vector_field.cpp
#ifndef johnson_jackson_particle_slip_fv_patch_vector_field_hpp_
#define johnson_jackson_particle_slip_fv_patch_vector_field_hpp_
#include "partial_slip_fv_patch_fields.hpp"
namespace mousse
{
class JohnsonJacksonParticleSlipFvPatchVectorField
:
  public partialSlipFvPatchVectorField
{
  // Private data
    //- Specularity coefficient
    dimensionedScalar specularityCoefficient_;
public:
  //- Runtime type information
  TYPE_NAME("JohnsonJacksonParticleSlip");
  // Constructors
    //- Construct from patch and internal field
    JohnsonJacksonParticleSlipFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    JohnsonJacksonParticleSlipFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping onto a new patch
    JohnsonJacksonParticleSlipFvPatchVectorField
    (
      const JohnsonJacksonParticleSlipFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    JohnsonJacksonParticleSlipFvPatchVectorField
    (
      const JohnsonJacksonParticleSlipFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new JohnsonJacksonParticleSlipFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    JohnsonJacksonParticleSlipFvPatchVectorField
    (
      const JohnsonJacksonParticleSlipFvPatchVectorField&,
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
        new JohnsonJacksonParticleSlipFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
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
    //- Update the coefficients
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
