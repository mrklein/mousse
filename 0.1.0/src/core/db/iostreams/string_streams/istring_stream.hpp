#ifndef CORE_DB_IOSTREAMS_STRING_STREAMS_ISTRING_STREAM_HPP_
#define CORE_DB_IOSTREAMS_STRING_STREAMS_ISTRING_STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IStringStream
// Description
//   Input from memory buffer stream.
// SourceFiles
//   string_streams_print.cpp
#include "isstream.hpp"
#include <sstream>
namespace mousse
{
class IStringStream
:
  public ISstream
{
public:
  // Constructors
    //- Construct from string
    IStringStream
    (
      const string& buffer,
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      ISstream
      (
        *(new std::istringstream(buffer)),
        "IStringStream.sourceFile",
        format,
        version
      )
    {}
    //- Construct from char*
    IStringStream
    (
      const char* buffer,
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      ISstream
      (
        *(new std::istringstream(buffer)),
        "IStringStream.sourceFile",
        format,
        version
      )
    {}
  //- Destructor
  ~IStringStream()
  {
    delete &dynamic_cast<std::istringstream&>(stdStream());
  }
  // Member functions
    // Access
      //- Return the string
      string str() const
      {
        return dynamic_cast<const std::istringstream&>
        (
          stdStream()
        ).str();
      }
    // Print
      //- Print description to Ostream
      void print(Ostream&) const;
  // Member operators
    //- Return a non-const reference to const Istream
    //  Needed for read-constructors where the stream argument is temporary:
    //  e.g. thing thisThing(IFstream("thingFileName")());
    Istream& operator()() const
    {
      return const_cast<IStringStream&>(*this);
    }
};
}  // namespace mousse
#endif
