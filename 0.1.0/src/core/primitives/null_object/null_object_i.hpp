// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
