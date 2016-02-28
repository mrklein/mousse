// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phase
// Description
//   Single incompressible phase derived from the phase-fraction.
//   Used as part of the multiPhaseMixture for interface-capturing multi-phase
//   simulations.
// SourceFiles
//   phase.cpp
#ifndef PHASE_HPP_
#define PHASE_HPP_
#include "vol_fields.hpp"
#include "dictionary_entry.hpp"
#include "incompressible/viscosity_model.hpp"
namespace mousse
{
class phase
:
  public volScalarField
{
  // Private data
    word name_;
    dictionary phaseDict_;
    autoPtr<viscosityModel> nuModel_;
    dimensionedScalar rho_;
public:
  // Constructors
    //- Construct from components
    phase
    (
      const word& name,
      const dictionary& phaseDict,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
    //- Return clone
    autoPtr<phase> clone() const;
    //- Return a pointer to a new phase created on freestore
    //  from Istream
    class iNew
    {
      const volVectorField& U_;
      const surfaceScalarField& phi_;
    public:
      iNew
      (
        const volVectorField& U,
        const surfaceScalarField& phi
      )
      :
        U_{U},
        phi_{phi}
      {}
      autoPtr<phase> operator()(Istream& is) const
      {
        dictionaryEntry ent{dictionary::null, is};
        return autoPtr<phase>{new phase{ent.keyword(), ent, U_, phi_}};
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
    //- Return const-access to phase1 viscosityModel
    const viscosityModel& nuModel() const
    {
      return nuModel_();
    }
    //- Return the kinematic laminar viscosity
    tmp<volScalarField> nu() const
    {
      return nuModel_->nu();
    }
    //- Return the laminar viscosity for patch
    tmp<scalarField> nu(const label patchi) const
    {
      return nuModel_->nu(patchi);
    }
    //- Return const-access to phase1 density
    const dimensionedScalar& rho() const
    {
      return rho_;
    }
    //- Correct the phase properties
    void correct();
    //-Inherit read from volScalarField
    using volScalarField::read;
    //- Read base transportProperties dictionary
    bool read(const dictionary& phaseDict);
};
}  // namespace mousse
#endif
