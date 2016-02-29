#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_STREAM_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_ENSIGHT_STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightStream
// Description
//   Abstract base class for writing Ensight data
// SourceFiles
//   ensight_stream.cpp
#include "file_name.hpp"
#include "scalar_field.hpp"
namespace mousse
{
class ensightStream
{
  // Private data
    const fileName name_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    ensightStream(const ensightStream&);
    //- Disallow default bitwise assignment
    void operator=(const ensightStream&);
public:
  // Constructors
    //- Construct from components
    ensightStream(const fileName& f)
    :
      name_(f)
    {}
  //- Destructor
  virtual ~ensightStream()
  {}
  // Member Functions
    const fileName& name() const
    {
      return name_;
    }
    virtual bool ascii() const = 0;
    virtual void write(const char*) = 0;
    virtual void write(const int) = 0;
    virtual void write(const scalarField&) = 0;
    virtual void write(const List<int>&) = 0;
    virtual void writePartHeader(const label) = 0;
};
}  // namespace mousse
#endif
