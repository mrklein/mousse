// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cubeRootVolDelta
// Description
//   Simple cube-root of cell volume delta used in LES models.
// SourceFiles
//   cube_root_vol_delta.cpp
#ifndef cube_root_vol_delta_hpp_
#define cube_root_vol_delta_hpp_
#include "les_delta.hpp"
namespace mousse
{
namespace LESModels
{
class cubeRootVolDelta
:
  public LESdelta
{
  // Private data
    scalar deltaCoeff_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    cubeRootVolDelta(const cubeRootVolDelta&);
    void operator=(const cubeRootVolDelta&);
    // Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TypeName("cubeRootVol");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    cubeRootVolDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
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
