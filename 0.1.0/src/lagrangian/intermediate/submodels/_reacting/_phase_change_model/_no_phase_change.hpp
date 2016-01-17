// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoPhaseChange
// Description
//   Dummy phase change model for 'none'
#ifndef _no_phase_change_hpp_
#define _no_phase_change_hpp_
#include "_phase_change_model.hpp"
namespace mousse
{
template<class CloudType>
class NoPhaseChange
:
  public PhaseChangeModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoPhaseChange(const dictionary&, CloudType&);
    //- Construct copy
    NoPhaseChange(const NoPhaseChange<CloudType>& pcm);
    //- Construct and return a clone
    virtual autoPtr<PhaseChangeModel<CloudType> > clone() const
    {
      return autoPtr<PhaseChangeModel<CloudType> >
      (
        new NoPhaseChange<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoPhaseChange();
  // Member Functions
    //- Flag to indicate whether model activates phase change model
    virtual bool active() const;
    //- Update model
    virtual void calculate
    (
      const scalar dt,
      const label cellI,
      const scalar Re,
      const scalar Pr,
      const scalar d,
      const scalar nu,
      const scalar T,
      const scalar Ts,
      const scalar pc,
      const scalar Tc,
      const scalarField& X,
      scalarField& dMassPC
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_no_phase_change.cpp"
#endif
#endif
