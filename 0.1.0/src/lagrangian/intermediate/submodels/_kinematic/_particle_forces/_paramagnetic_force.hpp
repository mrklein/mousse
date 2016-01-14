// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParamagneticForce
// Description
//   Calculates particle paramagnetic (magnetic field) force
// SourceFiles
//   _paramagnetic_force_i.hpp
//   _paramagnetic_force.cpp
#ifndef _paramagnetic_force_hpp_
#define _paramagnetic_force_hpp_
#include "_particle_force.hpp"
#include "interpolation.hpp"
namespace mousse
{
class fvMesh;
template<class CloudType>
class ParamagneticForce
:
  public ParticleForce<CloudType>
{
  // Private data
    //- Name of paramagnetic field strength field - default = "HdotGradH"
    const word HdotGradHName_;
    //- HdotGradH interpolator
    const interpolation<vector>* HdotGradHInterpPtr_;
    //- Magnetic susceptibility of particle
    const scalar magneticSusceptibility_;
public:
  //- Runtime type information
  TYPE_NAME("paramagnetic");
  // Constructors
    //- Construct from mesh
    ParamagneticForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Construct copy
    ParamagneticForce(const ParamagneticForce& gf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new ParamagneticForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ParamagneticForce();
  // Member Functions
    // Access
      //- Return the name of paramagnetic field strength field
      const word& HdotGradHName() const;
      //- Return the magnetic susceptibility of particle
      scalar magneticSusceptibility() const;
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
#include "_paramagnetic_force_i.hpp"
#ifdef NoRepository
  #include "_paramagnetic_force.cpp"
#endif
#endif
