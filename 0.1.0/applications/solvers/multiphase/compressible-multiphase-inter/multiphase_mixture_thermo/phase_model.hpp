// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseModel
// Description
//   Single incompressible phase derived from the phase-fraction.
//   Used as part of the multiPhaseMixture for interface-capturing multi-phase
//   simulations.
// SourceFiles
//   phase_model.cpp
#ifndef PHASE_MODEL_HPP_
#define PHASE_MODEL_HPP_
#include "rho_thermo.hpp"
#include "vol_fields.hpp"
#include "dictionary_entry.hpp"
namespace mousse
{
class phaseModel
:
  public volScalarField
{
  // Private data
    word name_;
    const volScalarField& p_;
    const volScalarField& T_;
    autoPtr<rhoThermo> thermo_;
    volScalarField dgdt_;
public:
  // Constructors
    //- Construct from components
    phaseModel
    (
      const word& phaseName,
      const volScalarField& p,
      const volScalarField& T
    );
    //- Return clone
    autoPtr<phaseModel> clone() const;
    //- Return a pointer to a new phaseModel created on freestore
    //  from Istream
    class iNew
    {
      const volScalarField& p_;
      const volScalarField& T_;
    public:
      iNew
      (
        const volScalarField& p,
        const volScalarField& T
      )
      :
        p_{p},
        T_{T}
      {}
      autoPtr<phaseModel> operator()(Istream& is) const
      {
        return autoPtr<phaseModel>{new phaseModel{is, p_, T_}};
      }
    };
  // Member Functions
    const word& name() const
    {
      return name_;
    }
    const word& keyword() const
    {
      return name();
    }
    //- Return const-access to phase rhoThermo
    const rhoThermo& thermo() const
    {
      return thermo_();
    }
    //- Return access to phase rhoThermo
    rhoThermo& thermo()
    {
      return thermo_();
    }
    //- Return const-access to phase divergence
    const volScalarField& dgdt() const
    {
      return dgdt_;
    }
    //- Return access to phase divergence
    volScalarField& dgdt()
    {
      return dgdt_;
    }
    void correct();
};
}  // namespace mousse
#endif
