#ifndef CORE_PRIMITIVES_RANDOM_RANDOM_HPP_
#define CORE_PRIMITIVES_RANDOM_RANDOM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Random
// Description
//   Simple random number generator.
// SourceFiles
//   random.cpp

#include "vector.hpp"
#include "tensor.hpp"


namespace mousse {

class Random
{
  // Private data
    label Seed;
public:
  // Constructors
    //- Construct given seed
    Random(const label);
  // Member functions
    int bit();
    //- Scalar [0..1] (so including 0,1)
    scalar scalar01();
    //- Vector with every component scalar01
    vector vector01();
    //- sphericalTensor with every component scalar01
    sphericalTensor sphericalTensor01();
    //- symmTensor with every component scalar01
    symmTensor symmTensor01();
    //- Tensor with every component scalar01
    tensor tensor01();
    //- Label [lower..upper]
    label integer(const label lower, const label upper);
    vector position(const vector&, const vector&);
    void randomise(scalar&);
    void randomise(vector&);
    void randomise(sphericalTensor&);
    void randomise(symmTensor&);
    void randomise(tensor&);
    //- Return a normal Gaussian randon number
    //  with zero mean and unity variance N(0, 1)
    scalar GaussNormal();
};
}  // namespace mousse
#endif
