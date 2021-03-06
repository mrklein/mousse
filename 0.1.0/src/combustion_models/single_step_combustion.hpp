#ifndef COMBUSTION_MODELS_SINGLE_STEP_COMBUSTION_HPP_
#define COMBUSTION_MODELS_SINGLE_STEP_COMBUSTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::singleStepCombustion
// Description
//   Base class for combustion models using singleStepReactingMixture.

#include "single_step_reacting_mixture.hpp"


namespace mousse {
namespace combustionModels {

template<class CombThermoType, class ThermoType>
class singleStepCombustion
:
  public CombThermoType
{
protected:
  // Protected data
    //- Pointer to singleStepReactingMixture mixture
    singleStepReactingMixture<ThermoType>* singleMixturePtr_;
    //- Fuel consumption rate
    volScalarField wFuel_;
    //- Semi-implicit (true) or explicit (false) treatment
    bool semiImplicit_;
public:
  // Constructors
    //- Construct from components
    singleStepCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Disallow copy construct
    singleStepCombustion(const singleStepCombustion&) = delete;
    //- Disallow default bitwise assignment
    singleStepCombustion& operator=(const singleStepCombustion&) = delete;
  //- Destructor
  virtual ~singleStepCombustion();
  // Member Functions
    // Evolution
      //- Fuel consumption rate matrix
      virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;
      //- Heat release rate calculated from fuel consumption rate matrix
      virtual tmp<volScalarField> dQ() const;
      //- Sensible enthalpy source term
      virtual tmp<volScalarField> Sh() const;
    // IO
      //- Update properties from given dictionary
      virtual bool read();
};

}  // namespace combustionModels
}  // namespace mousse

#include "single_step_combustion.ipp"

#endif
