#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_HEHEU_REACTION_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_HEHEU_REACTION_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heheuReactionThermo
// Description
//   mousse::heheuReactionThermo

#include "he_thermo.hpp"


namespace mousse {

template<class BasicPsiThermo, class MixtureType>
class heheuPsiThermo
:
  public heThermo<BasicPsiThermo, MixtureType>
{
  // Private Member Functions
    volScalarField Tu_;
    volScalarField heu_;
  // Private Member Functions
    void calculate();
public:
  //- Runtime type information
  TYPE_NAME("heheuPsiThermo");
  // Constructors
    //- Construct from mesh and phase name
    heheuPsiThermo
    (
      const fvMesh&,
      const word& phaseName
    );
    //- Disable construct as copy
    heheuPsiThermo
    (
      const heheuPsiThermo<BasicPsiThermo, MixtureType>&
    ) = delete;
  //- Destructor
  virtual ~heheuPsiThermo();
  // Member functions
    //- Update properties
    virtual void correct();
    // Access to thermodynamic state variables.
      //- Unburnt gas enthalpy [J/kg]
      //  Non-const access allowed for transport equations
      virtual volScalarField& heu()
      {
        return heu_;
      }
      //- Unburnt gas enthalpy [J/kg]
      virtual const volScalarField& heu() const
      {
        return heu_;
      }
      //- Unburnt gas temperature [K]
      virtual const volScalarField& Tu() const
      {
        return Tu_;
      }
    // Fields derived from thermodynamic state variables
      //- Unburnt gas enthalpy for cell-set [J/kg]
      virtual tmp<scalarField> heu
      (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
      ) const;
      //- Unburnt gas enthalpy for patch [J/kg]
      virtual tmp<scalarField> heu
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Burnt gas temperature [K]
      virtual tmp<volScalarField> Tb() const;
      //- Unburnt gas compressibility [s^2/m^2]
      virtual tmp<volScalarField> psiu() const;
      //- Burnt gas compressibility [s^2/m^2]
      virtual tmp<volScalarField> psib() const;
    // Access to transport variables
      //- Dynamic viscosity of unburnt gas [kg/ms]
      virtual tmp<volScalarField> muu() const;
      //- Dynamic viscosity of burnt gas [kg/ms]
      virtual tmp<volScalarField> mub() const;
};

}  // namespace mousse

#include "heheu_psi_thermo.ipp"

#endif
