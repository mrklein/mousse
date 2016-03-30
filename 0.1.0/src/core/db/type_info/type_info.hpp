#ifndef CORE_DB_TYPE_INFO_TYPE_INFO_HPP_
#define CORE_DB_TYPE_INFO_TYPE_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "class_name.hpp"
#include "error.hpp"
#include <typeinfo>


// declarations (for use in header files)
//- Declare a ClassNameNoDebug() with extra virtual type info
#define TYPE_NAME_NO_DEBUG(TypeNameString)                                    \
  CLASS_NAME_NO_DEBUG(TypeNameString);                                        \
  virtual const word& type() const { return typeName; }

//- Declare a ClassName() with extra virtual type info
#define TYPE_NAME(TypeNameString)                                             \
  CLASS_NAME(TypeNameString);                                                 \
  virtual const word& type() const { return typeName; }

namespace mousse {

// Global Functions 
//- Reference type cast template function,
//  wraps dynamic_cast to handle bad_cast exception and generate a FatalError.
template<class To, class From>
inline To& dynamicCast(From& r)
{
  try {
    return dynamic_cast<To&>(r);
  } catch (std::bad_cast) {
    FATAL_ERROR_IN("dynamicCast<To>(From&)")
      << "Attempt to cast type " << typeid(r).name()
      << " to type " << typeid(To).name()
      << abort(FatalError);
    return dynamic_cast<To&>(r);
  }
}


//- Reference type cast template function.
//  As per dynamicCast, but handles type names via the virtual type() method.
template<class To, class From>
inline To& refCast(From& r)
{
  try {
    return dynamic_cast<To&>(r);
  } catch (std::bad_cast) {
    FATAL_ERROR_IN("refCast<To>(From&)")
      << "Attempt to cast type " << r.type()
      << " to type " << To::typeName
      << abort(FatalError);
    return dynamic_cast<To&>(r);
  }
}


//- Check the typeid
template<class TestType, class Type>
inline bool isType(const Type& t)
{
  return typeid(t) == typeid(TestType);
}


//- Check if a dynamic_cast to typeid is possible
template<class TestType, class Type>
inline bool isA(const Type& t)
{
  return dynamic_cast<const TestType*>(&t);
}

}  // namespace mousse
#endif
