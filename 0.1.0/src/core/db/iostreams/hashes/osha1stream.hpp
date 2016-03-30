#ifndef CORE_DB_IOSTREAMS_HASHES_OSHA1STREAM_HPP_
#define CORE_DB_IOSTREAMS_HASHES_OSHA1STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OSHA1stream
// Description
//   An output stream for calculating SHA1 digests.

#include "osstream.hpp"
#include "sha1.hpp"


namespace mousse {

class osha1stream;
class OSHA1stream;

//- A streambuf class for calculating SHA1 digests
class sha1streambuf
:
  public std::streambuf
{

  // Private data

    //- This does all the work and has its own buffering
    SHA1 sha1_;
    friend class osha1stream;

public:

  // Constructors

    //- Construct null
    sha1streambuf()
    {}

  // Member Functions

    // Write
      //- Process unbuffered
      virtual std::streamsize xsputn(const char* str, std::streamsize n)
      {
        sha1_.append(str, n);
        return n;
      }
};

//- A basic output stream for calculating SHA1 digests
class osha1stream
:
  virtual public std::ios,
  public std::ostream
{

  // Private data
  sha1streambuf sbuf_;

public:

  // Constructors

    //- Construct null
    osha1stream()
    :
      std::ostream{&sbuf_}
    {}

  // Member Functions

    // Access

      //- This hides both signatures of std::basic_ios::rdbuf()
      sha1streambuf* rdbuf()
      {
        return &sbuf_;
      }

      //- Full access to the sha1
      SHA1& sha1()
      {
        return sbuf_.sha1_;
      }
};

//- The output stream for calculating SHA1 digests
class OSHA1stream
:
  public OSstream
{

public:
  // Constructors

    //- Construct and set stream status
    OSHA1stream
    (
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      OSstream
      {
        *(new osha1stream),
        "OSHA1stream.sinkFile_",
        format,
        version
      }
    {}

    //- Disallow default bitwise copy construct
    OSHA1stream(const OSHA1stream&) = delete;

    //- Disallow default bitwise assignment
    OSHA1stream& operator=(const OSHA1stream&) = delete;

  //- Destructor
  ~OSHA1stream()
  {
    delete &dynamic_cast<osha1stream&>(stdStream());
  }

  // Member functions

    // Access

      //- Full access to the sha1
      mousse::SHA1& sha1()
      {
        return dynamic_cast<osha1stream&>(stdStream()).sha1();
      }

      //- Return SHA1::Digest for the data processed until now
      mousse::SHA1Digest digest()
      {
        return sha1().digest();
      }

    // Edit

      //- Clear the SHA1 calculation
      void rewind()
      {
        sha1().clear();
      }
};

}  // namespace mousse
#endif
