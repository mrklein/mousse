// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nullObject
// Description
//   Singleton null-object class and instance
// SourceFiles
//   null_object_i.hpp
//   null_object.cpp
#ifndef null_object_hpp_
#define null_object_hpp_
namespace mousse
{
class NullObject
{
  //- Private constructor
  NullObject()
  {}
  //- Prevent copy-construction
  NullObject(const NullObject&);
  //- Prevent assignment
  NullObject& operator=(const NullObject&);
public:
  //- The unique null object
  static const NullObject nullObject;
};
//- Pointer to the unique nullObject
extern const NullObject* nullObjectPtr;
//- Return reference to the nullObject of type T
template <class T>
inline const T& NullObjectRef();
//- Return pointer to the nullObject of type T
template <class T>
inline const T* NullObjectPtr();
//- Return true if t is a reference to the nullObject of type T
template <class T>
inline bool isNull(const T& t);
//- Return true if t is not a reference to the nullObject of type T
template <class T>
inline bool notNull(const T& t);
//- Return true if t is a pointer to the nullObject of type T
template <class T>
inline bool isNull(const T* t);
//- Return true if t is not a pointer to the nullObject of type T
template <class T>
inline bool notNull(const T* t);
}  // namespace mousse
#include "null_object_i.hpp"
#endif
