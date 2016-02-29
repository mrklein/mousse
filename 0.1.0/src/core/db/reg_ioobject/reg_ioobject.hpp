#ifndef CORE_DB_REG_IOOBJECT_REG_IOOBJECT_HPP_
#define CORE_DB_REG_IOOBJECT_REG_IOOBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regIOobject
// Description
//   regIOobject is an abstract class derived from IOobject to handle
//   automatic object registration with the objectRegistry.
// SourceFiles
//   reg_ioobject.cpp
//   reg_ioobject_read.cpp
//   reg_ioobject_write.cpp


#include "ioobject.hpp"
#include "type_info.hpp"
#include "os_specific.hpp"
#include "named_enum.hpp"

namespace mousse
{
namespace functionEntries
{
  class codeStream;
}
class regIOobject
:
  public IOobject
{
public:
  //- Types of communications
  enum fileCheckTypes
  {
    timeStamp,
    timeStampMaster,
    inotify,
    inotifyMaster
  };
  static const NamedEnum<fileCheckTypes, 4> fileCheckTypesNames;
protected:
    //- To flag master-only reading of objects
    static bool masterOnlyReading;
private:
  // Private data
    //- Is this object registered with the registry
    bool registered_;
    //- Is this object owned by the registry
    bool ownedByRegistry_;
    //- Modification watch index
    mutable label watchIndex_;
    //- eventNo of last update
    label eventNo_;
    //- Istream for reading
    Istream* isPtr_;
  // Private Member Functions
    //- Return Istream
    Istream& readStream();
    //- Dissallow assignment
    void operator=(const regIOobject&);
public:
    //- Declare friendship with any classes that need access to
    //  masterOnlyReading
    friend class functionEntries::codeStream;
  // Static data
    //- Runtime type information
    TYPE_NAME("regIOobject");
    static int fileModificationSkew;
    static fileCheckTypes fileModificationChecking;
  // Constructors
    //- Construct from IOobject. Optional flag for if IOobject is the
    //  top level regIOobject.
    regIOobject(const IOobject&, const bool isTime = false);
    //- Construct as copy
    regIOobject(const regIOobject&);
    //- Construct as copy, transferring registry registration to copy
    //  if registerCopy is true
    regIOobject(const regIOobject&, bool registerCopy);
    //- Construct as copy with new name, transfering registry registration
    //  to copy as specified
    regIOobject(const word& newName, const regIOobject&, bool registerCopy);
    //- Construct as copy with new IO parameters
    regIOobject(const IOobject&, const regIOobject&);
  //- Destructor
  virtual ~regIOobject();
  // Member functions
    // Registration
      //- Add object to registry
      bool checkIn();
      //- Remove object from registry
      bool checkOut();
      //- Is this object owned by the registry?
      inline bool ownedByRegistry() const;
      //- Transfer ownership of this object to its registry
      inline void store();
      //- Transfer ownership of the given object pointer to its registry
      //  and return reference to object.
      template<class Type>
      inline static Type& store(Type*);
      //- Transfer ownership of the given object pointer to its registry
      //  and return reference to object.
      template<class Type>
      inline static Type& store(autoPtr<Type>&);
      //- Release ownership of this object from its registry
      inline void release();
    // Dependency checking
      //- Event number at last update.
      inline label eventNo() const;
      //- Event number at last update.
      inline label& eventNo();
      //- Return true if up-to-date with respect to given object
      //  otherwise false
      bool upToDate(const regIOobject&) const;
      //- Return true if up-to-date with respect to given objects
      //  otherwise false
      bool upToDate
      (
        const regIOobject&,
        const regIOobject&
      ) const;
      //- Return true if up-to-date with respect to given objects
      //  otherwise false
      bool upToDate
      (
        const regIOobject&,
        const regIOobject&,
        const regIOobject&
      ) const;
      //- Return true if up-to-date with respect to given objects
      //  otherwise false
      bool upToDate
      (
        const regIOobject&,
        const regIOobject&,
        const regIOobject&,
        const regIOobject&
      ) const;
      //- Set up to date (obviously)
      void setUpToDate();
    // Edit
      //- Rename
      virtual void rename(const word& newName);
    // Reading
      //- Return Istream and check object type against that given
      Istream& readStream(const word&);
      //- Close Istream
      void close();
      //- Virtual readData function.
      //  Must be defined in derived types for which
      //  re-reading is required
      virtual bool readData(Istream&);
      //- Read object
      virtual bool read();
      //- Return file-monitoring handle
      inline label watchIndex() const;
      //- Return file-monitoring handle
      inline label& watchIndex();
      //- Return true if the object's file (or files for objectRegistry)
      //  have been modified. (modified state is cached by Time)
      virtual bool modified() const;
      //- Read object if modified (as set by call to modified)
      virtual bool readIfModified();
    // Writing
      //- Pure virtual writaData function.
      //  Must be defined in derived types
      virtual bool writeData(Ostream&) const = 0;
      //- Write using given format, version and compression
      virtual bool writeObject
      (
        IOstream::streamFormat,
        IOstream::versionNumber,
        IOstream::compressionType
      ) const;
      //- Write using setting from DB
      virtual bool write() const;
  // Member operators
    void operator=(const IOobject&);
};
}  // namespace mousse

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

// #include "reg_ioobject_i.hpp"

#endif
