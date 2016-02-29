#ifndef RANDOM_PROCESSES_UO_PROCESS_HPP_
#define RANDOM_PROCESSES_UO_PROCESS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UOprocess
// Description
//   Random UO process.
// SourceFiles
//   uo_process.cpp
#include "complex_fields.hpp"
#include "scalar.hpp"
#include "random.hpp"
namespace mousse
{
class Kmesh;
class dictionary;
class UOprocess
{
  // Private data
    Random GaussGen;
    const Kmesh& Mesh;
    const scalar DeltaT, RootDeltaT;
    complexVectorField UOfield;
    scalar Alpha;
    scalar Sigma;
    scalar Kupper;
    scalar Klower;
    scalar Scale;
  // Private Member Functions
    complexVector WeinerProcess();
public:
  // Constructors
    //- Construct from wavenumber mesh and timestep
    UOprocess
    (
      const Kmesh& kmesh,
      const scalar deltaT,
      const dictionary&
    );
  // Member Functions
    const complexVectorField& newField();
};
}  // namespace mousse
#endif
