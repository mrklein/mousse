#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_ASCII_STREAM_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_ASCII_STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightAsciiStream

#include "ensight_stream.hpp"
#include "ofstream.hpp"


namespace mousse {

class ensightAsciiStream
:
  public ensightStream
{
  // Private data
    //- Description of data_
    OFstream str_;
public:
  // Constructors
    //- Construct from components
    ensightAsciiStream(const fileName& f, const Time& runTime)
    :
      ensightStream{f},
      str_
      {
        f,
        runTime.writeFormat(),
        runTime.writeVersion(),
        IOstream::UNCOMPRESSED
      }
    {
      str_.setf(ios_base::scientific, ios_base::floatfield);
      str_.precision(5);
    }
    //- Disallow default bitwise copy construct
    ensightAsciiStream(const ensightAsciiStream&) = delete;
    //- Disallow default bitwise assignment
    ensightAsciiStream& operator=(const ensightAsciiStream&) = delete;
  //- Destructor
  virtual ~ensightAsciiStream()
  {}
  // Member Functions
    virtual bool ascii() const { return true; }
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
      FOR_ALL(sf, i) {
        if (mag(sf[i]) >= scalar(floatScalarVSMALL)) {
          str_ << setw(12) << sf[i] << nl;
        } else {
          str_ << setw(12) << scalar(0) << nl;
        }
      }
    }
    virtual void write(const List<int>& sf)
    {
      FOR_ALL(sf, i) {
        str_ << setw(10) << sf[i];
      }
      str_<< nl;
    }
    virtual void writePartHeader(const label partI)
    {
      str_<< "part" << nl << setw(10) << partI << nl;
    }
};

}  // namespace mousse

#endif

