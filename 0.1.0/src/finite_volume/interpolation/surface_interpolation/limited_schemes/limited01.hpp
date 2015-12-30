// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Limited01Limiter
// Description
//   A LimitedLimiter with the range 0-1
#ifndef limited01_hpp_
#define limited01_hpp_
#include "limited.hpp"
namespace mousse
{
template<class LimitedScheme>
class Limited01Limiter
:
  public LimitedLimiter<LimitedScheme>
{
public:
  Limited01Limiter(Istream& is)
  :
    LimitedLimiter<LimitedScheme>(0, 1, is)
  {}
};
}  // namespace mousse
#endif
