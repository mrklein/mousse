// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoAtomization
// Description
//   Dummy phase change model for 'none'
#ifndef _no_atomization_hpp_
#define _no_atomization_hpp_

#include "_atomization_model.hpp"

namespace mousse
{
template<class CloudType>
class NoAtomization
:
  public AtomizationModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoAtomization(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    NoAtomization(const NoAtomization<CloudType>& am);
    //- Construct and return a clone
    virtual autoPtr<AtomizationModel<CloudType> > clone() const
    {
      return autoPtr<AtomizationModel<CloudType> >
      (
        new NoAtomization<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoAtomization();
  // Member Functions
    //- Flag to indicate whether model activates atomization model
    virtual bool active() const;
    //- Initial value of liquidCore
    virtual scalar initLiquidCore() const;
    //- Flag to indicate if chi needs to be calculated
    virtual bool calcChi() const;
    virtual void update
    (
      const scalar dt,
      scalar& d,
      scalar& liquidCore,
      scalar& tc,
      const scalar rho,
      const scalar mu,
      const scalar sigma,
      const scalar volFlowRate,
      const scalar rhoAv,
      const scalar Urel,
      const vector& pos,
      const vector& injectionPos,
      const scalar pAmbient,
      const scalar chi,
      cachedRandom& rndGen
    ) const;
};
}  // namespace mousse

#ifdef NoRepository
#   include "_no_atomization.cpp"
#endif
#endif
