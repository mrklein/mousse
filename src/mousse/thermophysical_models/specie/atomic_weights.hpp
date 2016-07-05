#ifndef THERMOPHYSICAL_MODELS_SPECIE_ATOMIC_WEIGHTS_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_ATOMIC_WEIGHTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::atomicWeightTable
// Description
//   A table of atomic weights for all the elements

#include "scalar.hpp"
#include "hash_table.hpp"


namespace mousse {

class atomicWeightTable
:
  public HashTable<scalar>
{
public:
  // Public types and data
    //- Structure to hold the element name and atomic weight pair
    struct atomicWeight
    {
      char name[3];
      scalar weight;
    };
    static const int nElements = 104;
    //- Static table of the weights of all known elements
    static const atomicWeight atomicWeights[nElements];
  // Constructors
    //- Construct from atomicWeights_
    atomicWeightTable();
};

// Global data 
// Atomic weights table for every element in the periodic table
extern atomicWeightTable atomicWeights;

}  // namespace mousse

#endif

