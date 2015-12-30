// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightAsciiStream
// Description
// SourceFiles
//   ensight_ascii_stream.cpp
#ifndef ensight_ascii_stream_hpp_
#define ensight_ascii_stream_hpp_
#include "ensight_stream.hpp"
#include "ofstream.hpp"
namespace mousse
{
class ensightAsciiStream
:
  public ensightStream
{
  // Private data
    //- Description of data_
    OFstream str_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    ensightAsciiStream(const ensightAsciiStream&);
    //- Disallow default bitwise assignment
    void operator=(const ensightAsciiStream&);
public:
  // Constructors
    //- Construct from components
    ensightAsciiStream(const fileName& f, const Time& runTime)
    :
      ensightStream(f),
      str_
      (
        f,
        runTime.writeFormat(),
        runTime.writeVersion(),
        IOstream::UNCOMPRESSED
      )
    {
      str_.setf(ios_base::scientific, ios_base::floatfield);
      str_.precision(5);
    }
  //- Destructor
  virtual ~ensightAsciiStream()
  {}
  // Member Functions
    virtual bool ascii() const
    {
      return true;
    }
    virtual void write(const char* c)
    {
      str_ << c << nl;
    }
    virtual void write(const int v)
    {
      str_ << setw(10) << v << nl;
    }
    virtual void write(const scalarField& sf)
    {
      forAll(sf, i)
      {
        if (mag(sf[i]) >= scalar(floatScalarVSMALL))
        {
          str_ << setw(12) << sf[i] << nl;
        }
        else
        {
          str_ << setw(12) << scalar(0) << nl;
        }
      }
    }
    virtual void write(const List<int>& sf)
    {
      forAll(sf, i)
      {
        str_ << setw(10) << sf[i];
      }
      str_<< nl;
    }
    virtual void writePartHeader(const label partI)
    {
      str_<< "part" << nl
        << setw(10) << partI << nl;
    }
  // Member Operators
  // Friend Functions
  // Friend Operators
  // IOstream Operators
};
}  // namespace mousse
#endif
