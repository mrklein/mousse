// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zero
// Description
//   A class representing the concept of 0 used to avoid unnecessary
//   manipulations for objects that are known to be zero at compile-time.
// SourceFiles
//   zero_i.hpp
#ifndef zero_hpp_
#define zero_hpp_
namespace mousse
{
class zero
{
public:
  zero()
  {}
};
}  // namespace mousse
#   include "zero_i.hpp"
#endif
