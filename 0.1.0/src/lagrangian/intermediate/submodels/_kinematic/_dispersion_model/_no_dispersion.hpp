// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoDispersion
// Description
//   Place holder for 'none' option
#ifndef _no_dispersion_hpp_
#define _no_dispersion_hpp_
#include "_dispersion_model.hpp"
namespace mousse
{
template<class CloudType>
class NoDispersion
:
  public DispersionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    NoDispersion(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoDispersion(const NoDispersion<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DispersionModel<CloudType> > clone() const
    {
      return autoPtr<DispersionModel<CloudType> >
      (
        new NoDispersion<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoDispersion();
  // Member Functions
    //- Flag to indicate whether model activates injection model
    virtual bool active() const;
    //- Update (disperse particles)
    virtual vector update
    (
      const scalar dt,
      const label cellI,
      const vector& U,
      const vector& Uc,
      vector& UTurb,
      scalar& tTurb
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_dispersion.cpp"
#endif
#endif
