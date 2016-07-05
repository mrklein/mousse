#ifndef CORE_DB_OBJECT_REGISTRY_OBJECT_REGISTRY_HPP_
#define CORE_DB_OBJECT_REGISTRY_OBJECT_REGISTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::objectRegistry
// Description
//   Registry of regIOobjects

#include "hash_table.hpp"
#include "reg_ioobject.hpp"
#include "word_re_list.hpp"


namespace mousse {

class objectRegistry
:
  public regIOobject,
  public HashTable<regIOobject*>
{
  // Private Data

    //- Master time objectRegistry
    const Time& time_;

    //- Parent objectRegistry
    const objectRegistry& parent_;

    //- Local directory path of this objectRegistry relative to time
    fileName dbDir_;

    //- Current event
    mutable label event_;

  // Private Member Functions

    //- Is the objectRegistry parent_ different from time_
    //  Used to terminate searching within the ancestors
    bool parentNotTime() const;

public:

  //- Declare type name for this IOobject
  TYPE_NAME("objectRegistry");

  // Constructors

    //- Construct the time objectRegistry given an initial estimate
    //  for the number of entries
    explicit objectRegistry
    (
      const Time& db,
      const label nIoObjects = 128
    );

    //- Construct a sub-registry given an IObject to describe the registry
    //  and an initial estimate for the number of entries
    explicit objectRegistry
    (
      const IOobject& io,
      const label nIoObjects = 128
    );

    //- Disallow Copy constructor
    objectRegistry(const objectRegistry&) = delete;

    //- Disallow default bitwise copy construct and assignment
    objectRegistry& operator=(const objectRegistry&) = delete;

  //- Destructor
  virtual ~objectRegistry();

  // Member functions

    // Access

      //- Return time
      const Time& time() const
      {
        return time_;
      }

      //- Return the parent objectRegistry
      const objectRegistry& parent() const
      {
        return parent_;
      }

      //- Local directory path of this objectRegistry relative to the time
      virtual const fileName& dbDir() const
      {
        return dbDir_;
      }

      //- Return the list of names of the IOobjects
      wordList names() const;

      //- Return the sorted list of names of the IOobjects
      wordList sortedNames() const;

      //- Return the list of names of IOobjects of given class name
      wordList names(const word& className) const;

      //- Return the sorted list of names of IOobjects of given class name
      wordList sortedNames(const word& className) const;

      //- Return the list of names of the IOobjects of given type
      template<class Type>
      wordList names() const;

      //- Return the list of objects whose name matches the input regExp
      template<class Type>
      wordList names(const wordRe& name) const;

      //- Return the list of objects whose name matches the input regExp
      template<class Type>
      wordList names(const wordReList& name) const;

      //- Lookup and return a const sub-objectRegistry. Optionally create
      //  it if it does not exist.
      const objectRegistry& subRegistry
      (
        const word& name,
        const bool forceCreate = false
      ) const;

      //- Lookup and return all objects of the given Type
      template<class Type>
      HashTable<const Type*> lookupClass(const bool strict = false) const;

      //- Lookup and return all objects of the given Type
      template<class Type>
      HashTable<Type*> lookupClass(const bool strict = false);

      //- Is the named Type found?
      template<class Type>
      bool foundObject(const word& name) const;

      //- Lookup and return the object of the given Type
      template<class Type>
      const Type& lookupObject(const word& name) const;

      //- Return new event number.
      label getEvent() const;

    // Edit

      //- Rename
      virtual void rename(const word& newName);

      //- Add an regIOobject to registry
      bool checkIn(regIOobject&) const;

      //- Remove an regIOobject from registry
      bool checkOut(regIOobject&) const;

    // Reading

      //- Return true if any of the object's files have been modified
      virtual bool modified() const;

      //- Read the objects that have been modified
      void readModifiedObjects();

      //- Read object if modified
      virtual bool readIfModified();

    // Writing

      //- writeData function required by regIOobject but not used
      //  for this class, write is used instead
      virtual bool writeData(Ostream&) const
      {
        NOT_IMPLEMENTED
        (
          "void objectRegistry::writeData(Ostream&) const: "
          "use write() instead"
        );
        return false;
      }

      //- Write the objects
      virtual bool writeObject
      (
        IOstream::streamFormat fmt,
        IOstream::versionNumber ver,
        IOstream::compressionType cmp
      ) const;

};

}  // namespace mousse

#include "object_registry.ipp"

#endif
