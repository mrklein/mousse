// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LiftForce
// Description
//   Base class for particle lift force models
// SourceFiles
//   _lift_force.cpp
#ifndef _lift_force_hpp_
#define _lift_force_hpp_
#include "_particle_force.hpp"
#include "vol_fields.hpp"
#include "interpolation.hpp"
namespace mousse
{
template<class CloudType>
class LiftForce
:
  public ParticleForce<CloudType>
{
protected:
  // Protected data
    //- Name of velocity field
    const word UName_;
    //- Curk of carrier phase velocity interpolator
    autoPtr<interpolation<vector> > curlUcInterpPtr_;
  // Potected Member Functions
    //- Calculate the lift coefficient
    virtual scalar Cl
    (
      const typename CloudType::parcelType& p,
      const vector& curlUc,
      const scalar Re,
      const scalar muc
    ) const;
public:
  // Constructors
    //- Construct from mesh
    LiftForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType
    );
    //- Construct copy
    LiftForce(const LiftForce& lf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType> > clone() const
    {
      return autoPtr<ParticleForce<CloudType> >
      (
        new LiftForce<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~LiftForce();
  // Member Functions
    // Access
      //- Return the curl of the carrier phase velocity interpolator
      inline const interpolation<vector>& curlUcInterp() const;
    // Evaluation
      //- Cache fields
      virtual void cacheFields(const bool store);
      //- Calculate the non-coupled force
      virtual forceSuSp calcCoupled
      (
        const typename CloudType::parcelType& p,
        const scalar dt,
        const scalar mass,
        const scalar Re,
        const scalar muc
      ) const;
};
}  // namespace mousse

template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::LiftForce<CloudType>::curlUcInterp() const
{
  if (!curlUcInterpPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::interpolation<mousse::vector>&"
      "mousse::LiftForce<CloudType>::curlUcInterp() const"
    )
    << "Carrier phase curlUc interpolation object not set"
    << abort(FatalError);
  }
  return curlUcInterpPtr_();
}
#ifdef NoRepository
  #include "_lift_force.cpp"
#endif
#endif
