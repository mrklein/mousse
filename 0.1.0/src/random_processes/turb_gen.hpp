// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbGen
// Description
//   Generate a turbulent velocity field conforming to a given
//   energy spectrum and being divergence free.
// SourceFiles
//   turb_gen.cpp
//   turb_gen_io.cpp
#ifndef turb_gen_hpp_
#define turb_gen_hpp_
#include "random.hpp"
#include "primitive_fields.hpp"
namespace mousse
{
class Kmesh;
class turbGen
{
  // Private data
    const Kmesh& K;
    const scalar Ea;
    const scalar k0;
    Random RanGen;
public:
  // Constructors
    //- Construct from components
    turbGen(const Kmesh& k, const scalar EA, const scalar K0);
  // Member Functions
    //- Generate and return a velocity field
    vectorField U();
};
}  // namespace mousse
#endif
