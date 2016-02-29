#ifndef CORE_DB_IOSTREAMS_FSTREAMS_OFSTREAM_HPP_
#define CORE_DB_IOSTREAMS_FSTREAMS_OFSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OFstream
// Description
//   Output to file stream.
// SourceFiles
//   ofstream.cpp


#include "osstream.hpp"
#include "file_name.hpp"
#include "class_name.hpp"
#include <fstream>

using std::ofstream;
namespace mousse
{
class OFstream;
//- A std::ostream with ability to handle compressed files
class OFstreamAllocator
{
  friend class OFstream;
  ostream* ofPtr_;
  // Constructors
    //- Construct from pathname
    OFstreamAllocator
    (
      const fileName& pathname,
      IOstream::compressionType compression=IOstream::UNCOMPRESSED
    );
  //- Destructor
  ~OFstreamAllocator();
};
class OFstream
:
  private OFstreamAllocator,
  public OSstream
{
  // Private data
    fileName pathname_;
public:
  // Declare name of the class and its debug switch
  CLASS_NAME("OFstream");
  // Constructors
    //- Construct from pathname
    OFstream
    (
      const fileName& pathname,
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    );
  //- Destructor
  ~OFstream();
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
      //- Access to underlying std::ostream
      virtual ostream& stdStream();
      //- Const access to underlying std::ostream
      virtual const ostream& stdStream() const;
    // Print
      //- Print description of IOstream to Ostream
      void print(Ostream&) const;
};
//- Global predefined null output stream "/dev/null"
extern OFstream Snull;
}  // namespace mousse
#endif
