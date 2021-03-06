#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_DELTAS_CUBE_ROOT_VOL_DELTA_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_DELTAS_CUBE_ROOT_VOL_DELTA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cubeRootVolDelta
// Description
//   Simple cube-root of cell volume delta used in LES models.

#include "les_delta.hpp"


namespace mousse {
namespace LESModels {

class cubeRootVolDelta
:
  public LESdelta
{
  // Private data
    scalar deltaCoeff_;
  // Private Member Functions
    // Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TYPE_NAME("cubeRootVol");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    cubeRootVolDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
    //- Disallow default bitwise copy construct and assignment
    cubeRootVolDelta(const cubeRootVolDelta&) = delete;
    cubeRootVolDelta& operator=(const cubeRootVolDelta&) = delete;
  //- Destructor
  virtual ~cubeRootVolDelta()
  {}
  // Member Functions
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&);
    // Correct values
    virtual void correct();
};

}  // namespace LESModels
}  // namespace mousse

#endif

