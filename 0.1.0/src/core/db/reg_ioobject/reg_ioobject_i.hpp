// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline bool mousse::regIOobject::ownedByRegistry() const
{
  return ownedByRegistry_;
}


inline void mousse::regIOobject::store()
{
  ownedByRegistry_ = true;
}


template<class Type>
inline Type& mousse::regIOobject::store(Type* tPtr)
{
  if (!tPtr)
  {
    FATAL_ERROR_IN("Type& regIOobject::store(Type*)")
      << "object deallocated"
      << abort(FatalError);
  }
  tPtr->regIOobject::ownedByRegistry_ = true;
  return *tPtr;
}


template<class Type>
inline Type& mousse::regIOobject::store(autoPtr<Type>& atPtr)
{
  Type* tPtr = atPtr.ptr();
  if (!tPtr)
  {
    FATAL_ERROR_IN("Type& regIOobject::store(autoPtr<Type>&)")
      << "object deallocated"
      << abort(FatalError);
  }
  tPtr->regIOobject::ownedByRegistry_ = true;
  return *tPtr;
}


inline void mousse::regIOobject::release()
{
  ownedByRegistry_ = false;
}


inline mousse::label mousse::regIOobject::eventNo() const
{
  return eventNo_;
}


inline mousse::label& mousse::regIOobject::eventNo()
{
  return eventNo_;
}


inline mousse::label mousse::regIOobject::watchIndex() const
{
  return watchIndex_;
}


inline mousse::label& mousse::regIOobject::watchIndex()
{
  return watchIndex_;
}
