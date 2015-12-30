// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRFForce
// Description
//   Calculates particle SRF reference frame force
// SourceFiles
//   _srf_force.cpp
#ifndef _srf_force_hpp_
#define _srf_force_hpp_
#include "_particle_force.hpp"
#include "srf_model.hpp"
namespace mousse
{
class fvMesh;
template<class CloudType>
class SRFForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Pointer to the SRF model
    const typename SRF::SRFModel* srfPtr_;
public:
  //- Runtime type information
  TypeName("SRF");
  // Constructors
    //- Construct from mesh
    SRFForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    SRFForce(const SRFForce& srff);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new ParticleForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~SRFForce();
  // Member Functions
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the non-coupled force
      virtual forceSuSp calcNonCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "_srf_force.cpp"
#endif
#endif
