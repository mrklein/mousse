#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TDAMPING_MODELS_TNO_DAMPING_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TDAMPING_MODELS_TNO_DAMPING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DampingModels::NoDamping

#include "_damping_model.hpp"


namespace mousse {
namespace DampingModels {

template<class CloudType>
class NoDamping
:
  public DampingModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    NoDamping(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoDamping(const NoDamping<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<DampingModel<CloudType>> clone() const
    {
      return
        autoPtr<DampingModel<CloudType>>
        {
          new NoDamping<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~NoDamping();
  //- Member Functions
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
    //- Return the model 'active' status
    virtual bool active() const;
};

}  // namespace DampingModels
}  // namespace mousse

#include "_no_damping.ipp"

#endif
