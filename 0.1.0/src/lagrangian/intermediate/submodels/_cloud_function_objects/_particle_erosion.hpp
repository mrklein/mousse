#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_EROSION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_EROSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleErosion
// Description
//   Creates particle erosion field, Q
// SourceFiles
//   _particle_erosion.cpp
#include "_cloud_function_object.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class CloudType>
class ParticleErosion
:
  public CloudFunctionObject<CloudType>
{
  // Private Data
    // Typedefs
      //- Convenience typedef for parcel type
      typedef typename CloudType::parcelType parcelType;
    //- Particle erosion field
    autoPtr<volScalarField> QPtr_;
    //- List of patch indices to post-process
    labelList patchIDs_;
    //- Plastic flow stress - typical metal value = 2.7 GPa
    scalar p_;
    //- Ratio between depth of contact and length of cut - default=2
    scalar psi_;
    //- Ratio of normal and tangential forces - default=2
    scalar K_;
protected:
  // Protected Member Functions
    //- Returns local patchI if patch is in patchIds_ list
    label applyToPatch(const label globalPatchI) const;
    //- Write post-processing info
    virtual void write();
public:
  //- Runtime type information
  TYPE_NAME("particleErosion");
  // Constructors
    //- Construct from dictionary
    ParticleErosion
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ParticleErosion(const ParticleErosion<CloudType>& pe);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new ParticleErosion<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ParticleErosion();
  // Member Functions
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-patch hook
      virtual void postPatch
      (
        const parcelType& p,
        const polyPatch& pp,
        const scalar trackFraction,
        const tetIndices& tetIs,
        bool& keepParticle
      );
};
}  // namespace mousse
#ifdef NoRepository
#   include "_particle_erosion.cpp"
#endif
#endif
