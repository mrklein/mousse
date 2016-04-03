// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_point_data.hpp"


namespace mousse {

// Friend Operators 
template<class Type>
Ostream& operator<<
(
  Ostream& os,
  const wallPointData<Type>& wDist
)
{
  return os
    << static_cast<const wallPoint&>(wDist)
    << token::SPACE
    << wDist.data();
}


template<class Type>
Istream& operator>>
(
  Istream& is,
  wallPointData<Type>& wDist
)
{
  return is >> static_cast<wallPoint&>(wDist) >> wDist.data_;
}

}  // namespace mousse
