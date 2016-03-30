// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "object_registry.hpp"
#include "string_list_ops.hpp"


// Member Functions 
template<class Type>
mousse::wordList mousse::objectRegistry::names() const
{
  wordList objectNames{size()};
  label count=0;
  FOR_ALL_CONST_ITER(HashTable<regIOobject*>, *this, iter) {
    if (isA<Type>(*iter())) {
      objectNames[count++] = iter()->name();
    }
  }
  objectNames.setSize(count);
  return objectNames;
}


template<class Type>
mousse::wordList mousse::objectRegistry::names(const wordRe& name) const
{
  wordList objectNames{size()};
  label count = 0;
  FOR_ALL_CONST_ITER(HashTable<regIOobject*>, *this, iter) {
    if (isA<Type>(*iter())) {
      const word& objectName = iter()->name();
      if (name.match(objectName)) {
        objectNames[count++] = objectName;
      }
    }
  }
  objectNames.setSize(count);
  return objectNames;
}


template<class Type>
mousse::wordList mousse::objectRegistry::names(const wordReList& patterns) const
{
  wordList names{this->names<Type>()};
  return wordList{names, findStrings(patterns, names)};
}


template<class Type>
mousse::HashTable<const Type*> mousse::objectRegistry::lookupClass
(
  const bool strict
) const
{
  HashTable<const Type*> objectsOfClass(size());
  FOR_ALL_CONST_ITER(HashTable<regIOobject*>, *this, iter) {
    if ((strict && isType<Type>(*iter()))
        || (!strict && isA<Type>(*iter()))) {
      objectsOfClass.insert
      (
        iter()->name(),
        dynamic_cast<const Type*>(iter())
      );
    }
  }
  return objectsOfClass;
}


template<class Type>
mousse::HashTable<Type*> mousse::objectRegistry::lookupClass
(
  const bool strict
)
{
  HashTable<Type*> objectsOfClass{size()};
  FOR_ALL_ITER(HashTable<regIOobject*>, *this, iter) {
    if ((strict && isType<Type>(*iter()))
        || (!strict && isA<Type>(*iter()))) {
      objectsOfClass.insert
      (
        iter()->name(),
        dynamic_cast<Type*>(iter())
      );
    }
  }
  return objectsOfClass;
}


template<class Type>
bool mousse::objectRegistry::foundObject(const word& name) const
{
  const_iterator iter = find(name);
  if (iter != end()) {
    const Type* vpsiPtr_ = dynamic_cast<const Type*>(iter());
    if (vpsiPtr_) {
      return true;
    }
  } else if (this->parentNotTime()) {
    return parent_.foundObject<Type>(name);
  }
  return false;
}


template<class Type>
const Type& mousse::objectRegistry::lookupObject(const word& name) const
{
  const_iterator iter = find(name);
  if (iter != end()) {
    const Type* vpsiPtr_ = dynamic_cast<const Type*>(iter());
    if (vpsiPtr_) {
      return *vpsiPtr_;
    }
    FATAL_ERROR_IN
    (
      "objectRegistry::lookupObject<Type>(const word&) const"
    )
    << nl
    << "    lookup of " << name << " from objectRegistry "
    << this->name()
    << " successful\n    but it is not a " << Type::typeName
    << ", it is a " << iter()->type()
    << abort(FatalError);
  } else {
    if (this->parentNotTime()) {
      return parent_.lookupObject<Type>(name);
    }
    FATAL_ERROR_IN
    (
      "objectRegistry::lookupObject<Type>(const word&) const"
    )
    << nl
    << "    request for " << Type::typeName
    << " " << name << " from objectRegistry " << this->name()
    << " failed\n    available objects of type " << Type::typeName
    << " are" << nl
    << names<Type>()
    << abort(FatalError);
  }
  return NullObjectRef<Type>();
}
