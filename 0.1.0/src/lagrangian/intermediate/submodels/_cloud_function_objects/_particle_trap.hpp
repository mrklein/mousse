#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_TRAP_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_TRAP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleTrap
// Description
//   Traps particles within a given phase fraction for multi-phase cases
//   Model is activated using:
//     particleTrap1
//     {
//       type        particleTrap;
//       alphaName   alpha;      // name volume fraction field
//       threshold   0.95;       // alpha value below which model is active
//     }
// SourceFiles
//   _particle_trap.cpp
#include "_cloud_function_object.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class CloudType>
class ParticleTrap
:
  public CloudFunctionObject<CloudType>
{
  // Private Data
    // Typedefs
      //- Convenience typedef for parcel type
      typedef typename CloudType::parcelType parcelType;
    //- Name of vol fraction field
    const word alphaName_;
    //- Pointer to the volume fraction field
    const volScalarField* alphaPtr_;
    //- Gradient of the volume fraction field
    autoPtr<volVectorField> gradAlphaPtr_;
    //- Threshold beyond which model is active
    scalar threshold_;
public:
  //- Runtime type information
  TYPE_NAME("particleTrap");
  // Constructors
    //- Construct from dictionary
    ParticleTrap
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ParticleTrap(const ParticleTrap<CloudType>& pe);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new ParticleTrap<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ParticleTrap();
  // Member Functions
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-evolve hook
      virtual void postEvolve();
      //- Post-move hook
      virtual void postMove
      (
        typename CloudType::parcelType& p,
        const label cellI,
        const scalar dt,
        const point& position0,
        bool& keepParticle
      );
};
}  // namespace mousse
#ifdef NoRepository
  #include "_particle_trap.cpp"
#endif
#endif
