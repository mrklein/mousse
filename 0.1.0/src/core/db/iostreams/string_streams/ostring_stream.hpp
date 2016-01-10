// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OStringStream
// Description
//   Output to memory buffer stream.
// SourceFiles
//   string_streams_print.cpp
#ifndef ostring_stream_hpp_
#define ostring_stream_hpp_
#include "osstream.hpp"
#include <sstream>
namespace mousse
{
class OStringStream
:
  public OSstream
{
public:
  // Constructors
    //- Construct and set stream status
    OStringStream
    (
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      OSstream
      (
       *(new std::ostringstream()),
        "OStringStream.sinkFile",
        format,
        version
      )
    {}
    //- Construct as copy
    OStringStream(const OStringStream& oss)
    :
      OSstream
      (
       *(
          new std::ostringstream
          (
            dynamic_cast<const std::ostringstream&>
            (
              oss.stdStream()
            ).str()
          )
        ),
        oss.name(),
        oss.format(),
        oss.version()
      )
    {}
  //- Destructor
  ~OStringStream()
  {
    delete &dynamic_cast<std::ostringstream&>(stdStream());
  }
  // Member functions
    // Access
      //- Return the string
      string str() const
      {
        return dynamic_cast<const std::ostringstream&>
        (
          stdStream()
        ).str();
      }
    // Edit
      //- Rewind the OStringStream
      void rewind()
      {
        stdStream().rdbuf()->pubseekpos(0);
      }
    // Print
      //- Print description to Ostream
      void print(Ostream&) const;
};
}  // namespace mousse
#endif
