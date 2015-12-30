// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoBreakup
// Description
//   Dummy breakup model for 'none'

#ifndef _no_breakup_hpp_
#define _no_breakup_hpp_

#include "_breakup_model.hpp"

namespace mousse
{
template<class CloudType>
class NoBreakup
:
  public BreakupModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from dictionary
    NoBreakup(const dictionary&, CloudType&);
    //- Construct copy
    NoBreakup(const NoBreakup<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType> > clone() const
    {
      return autoPtr<BreakupModel<CloudType> >
      (
        new NoBreakup<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoBreakup();
  // Member Functions
    //- Flag to indicate whether model activates break-up model
    virtual bool active() const;
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
#   include "_no_breakup.cpp"
#endif
#endif
