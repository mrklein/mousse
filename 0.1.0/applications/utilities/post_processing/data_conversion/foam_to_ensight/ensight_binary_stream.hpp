// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightBinaryStream
// Description
// SourceFiles
//   ensight_binary_stream.cpp
#ifndef ensight_binary_stream_hpp_
#define ensight_binary_stream_hpp_
#include "ensight_stream.hpp"
namespace mousse
{
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
    virtual bool ascii() const
    {
      return false;
    }
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
      if (sf.size())
      {
        List<float> temp{sf.size()};
        FOR_ALL(sf, i)
        {
          temp[i] = float(sf[i]);
        }
        str_().write
        (
          reinterpret_cast<const char*>(temp.begin()),
          sf.size()*sizeof(float)
        );
      }
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
  // Member Operators
  // Friend Functions
  // Friend Operators
  // IOstream Operators
};
}  // namespace mousse
#endif
