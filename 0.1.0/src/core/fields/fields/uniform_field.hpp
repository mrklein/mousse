// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UniformField
// Description
//   A class representing the concept of a uniform field which stores only
//   the single value and providing the operator[] to return it.
#ifndef uniform_field_hpp_
#define uniform_field_hpp_
#include "label.hpp"
namespace mousse
{
template<class Type>
class UniformField
{
  // Private data
    Type value_;
public:
  // Constructors
    //- Construct given value
    inline UniformField(const Type& value);
  // Member Operators
    inline Type operator[](const label) const;
    inline UniformField field() const;
};
}  // namespace mousse
#include "uniform_field_i.hpp"
#endif
