#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TPRESSURE_GRADIENT_FORCE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPARTICLE_FORCES_TPRESSURE_GRADIENT_FORCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PressureGradientForce
// Description
//   Calculates particle pressure gradient force

#include "_particle_force.hpp"
#include "vol_fields.hpp"
#include "interpolation.hpp"


namespace mousse {

template<class CloudType>
class PressureGradientForce
:
  public ParticleForce<CloudType>
{
protected:
  // Protected data
    //- Name of velocity field
    const word UName_;
    //- Rate of change of carrier phase velocity interpolator
    autoPtr<interpolation<vector>> DUcDtInterpPtr_;
public:
  //- Runtime type information
  TYPE_NAME("pressureGradient");
  // Constructors
    //- Construct from mesh
    PressureGradientForce
    (
      CloudType& owner,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& forceType = typeName
    );
    //- Construct copy
    PressureGradientForce(const PressureGradientForce& pgf);
    //- Construct and return a clone
    virtual autoPtr<ParticleForce<CloudType>> clone() const
    {
      return
        autoPtr<ParticleForce<CloudType>>
        {
          new PressureGradientForce<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~PressureGradientForce();
  // Member Functions
    // Access
      //- Return the rate of change of carrier phase velocity interpolator
      inline const interpolation<vector>& DUcDtInterp() const;
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
      //- Return the added mass
      virtual scalar massAdd
      (
        const typename CloudType::parcelType& p,
        const scalar mass
      ) const;
};

}  // namespace mousse


template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::PressureGradientForce<CloudType>::DUcDtInterp() const
{
  if (!DUcDtInterpPtr_.valid()) {
    FATAL_ERROR_IN
    (
      "inline const mousse::interpolation<mousse::vector>&"
      "mousse::PressureGradientForce<CloudType>::DUcDtInterp() const"
    )
    << "Carrier phase DUcDt interpolation object not set"
    << abort(FatalError);
  }
  return DUcDtInterpPtr_();
}

#include "_pressure_gradient_force.ipp"

#endif
