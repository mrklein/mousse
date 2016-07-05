#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_DERIVED_FV_PATCH_FIELDS_JOHNSON_JACKSON_PARTICLE_THETA_FV_PATCH_SCALAR_FIELD_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_COMPRESSIBLE_TURBULENCE_MODELS_KINETIC_THEORY_MODELS_DERIVED_FV_PATCH_FIELDS_JOHNSON_JACKSON_PARTICLE_THETA_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::JohnsonJacksonParticleThetaFvPatchScalarField
// Description
//   Robin condition for the particulate granular temperature.
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
//   johnson_jackson_particle_theta_fv_patch_scalar_field.cpp
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class JohnsonJacksonParticleThetaFvPatchScalarField
:
  public mixedFvPatchScalarField
{
  // Private data
    //- Particle-wall restitution coefficient
    dimensionedScalar restitutionCoefficient_;
    //- Specularity coefficient
    dimensionedScalar specularityCoefficient_;
public:
  //- Runtime type information
  TYPE_NAME("JohnsonJacksonParticleTheta");
  // Constructors
    //- Construct from patch and internal field
    JohnsonJacksonParticleThetaFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    JohnsonJacksonParticleThetaFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping onto a new patch
    JohnsonJacksonParticleThetaFvPatchScalarField
    (
      const JohnsonJacksonParticleThetaFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    JohnsonJacksonParticleThetaFvPatchScalarField
    (
      const JohnsonJacksonParticleThetaFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new JohnsonJacksonParticleThetaFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    JohnsonJacksonParticleThetaFvPatchScalarField
    (
      const JohnsonJacksonParticleThetaFvPatchScalarField&,
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
        new JohnsonJacksonParticleThetaFvPatchScalarField(*this, iF)
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
        const fvPatchScalarField&,
        const labelList&
      );
    //- Update the coefficients
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
