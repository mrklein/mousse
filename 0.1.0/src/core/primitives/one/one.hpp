// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::one
// Description
//   A class representing the concept of 1 (scalar(1.0)) used to avoid
//   unnecessary manipulations for objects which are known to be one at
//   compile-time.
#ifndef one_hpp_
#define one_hpp_
namespace mousse
{
class one
{
public:
  // Constructors
    //- Construct null
    one()
    {}
};
}  // namespace mousse
#include "one_i.hpp"
#endif
