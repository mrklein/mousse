// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TAB
// Description
//   The TAB Method for Numerical Calculation of Spray Droplet Breakup.
//   @verbatim
//     O'Rourke, P.J. and Amsden, A.A.,
//     "The TAB Method for Numerical Calculation of Spray Droplet Breakup,"
//     1987 SAE International Fuels and Lubricants Meeting and Exposition,
//     Toronto, Ontario, November 2-5, 1987,
//     Los Alamos National Laboratory document LA-UR-87-2105;
//     SAE Technical Paper Series, Paper 872089.
//   @endverbatim
//   This implementation follows the kiva version.
// See Also
//   The Enhanced %TAB model - ETAB

#ifndef tab_hpp_
#define tab_hpp_

#include "_breakup_model.hpp"
#include "fixed_list.hpp"

namespace mousse
{
template<class CloudType>
class TAB
:
  public BreakupModel<CloudType>
{
public:
  //- Enumeration for the SMD brekup calculation
  enum SMDMethods
  {
    method1,
    method2
  };
private:
  // Private data
    // Inverse function approximation of the Rossin-Rammler Distribution
    // used when calculating the droplet size after breakup
    FixedList<scalar, 100> rrd_;
    // Model constants
      word SMDCalcMethod_;
      SMDMethods SMDMethod_;
public:
  //- Runtime type information
  TYPE_NAME("TAB");
  // Constructors
    //- Construct from dictionary
    TAB(const dictionary& dict, CloudType& owner);
    //- Construct copy
    TAB(const TAB<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<BreakupModel<CloudType> > clone() const
    {
      return autoPtr<BreakupModel<CloudType> >
      (
        new TAB<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~TAB();
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

#ifdef NoRepository
#   include "_tab.cpp"
#endif
#endif
