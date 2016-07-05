#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TPACKING_MODELS_TNO_PACKING_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TPACKING_MODELS_TNO_PACKING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackingModel::NoPacking

#include "_packing_model.hpp"


namespace mousse {
namespace PackingModels {

template<class CloudType>
class NoPacking
:
  public PackingModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    NoPacking(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoPacking(const NoPacking<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<PackingModel<CloudType>> clone() const
    {
      return
        autoPtr<PackingModel<CloudType>>
        {
          new NoPacking<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~NoPacking();
  // Member Functions
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
    //- Return the model 'active' status
    virtual bool active() const;
};

}  // namespace PackingModels
}  // namespace mousse

#include "_no_packing.ipp"

#endif
