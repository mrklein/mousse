// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LiquidEvaporation
// Description
//   Liquid evaporation model
//   - uses ideal gas assumption
#ifndef _liquid_evaporation_hpp_
#define _liquid_evaporation_hpp_
#include "_phase_change_model.hpp"
#include "liquid_mixture_properties.hpp"
namespace mousse
{
template<class CloudType>
class LiquidEvaporation
:
  public PhaseChangeModel<CloudType>
{
protected:
  // Protected data
    //- Global liquid properties data
    const liquidMixtureProperties& liquids_;
    //- List of active liquid names
    List<word> activeLiquids_;
    //- Mapping between liquid and carrier species
    List<label> liqToCarrierMap_;
    //- Mapping between local and global liquid species
    List<label> liqToLiqMap_;
  // Protected Member Functions
    //- Sherwood number as a function of Reynolds and Schmidt numbers
    scalar Sh(const scalar Re, const scalar Sc) const;
    //- Calculate the carrier phase component volume fractions at cellI
    tmp<scalarField> calcXc(const label cellI) const;
public:
  //- Runtime type information
  TYPE_NAME("liquidEvaporation");
  // Constructors
    //- Construct from dictionary
    LiquidEvaporation(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    LiquidEvaporation(const LiquidEvaporation<CloudType>& pcm);
    //- Construct and return a clone
    virtual autoPtr<PhaseChangeModel<CloudType> > clone() const
    {
      return autoPtr<PhaseChangeModel<CloudType> >
      (
        new LiquidEvaporation<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~LiquidEvaporation();
  // Member Functions
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
    //- Return the enthalpy per unit mass
    virtual scalar dh
    (
      const label idc,
      const label idl,
      const scalar p,
      const scalar T
    ) const;
    //- Return vapourisation temperature
    virtual scalar Tvap(const scalarField& X) const;
    //- Return maximum/limiting temperature
    virtual scalar TMax(const scalar p, const scalarField& X) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_liquid_evaporation.cpp"
#endif
#endif
