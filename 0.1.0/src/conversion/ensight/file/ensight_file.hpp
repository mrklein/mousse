// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightFile
// Description
//   Ensight output with specialized write() for strings, integers and floats.
//   Correctly handles binary write as well.
#ifndef ensight_file_hpp_
#define ensight_file_hpp_
#include "ofstream.hpp"
#include "iostream.hpp"
namespace mousse
{
class ensightFile
:
  public OFstream
{
  // Private data
    //- Allow undef in results
    static bool allowUndef_;
    //- Value to represent undef in results
    static scalar undefValue_;
    //- The '*' mask appropriate for subDir
    static string mask_;
    //- The printf format for zero-padded subdirectory numbers
    static string dirFmt_;
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const ensightFile&);
    //- Disallow default copy constructor
    ensightFile(const ensightFile&);
public:
  // Constructors
    //- Construct from pathname
    ensightFile
    (
      const fileName& pathname,
      IOstream::streamFormat format=IOstream::BINARY
    );
  //- Destructor
  ~ensightFile();
  // Access
    //- Return setting for whether 'undef' values are allowed in results
    static bool allowUndef();
    //- The '*' mask appropriate for subDir
    static string mask();
    //- Consistent zero-padded numbers for subdirectories
    static string subDir(const label);
    //- Set width of subDir and mask. Default width is 8 digits.
    //  Max width is 31 digits.
    static void subDirWidth(const label);
    //- Return current width of subDir and mask.
    static label subDirWidth();
  // Edit
    static bool allowUndef(bool);
    //- Assign the value to represent undef in the results
    //  Returns the previous value
    //  NB: do not use values larger than floatScalarVGREAT
    static scalar undefValue(const scalar);
  // Output
    //- Inherit write from Ostream
    using Ostream::write;
    //- Binary write
    virtual Ostream& write(const char* buf, std::streamsize count);
    //- Write element keyword with trailing newline, optionally with undef
    virtual Ostream& writeKeyword(const string& key);
    //- Write "C Binary" for binary files (eg, geometry/measured)
    Ostream& writeBinaryHeader();
    //- Write undef value
    Ostream& writeUndef();
    //- Write C-string as "%80s" or as binary
    Ostream& write(const char* value);
    //- Write string as "%80s" or as binary
    Ostream& write(const string& value);
    //- Write integer as "%10d" or as binary
    Ostream& write(const label value);
    //- Write integer with specified width or as binary
    Ostream& write(const label value, const label fieldWidth);
    //- Write float as "%12.5e" or as binary
    Ostream& write(const scalar value);
    //- Add carriage return to ascii stream
    void newline();
};
}  // namespace mousse
#endif
