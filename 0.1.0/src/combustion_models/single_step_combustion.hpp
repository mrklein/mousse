// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::singleStepCombustion
// Description
//   Base class for combustion models using singleStepReactingMixture.
// SourceFiles
//   single_step_combustion.cpp
#ifndef single_step_combustion_hpp_
#define single_step_combustion_hpp_
#include "single_step_reacting_mixture.hpp"
namespace mousse
{
namespace combustionModels
{
template<class CombThermoType, class ThermoType>
class singleStepCombustion
:
  public CombThermoType
{
  // Private Member Functions
    //- Disallow copy construct
    singleStepCombustion(const singleStepCombustion&);
    //- Disallow default bitwise assignment
    void operator=(const singleStepCombustion&);
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
#ifdef NoRepository
#   include "single_step_combustion.cpp"
#endif
#endif
