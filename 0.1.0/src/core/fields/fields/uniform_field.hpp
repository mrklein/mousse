#ifndef CORE_FIELDS_FIELDS_UNIFORM_FIELD_HPP_
#define CORE_FIELDS_FIELDS_UNIFORM_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UniformField
// Description
//   A class representing the concept of a uniform field which stores only
//   the single value and providing the operator[] to return it.

#include "label.hpp"


namespace mousse {

template<class Type>
class UniformField
{
  // Private data
    Type value_;
public:
  // Constructors
    //- Construct given value
    inline UniformField(const Type& value) : value_{value} {};
  // Member Operators
    inline Type operator[](const label) const { return value_; }
    inline UniformField field() const { return {value_}; }
};

}  // namespace mousse

#endif
