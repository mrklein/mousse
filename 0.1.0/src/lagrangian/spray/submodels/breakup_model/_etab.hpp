// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ETAB
// Description
//   The Enhanced TAB model.
//   Described in the papers below.
//   @verbatim
//   F.X. Tanner
//     "Liquid Jet Atomization and Droplet Breakup Modeling of
//     Non-Evaporating Diesel Fuel Sprays"
//     SAE 970050,
//     SAE Transactions: Journal of Engines, Vol 106, Sec 3 pp 127-140
//   F.X. Tanner and G. Weisser
//     "Simulation of Liquid Jet Atomization for
//     Fuel Sprays by Means of Cascade Drop Breakup Model"
//     SAE 980808
//     SAE Technical Paper Series
//   @endverbatim
// See Also
//   The TAB model

#ifndef _etab_hpp_
#define _etab_hpp_

#include "_breakup_model.hpp"

namespace mousse
{
template<class CloudType>
class ETAB
:
  public BreakupModel<CloudType>
{
private:
  // Private data
    // Model constants
      scalar k1_;
      scalar k2_;
      scalar WeTransition_;
      scalar AWe_;
public:
  //- Runtime type information
  TypeName("ETAB");
  // Constructors
    //- Construct from dictionary
    ETAB(const dictionary&, CloudType&);
    //- Construct copy
    ETAB(const ETAB<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType> > clone() const
    {
      return autoPtr<BreakupModel<CloudType> >
      (
        new ETAB<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ETAB();
  // Member Functions
    //- Update the parcel properties
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

#ifdef NoRepository
#   include "_etab.cpp"
#endif
#endif
