#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_BINARY_STREAM_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_BINARY_STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightBinaryStream

#include "ensight_stream.hpp"


namespace mousse {

class ensightBinaryStream
:
  public ensightStream
{
  // Private data
    //- Description of data_
    autoPtr<std::ofstream> str_;
public:
  // Constructors
    //- Construct from components
    ensightBinaryStream(const fileName& f, const Time&)
    :
      ensightStream{f},
      str_
      {
        new std::ofstream
        {
          f.c_str(),
          ios_base::out | ios_base::binary | ios_base::trunc
        }
      }
    {}
    //- Disallow default bitwise copy construct
    ensightBinaryStream(const ensightBinaryStream&) = delete;
    //- Disallow default bitwise assignment
    ensightBinaryStream& operator=(const ensightBinaryStream&) = delete;
  //- Destructor
  virtual ~ensightBinaryStream()
  {}
  // Member Functions
    virtual bool ascii() const { return false; }
    virtual void write(const char* val)
    {
      char buffer[80] = {0};
      strcpy(buffer, val);
      str_().write(buffer, 80*sizeof(char));
    }
    virtual void write(const int val)
    {
      str_().write(reinterpret_cast<const char*>(&val), sizeof(int));
    }
    virtual void write(const scalarField& sf)
    {
      if (!sf.size())
        return;
      List<float> temp{sf.size()};
      FOR_ALL(sf, i) {
        temp[i] = float(sf[i]);
      }
      str_().write
      (
        reinterpret_cast<const char*>(temp.begin()),
        sf.size()*sizeof(float)
      );
    }
    virtual void write(const List<int>& sf)
    {
      str_().write
      (
        reinterpret_cast<const char*>(sf.begin()),
        sf.size()*sizeof(int)
      );
    }
    virtual void writePartHeader(const label partI)
    {
      write("part");
      write(partI);
    }
};

}  // namespace mousse

#endif

