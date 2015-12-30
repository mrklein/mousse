// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReitzDiwakar
// Description
//   secondary breakup model
//   @verbatim
//   Reitz, R.D.
//   "Modelling atomization processes in highpressure vaporizing sprays"
//   Atomization and Spray Technology 3 (1987), 309-337
//   @endverbatim
//   @verbatim
//   Reitz, R.D. and Diwakar, R.
//   "Effect of drop breakup on fuel sprays"
//   SAE Tech. paper series, 860469 (1986)
//   @endverbatim
//   @verbatim
//   Reitz, R.D. and Diwakar, R.
//   "Structure of high-pressure fuel sprays"
//   SAE Tech. paper series, 870598 (1987)
//   @endverbatim

#ifndef reitz_diwakar_hpp_
#define reitz_diwakar_hpp_

#include "_breakup_model.hpp"

namespace mousse
{
template<class CloudType>
class ReitzDiwakar
:
  public BreakupModel<CloudType>
{
private:
  // Private data
    scalar Cbag_;
    scalar Cb_;
    scalar Cstrip_;
    scalar Cs_;
public:
  //- Runtime type information
  TypeName("ReitzDiwakar");
  // Constructors
    //- Construct from dictionary
    ReitzDiwakar(const dictionary&, CloudType&);
    //- Construct copy
    ReitzDiwakar(const ReitzDiwakar<CloudType>& bum);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType> > clone() const
    {
      return autoPtr<BreakupModel<CloudType> >
      (
        new ReitzDiwakar<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ReitzDiwakar();
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
#   include "_reitz_diwakar.cpp"
#endif
#endif
