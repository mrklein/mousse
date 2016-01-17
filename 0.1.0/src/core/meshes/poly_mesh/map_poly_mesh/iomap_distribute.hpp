// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOmapDistribute
// Description
//   IOmapDistribute is derived from mapDistribute and
//   IOobject to give the mapDistribute
//   automatic IO functionality via the objectRegistry.
// SourceFiles
//   iomap_distribute.cpp
#ifndef iomap_distribute_hpp_
#define iomap_distribute_hpp_
#include "map_distribute.hpp"
#include "reg_ioobject.hpp"
namespace mousse
{
class IOmapDistribute
:
  public regIOobject,
  public mapDistribute
{
public:
  //- Runtime type information
  TYPE_NAME("mapDistribute");
  // Constructors
    //- Construct given an IOobject
    IOmapDistribute(const IOobject&);
    //- Construct given an IOobject and mapDistribute
    IOmapDistribute(const IOobject&, const mapDistribute&);
    //- Construct by transferring the mapDistribute contents
    IOmapDistribute(const IOobject&, const Xfer<mapDistribute>&);
  //- Destructor
  virtual ~IOmapDistribute();
  // Member functions
    //- ReadData function required for regIOobject read operation
    virtual bool readData(Istream&);
    //- WriteData function required for regIOobject write operation
    virtual bool writeData(Ostream&) const;
};
}  // namespace mousse
#endif
