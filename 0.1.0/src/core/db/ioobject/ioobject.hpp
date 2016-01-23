// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOobject
// Description
//   IOobject defines the attributes of an object for which implicit
//   objectRegistry management is supported, and provides the infrastructure
//   for performing stream I/O.
//   An IOobject is constructed with an object name, a class name, an instance
//   path, a reference to a objectRegistry, and parameters determining its
//   storage status.
//   \par Read options
//   Define what is done on object construction and explicit reads:
//   \param MUST_READ
//     Object must be read from Istream on construction. \n
//     Error if Istream does not exist or can't be read.
//     Does not check timestamp or re-read.
//   \param MUST_READ_IF_MODIFIED
//     Object must be read from Istream on construction. \n
//     Error if Istream does not exist or can't be read. If object is
//     registered its timestamp will be checked every timestep and possibly
//     re-read.
//   \param READ_IF_PRESENT
//     Read object from Istream if Istream exists, otherwise don't. \n
//     Error only if Istream exists but can't be read.
//     Does not check timestamp or re-read.
//   \param NO_READ
//      Don't read
//   \par Write options
//   Define what is done on object destruction and explicit writes:
//   \param AUTO_WRITE
//     Object is written automatically when requested to by the
//     objectRegistry.
//   \param NO_WRITE
//     No automatic write on destruction but can be written explicitly
// SourceFiles
//   ioobject.cpp
//   ioobject_read_header.cpp
//   ioobject_write_header.cpp
//   ioobject_print.cpp

#ifndef ioobject_hpp_
#define ioobject_hpp_

#include "file_name.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "info_proxy.hpp"
#include "mousse_version.hpp"

namespace mousse
{
class Time;
class objectRegistry;
class IOobject
{
public:
  // Public data types
    //- Enumeration defining the valid states of an IOobject
    enum objectState
    {
      GOOD,
      BAD
    };
    //- Enumeration defining the read options
    enum readOption
    {
      MUST_READ,
      MUST_READ_IF_MODIFIED,
      READ_IF_PRESENT,
      NO_READ
    };
    //- Enumeration defining the write options
    enum writeOption
    {
      AUTO_WRITE = 0,
      NO_WRITE = 1
    };
private:
  // Private data
    //- Name
    word name_;
    //- Class name read from header
    word headerClassName_;
    //- Optional note
    string note_;
    //- Instance path component
    fileName instance_;
    //- Local path component
    fileName local_;
    //- objectRegistry reference
    const objectRegistry& db_;
    //- Read option
    readOption rOpt_;
    //- Write option
    writeOption wOpt_;
    //- Register object created from this IOobject with registry if true
    bool registerObject_;
    //- IOobject state
    objectState objState_;
protected:
  // Protected Member Functions
    //- Construct and return an IFstream for the object.
    //  The results is NULL if the stream construction failed
    Istream* objectStream();
    //- Construct and return an IFstream for the object given the
    //  exact file. The results is NULL if the stream construction failed
    Istream* objectStream(const fileName&);
    //- Set the object state to bad
    void setBad(const string&);
public:
  //- Runtime type information
  TYPE_NAME("IOobject");
  // Static Member Functions
    //- Split path into instance, local, name components
    //  input               IOobject(instance, local, name)
    //  -----               ------
    //  "foo"               ("", "", "foo")
    //  "foo/bar"           ("foo", "", "bar")
    //  "/XXX/bar"          ("/XXX", "", "bar")
    //  "foo/bar/"          ERROR - no name
    //  "foo/xxx/bar"       ("foo", "xxx", "bar")
    //  "foo/xxx/yyy/bar"   ("foo", "xxx/yyy", "bar")
    static bool fileNameComponents
    (
      const fileName& path,
      fileName& instance,
      fileName& local,
      word& name
    );
    template<class Name>
    static inline word groupName(Name name, const word& group);
  // Constructors
    //- Construct from name, instance, registry, io options
    IOobject
    (
      const word& name,
      const fileName& instance,
      const objectRegistry& registry,
      readOption r=NO_READ,
      writeOption w=NO_WRITE,
      bool registerObject=true
    );
    //- Construct from name, instance, local, registry, io options
    IOobject
    (
      const word& name,
      const fileName& instance,
      const fileName& local,
      const objectRegistry& registry,
      readOption r=NO_READ,
      writeOption w=NO_WRITE,
      bool registerObject=true
    );
    //- Construct from path, registry, io options
    //  Uses fileNameComponents() to split path into components.
    IOobject
    (
      const fileName& path,
      const objectRegistry& registry,
      readOption r=NO_READ,
      writeOption w=NO_WRITE,
      bool registerObject=true
    );
    //- Clone
    mousse::autoPtr<IOobject> clone() const
    {
      return autoPtr<IOobject>(new IOobject(*this));
    }
  //- Destructor
  virtual ~IOobject();
  // Member Functions
    // General access
      //- Return time
      const Time& time() const;
      //- Return the local objectRegistry
      const objectRegistry& db() const;
      //- Return name
      const word& name() const
      {
        return name_;
      }
      //- Return name of the class name read from header
      const word& headerClassName() const
      {
        return headerClassName_;
      }
      //- Return non-constant access to the optional note
      string& note()
      {
        return note_;
      }
      //- Return the optional note
      const string& note() const
      {
        return note_;
      }
      //- Rename
      virtual void rename(const word& newName)
      {
        name_ = newName;
      }
      //- Register object created from this IOobject with registry if true
      bool& registerObject()
      {
        return registerObject_;
      }
      //- Register object created from this IOobject with registry if true
      bool registerObject() const
      {
        return registerObject_;
      }
    // Read/write options
      readOption readOpt() const
      {
        return rOpt_;
      }
      readOption& readOpt()
      {
        return rOpt_;
      }
      writeOption writeOpt() const
      {
        return wOpt_;
      }
      writeOption& writeOpt()
      {
        return wOpt_;
      }
    // Path components
      //- Return group (extension part of name)
      word group() const;
      //- Return member (name without the extension)
      word member() const;
      const fileName& rootPath() const;
      const fileName& caseName() const;
      const fileName& instance() const
      {
        return instance_;
      }
      fileName& instance()
      {
        return instance_;
      }
      const fileName& local() const
      {
        return local_;
      }
      //- Return complete path
      fileName path() const;
      //- Return complete path with alternative instance and local
      fileName path
      (
        const word& instance,
        const fileName& local = ""
      ) const;
      //- Return complete path + object name
      fileName objectPath() const
      {
        return path()/name();
      }
      //- Return complete path + object name if the file exists
      //  either in the case/processor or case otherwise null
      fileName filePath() const;
    // Reading
      //- Read header
      bool readHeader(Istream&);
      //- Read and check header info
      bool headerOk();
    // Writing
      //- Write the standard OpenFOAM file/dictionary banner
      //  Optionally without -*- C++ -*- editor hint (eg, for logs)
      template<class Stream>
      static inline Stream& writeBanner(Stream& os, bool noHint=false);
      //- Write the standard file section divider
      template<class Stream>
      static inline Stream& writeDivider(Stream& os);
      //- Write the standard end file divider
      template<class Stream>
      static inline Stream& writeEndDivider(Stream& os);
      //- Write header
      bool writeHeader(Ostream&) const;
      //- Write header. Allow override of type
      bool writeHeader(Ostream&, const word& objectType) const;
    // Error Handling
      bool good() const
      {
        return objState_ == GOOD;
      }
      bool bad() const
      {
        return objState_ == BAD;
      }
    // Info
      //- Return info proxy.
      //  Used to print token information to a stream
      InfoProxy<IOobject> info() const
      {
        return *this;
      }
  // Member operators
    void operator=(const IOobject&);
};
template<>
Ostream& operator<<(Ostream& os, const InfoProxy<IOobject>& ip);
}  // namespace mousse

// Member Functions 
template<class Name>
inline mousse::word mousse::IOobject::groupName(Name name, const word& group)
{
  if (group != word::null)
  {
    return name + ('.' + group);
  }
  else
  {
    return name;
  }
}
template<class Stream>
inline Stream& mousse::IOobject::writeBanner(Stream& os, bool noHint)
{
  if (! noHint)
  {
    os  << "// -*- c++ -*-\n";
  }
  os << "// mousse: CFD toolbox (v." << mousse_version << ")\n";
  return os;
}
template<class Stream>
inline Stream& mousse::IOobject::writeDivider(Stream& os)
{
  os  <<
    "// ---\n";
  return os;
}
template<class Stream>
inline Stream& mousse::IOobject::writeEndDivider(Stream& os)
{
  os  << "\n"
    "// vim: set ft=foam et sw=2 ts=2 sts=2:\n";
  return os;
}

#endif
