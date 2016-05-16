#ifndef LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TREITZ_KHRT_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TREITZ_KHRT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReitzKHRT
// Description
//  secondary breakup model which uses the Kelvin-Helmholtz
//   instability theory to predict the 'stripped' droplets... and
//   the Raleigh-Taylor instability as well.

#include "_breakup_model.hpp"


namespace mousse {

template<class CloudType>
class ReitzKHRT
:
  public BreakupModel<CloudType>
{
private:
  // Private data
    // model constants
    scalar b0_;
    scalar b1_;
    scalar cTau_;
    scalar cRT_;
    scalar msLimit_;
    scalar weberLimit_;
public:
  //- Runtime type information
  TYPE_NAME("ReitzKHRT");
  // Constructors
    //- Construct from dictionary
    ReitzKHRT(const dictionary&, CloudType&);
    //- Construct copy
    ReitzKHRT(const ReitzKHRT<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType>> clone() const
    {
      return
        autoPtr<BreakupModel<CloudType>>
        {
          new ReitzKHRT<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~ReitzKHRT();
  // Member Functions
    //- Update the parcel diameter
    virtual bool update
    (
      const scalar dt,
      const vector& g,
      scalar& d,
      scalar& tc,
      scalar& ms,
      scalar& nParticle,
      scalar& KHindex,
      scalar& y,
      scalar& yDot,
      const scalar d0,
      const scalar rho,
      const scalar mu,
      const scalar sigma,
      const vector& U,
      const scalar rhoc,
      const scalar muc,
      const vector& Urel,
      const scalar Urmag,
      const scalar tMom,
      scalar& dChild,
      scalar& massChild
    );
};

}  // namespace mousse

#include "_reitz_khrt.ipp"

#endif
