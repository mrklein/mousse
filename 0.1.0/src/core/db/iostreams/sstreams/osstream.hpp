#ifndef CORE_DB_IOSTREAMS_SSTREAMS_OSSTREAM_HPP_
#define CORE_DB_IOSTREAMS_SSTREAMS_OSSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::OSstream
// Description
//   Generic output stream.
// SourceFiles
//   osstream.cpp


#include "ostream.hpp"
#include "file_name.hpp"
#include <iostream>

namespace mousse
{

class OSstream
:
  public Ostream
{

  // Private data

    fileName name_;
    ostream& os_;

public:

  // Constructors

    //- Set stream status
    OSstream
    (
      ostream& os,
      const string& name,
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    );

    //- Disallow default bitwise assignment
    OSstream& operator=(const OSstream&) = delete;

  // Member functions

    // Enquiry

      //- Return the name of the stream
      //  Useful for Fstream to return the filename
      virtual const fileName& name() const
      {
        return name_;
      }

      //- Return non-const access to the name of the stream
      //  Useful to alter the stream name
      virtual fileName& name()
      {
        return name_;
      }

      //- Return flags of output stream
      virtual ios_base::fmtflags flags() const;

    // Write functions

      //- Write next token to stream
      virtual Ostream& write(const token&);

      //- Write character
      virtual Ostream& write(const char);

      //- Write character string
      virtual Ostream& write(const char*);

      //- Write word
      virtual Ostream& write(const word&);

      //- Write string
      //  In the rare case that the string contains a final trailing
      //  backslash, it will be dropped to the appearance of an escaped
      //  double-quote.
      virtual Ostream& write(const string&);

      //- Write std::string surrounded by quotes.
      //  Optional write without quotes.
      virtual Ostream& writeQuoted
      (
        const std::string&,
        const bool quoted=true
      );

      //- Write int32_t
      virtual Ostream& write(const int32_t);

      //- Write int64_t
      virtual Ostream& write(const int64_t);

      //- Write floatScalar
      virtual Ostream& write(const floatScalar);

      //- Write doubleScalar
      virtual Ostream& write(const doubleScalar);

      //- Write binary block
      virtual Ostream& write(const char*, std::streamsize);

      //- Add indentation characters
      virtual void indent();

    // Stream state functions

      //- Set flags of output stream
      virtual ios_base::fmtflags flags(const ios_base::fmtflags flags);

      //- Flush stream
      virtual void flush();

      //- Add newline and flush stream
      virtual void endl();

      //- Get width of output field
      virtual int width() const;

      //- Set width of output field (and return old width)
      virtual int width(const int);

      //- Get precision of output field
      virtual int precision() const;

      //- Set precision of output field (and return old precision)
      virtual int precision(const int);

    // STL stream

      //- Access to underlying std::ostream
      virtual ostream& stdStream()
      {
        return os_;
      }

      //- Const access to underlying std::ostream
      virtual const ostream& stdStream() const
      {
        return os_;
      }

    // Print

      //- Print description of IOstream to Ostream
      virtual void print(Ostream&) const;

};

}  // namespace mousse

// Constructors 
inline mousse::OSstream::OSstream
(
  ostream& os,
  const string& name,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  Ostream{format, version, compression},
  name_{name},
  os_{os}
{
  if (os_.good())
  {
    setOpened();
    setGood();
    os_.precision(precision_);
  }
  else
  {
    setState(os_.rdstate());
  }
}

#endif
