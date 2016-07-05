#ifndef LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TSHF_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_BREAKUP_MODEL_TSHF_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SHF
// Description
//   Secondary Breakup Model to take account of the different breakup regimes,
//   bag, molutimode, shear....
//   Accurate description in
//   @verbatim
//   R. Schmehl, G. Maier, S. Witting
//   "CFD Analysis of Fuel Atomization, Secondary Droplet Breakup and Spray
//   Dispersion in the Premix Duct of a LPP Combustor".
//   Eight International Conference on Liquid Atomization and Spray Systems, 2000
//   @endverbatim

#include "_breakup_model.hpp"

namespace mousse {

template<class CloudType>
class SHF
:
  public BreakupModel<CloudType>
{
private:
  // Private data
    // Model constants
      scalar weCorrCoeff_;
      scalar weBuCrit_;
      scalar weBuBag_;
      scalar weBuMM_;
      scalar ohnCoeffCrit_;
      scalar ohnCoeffBag_;
      scalar ohnCoeffMM_;
      scalar ohnExpCrit_;
      scalar ohnExpBag_;
      scalar ohnExpMM_;
      scalar cInit_;
      scalar c1_;
      scalar c2_;
      scalar c3_;
      scalar cExp1_;
      scalar cExp2_;
      scalar cExp3_;
      scalar weConst_;
      scalar weCrit1_;
      scalar weCrit2_;
      scalar coeffD_;
      scalar onExpD_;
      scalar weExpD_;
      scalar mu_;
      scalar sigma_;
      scalar d32Coeff_;
      scalar cDmaxBM_;
      scalar cDmaxS_;
      scalar corePerc_;
public:
  //- Runtime type information
  TYPE_NAME("SHF");
  // Constructors
    //- Construct from dictionary
    SHF(const dictionary&, CloudType&);
    //- Construct copy
    SHF(const SHF<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType>> clone() const
    {
      return
        autoPtr<BreakupModel<CloudType>>
        {
          new SHF<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~SHF();
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

#include "_shf.ipp"

#endif
