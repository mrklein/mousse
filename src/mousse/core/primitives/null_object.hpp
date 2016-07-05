#ifndef CORE_PRIMITIVES_NULL_OBJECT_NULL_OBJECT_HPP_
#define CORE_PRIMITIVES_NULL_OBJECT_NULL_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nullObject
// Description
//   Singleton null-object class and instance
// SourceFiles
//   null_object.cpp


namespace mousse {

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


template <class T>
inline const T& mousse::NullObjectRef()
{
  return *reinterpret_cast<const T*>(nullObjectPtr);
}


template <class T>
inline const T* mousse::NullObjectPtr()
{
  return reinterpret_cast<const T*>(nullObjectPtr);
}


template <class T>
inline bool mousse::isNull(const T& t)
{
  return &t == NullObjectPtr<T>();
}


template <class T>
inline bool mousse::notNull(const T& t)
{
  return &t != NullObjectPtr<T>();
}


template <class T>
inline bool mousse::isNull(const T* t)
{
  return t == NullObjectPtr<T>();
}


template <class T>
inline bool mousse::notNull(const T* t)
{
  return t != NullObjectPtr<T>();
}

#endif
