#ifndef SAMPLING_SAMPLED_SURFACE_WRITERS_ENSIGHT_PTRAITS_HPP_
#define SAMPLING_SAMPLED_SURFACE_WRITERS_ENSIGHT_PTRAITS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightPTraits
// Description
//   Conversion of OpenFOAM pTraits into the Ensight equivalent

#include "ptraits.hpp"
#include "field_types.hpp"


namespace mousse {

template<class PrimitiveType>
class ensightPTraits
{
public:
  // Static data members
    static const char* const typeName;
};


template<>
class ensightPTraits<scalar>
{
public:
  static const char* const typeName;
};


template<>
class ensightPTraits<vector>
{
public:
  static const char* const typeName;
};


template<>
class ensightPTraits<sphericalTensor>
{
public:
  static const char* const typeName;
};


template<>
class ensightPTraits<symmTensor>
{
public:
  static const char* const typeName;
};


template<>
class ensightPTraits<tensor>
{
public:
  static const char* const typeName;
};

}  // namespace mousse

#endif

