#ifndef CORE_DB_IOOBJECTS_IOMAP_HPP_
#define CORE_DB_IOOBJECTS_IOMAP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOMap
// Description
//   A Map of objects of type \<T\> with automated input and output.

#include "map.hpp"
#include "reg_ioobject.hpp"


namespace mousse {

template<class T>
class IOMap
:
  public regIOobject,
  public Map<T>
{
public:
  //- Runtime type information
  TYPE_NAME("Map");
  // Constructors
    //- Construct from IOobject
    IOMap(const IOobject&);
    //- Construct from IOobject and size of Map
    IOMap(const IOobject&, const label);
    //- Construct from IOobject and a Map
    IOMap(const IOobject&, const Map<T>&);
    //- Construct by transferring the Map contents
    IOMap(const IOobject&, const Xfer<Map<T> >&);
  //- Destructor
  virtual ~IOMap();
  // Member functions
    bool writeData(Ostream&) const;
  // Member operators
    //- Assignment of other IOMap's entries to this IOMap
    void operator=(const IOMap<T>&);
    //- Assignment of other Map's entries to this IOMap
    void operator=(const Map<T>&);
};

}  // namespace mousse


#include "iomap.ipp"

#endif
