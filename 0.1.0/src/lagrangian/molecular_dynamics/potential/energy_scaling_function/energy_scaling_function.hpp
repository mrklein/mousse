#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ENERGY_SCALING_FUNCTION_ENERGY_SCALING_FUNCTION_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_POTENTIAL_ENERGY_SCALING_FUNCTION_ENERGY_SCALING_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunction

#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "pair_potential.hpp"


namespace mousse {

class energyScalingFunction
{
protected:
  // Protected data
    word name_;
    dictionary energyScalingFunctionProperties_;
    const pairPotential& pairPot_;
public:
  //- Runtime type information
  TYPE_NAME("energyScalingFunction");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      energyScalingFunction,
      dictionary,
      (
        const word& name,
        const dictionary& energyScalingFunctionProperties,
        const pairPotential& pairPot
      ),
      (name, energyScalingFunctionProperties, pairPot)
    );
  // Selectors
    //- Return a reference to the selected viscosity model
    static autoPtr<energyScalingFunction> New
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
  // Constructors
    //- Construct from components
    energyScalingFunction
    (
      const word& name,
      const dictionary& energyScalingFunctionProperties,
      const pairPotential& pairPot
    );
    //- Disallow copy construct
    energyScalingFunction(const energyScalingFunction&) = delete;
    //- Disallow default bitwise assignment
    energyScalingFunction& operator=(const energyScalingFunction&) = delete;
  //- Destructor
  virtual ~energyScalingFunction()
  {}
  // Member Functions
    virtual void scaleEnergy(scalar& e, const scalar r) const = 0;
    const dictionary& energyScalingFunctionProperties() const
    {
      return energyScalingFunctionProperties_;
    }
    //- Read energyScalingFunction dictionary
    virtual bool read
    (
      const dictionary& energyScalingFunctionProperties
    ) = 0;
};

}  // namespace mousse

#endif

