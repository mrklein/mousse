// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IFstream
// Description
//   Input from file stream.
// SourceFiles
//   ifstream.cpp

#ifndef ifstream_hpp_
#define ifstream_hpp_

#include "isstream.hpp"
#include "file_name.hpp"
#include "class_name.hpp"
#include <fstream>

using std::ifstream;
namespace mousse
{
class IFstream;
//- A std::istream with ability to handle compressed files
class IFstreamAllocator
{
  friend class IFstream;
  // Private data
    istream* ifPtr_;
    IOstream::compressionType compression_;
  // Constructors
    //- Construct from pathname
    IFstreamAllocator(const fileName& pathname);
  //- Destructor
  ~IFstreamAllocator();
};
class IFstream
:
  public IFstreamAllocator,
  public ISstream
{
  // Private data
    fileName pathname_;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("IFstream");
  // Constructors
    //- Construct from pathname
    IFstream
    (
      const fileName& pathname,
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    );
  //- Destructor
  ~IFstream();
  // Member functions
    // Access
      //- Return the name of the stream
      const fileName& name() const
      {
        return pathname_;
      }
      //- Return non-const access to the name of the stream
      fileName& name()
      {
        return pathname_;
      }
    // STL stream
      //- Access to underlying std::istream
      virtual istream& stdStream();
      //- Const access to underlying std::istream
      virtual const istream& stdStream() const;
    // Print
      //- Print description of IOstream to Ostream
      virtual void print(Ostream&) const;
  // Member operators
    //- Return a non-const reference to const IFstream
    //  Needed for read-constructors where the stream argument is temporary:
    //  e.g. thing thisThing(IFstream("thingFileName")());
    IFstream& operator()() const;
};
}  // namespace mousse
#endif
