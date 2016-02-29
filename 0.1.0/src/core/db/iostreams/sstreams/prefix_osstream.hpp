#ifndef CORE_DB_IOSTREAMS_SSTREAMS_PREFIX_OSSTREAM_HPP_
#define CORE_DB_IOSTREAMS_SSTREAMS_PREFIX_OSSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::prefixOSstream
// Description
//   Version of OSstream which prints a prefix on each line.
//   This is useful for running in parallel as it allows the processor number
//   to be automatically prepended to each message line.
// SourceFiles
//   prefix_osstream.cpp
#include "osstream.hpp"
namespace mousse
{
class prefixOSstream
:
  public OSstream
{
  // Private data
    bool printPrefix_;
    string prefix_;
  // Private Member Functions
    inline void checkWritePrefix();
public:
  // Constructors
    //- Set stream status
    prefixOSstream
    (
      ostream& os,
      const string& name,
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    );
  // Member functions
    // Enquiry
      //- Return the prefix of the stream
      const string& prefix() const
      {
        return prefix_;
      }
      //- Return non-const access to the prefix of the stream
      string& prefix()
      {
        return prefix_;
      }
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
    // Print
      //- Print description of IOstream to Ostream
      virtual void print(Ostream&) const;
};
}  // namespace mousse
#endif
