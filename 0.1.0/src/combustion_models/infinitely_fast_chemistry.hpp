#ifndef COMBUSTION_MODELS_INFINITELY_FAST_CHEMISTRY_HPP_
#define COMBUSTION_MODELS_INFINITELY_FAST_CHEMISTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::combustionModels::infinitelyFastChemistry
// Description
//   Simple infinitely fast chemistry combustion model based on the principle
//   mixed is burnt. Additional parameter C is used to distribute the heat
//   release rate.in time

#include "single_step_combustion.hpp"


namespace mousse {
namespace combustionModels {

template<class CombThermoType, class ThermoType>
class infinitelyFastChemistry
:
  public singleStepCombustion<CombThermoType, ThermoType>
{
  // Private data
    //- Model constant
    scalar C_;
public:
  //- Runtime type information
  TYPE_NAME("infinitelyFastChemistry");
  // Constructors
    //- Construct from components
    infinitelyFastChemistry
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
    //- Disallow copy construct
    infinitelyFastChemistry(const infinitelyFastChemistry&) = delete;
    //- Disallow default bitwise assignment
    infinitelyFastChemistry& operator=(const infinitelyFastChemistry&) = delete;
  //- Destructor
  virtual ~infinitelyFastChemistry();
  // Member Functions
    // Evolution
      //- Correct combustion rate
      virtual void correct();
    // IO
      //- Update properties
      virtual bool read();
};

}  // namespace combustionModels
}  // namespace mousse

#include "infinitely_fast_chemistry.ipp"

#endif
