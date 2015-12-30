// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyScalingFunction
// Description
// SourceFiles
//   energy_scaling_function.cpp
//   energy_scaling_function_new.cpp
#ifndef energy_scaling_function_hpp_
#define energy_scaling_function_hpp_
#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "pair_potential.hpp"
namespace mousse
{
class energyScalingFunction
{
protected:
  // Protected data
    word name_;
    dictionary energyScalingFunctionProperties_;
    const pairPotential& pairPot_;
  // Private Member Functions
    //- Disallow copy construct
    energyScalingFunction(const energyScalingFunction&);
    //- Disallow default bitwise assignment
    void operator=(const energyScalingFunction&);
public:
  //- Runtime type information
  TypeName("energyScalingFunction");
  // Declare run-time constructor selection table
    declareRunTimeSelectionTable
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
