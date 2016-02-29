#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_PSIU_REACTION_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_PSIU_REACTION_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::psiuReactionThermo
// Description
//   mousse::psiuReactionThermo
// SourceFiles
//   psiu_reaction_thermo.cpp
//   psiu_reaction_thermo_new.cpp
#include "psi_reaction_thermo.hpp"
namespace mousse
{
class psiuReactionThermo
:
  public psiReactionThermo
{
protected:
  // Protected Member Functions
    wordList heuBoundaryTypes();
    void heuBoundaryCorrection(volScalarField& heu);
public:
  //- Runtime type information
  TYPE_NAME("psiuReactionThermo");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      psiuReactionThermo,
      fvMesh,
      (const fvMesh& mesh, const word& phaseName),
      (mesh, phaseName)
    );
  // Constructors
    //- Construct from mesh and phase name
    psiuReactionThermo
    (
      const fvMesh&,
      const word& phaseName
    );
  // Selectors
    static autoPtr<psiuReactionThermo> New
    (
      const fvMesh&,
      const word& phaseName=word::null
    );
  //- Destructor
  virtual ~psiuReactionThermo();
  // Member functions
    //- Update properties
    virtual void correct() = 0;
    // Access to thermodynamic state variables.
      //- Unburnt gas enthalpy [J/kg]
      //  Non-const access allowed for transport equations
      virtual volScalarField& heu() = 0;
      //- Unburnt gas enthalpy [J/kg]
      virtual const volScalarField& heu() const = 0;
    // Fields derived from thermodynamic state variables
      //- Unburnt gas enthalpy for cell-set [J/kg]
      virtual tmp<scalarField> heu
      (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
      ) const = 0;
      //- Unburnt gas enthalpy for patch [J/kg]
      virtual tmp<scalarField> heu
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Unburnt gas temperature [K]
      virtual const volScalarField& Tu() const = 0;
      //- Burnt gas temperature [K]
      virtual tmp<volScalarField> Tb() const = 0;
      //- Unburnt gas density [kg/m^3]
      virtual tmp<volScalarField> rhou() const
      {
        return p_*psiu();
      }
      //- Burnt gas density [kg/m^3]
      virtual tmp<volScalarField> rhob() const
      {
        return p_*psib();
      }
      //- Unburnt gas compressibility [s^2/m^2]
      virtual tmp<volScalarField> psiu() const = 0;
      //- Burnt gas compressibility [s^2/m^2]
      virtual tmp<volScalarField> psib() const = 0;
      //- Dynamic viscosity of unburnt gas [kg/ms]
      virtual tmp<volScalarField> muu() const = 0;
      //- Dynamic viscosity of burnt gas [kg/ms]
      virtual tmp<volScalarField> mub() const = 0;
};
}  // namespace mousse
#endif
