#ifndef CORE_GLOBAL_SIMPLE_REG_IOOBJECT_HPP_
#define CORE_GLOBAL_SIMPLE_REG_IOOBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simpleRegIOobject
// Description
//   Abstract base class for registered object with I/O. Used in debug symbol
//   registration.

namespace mousse {

// Forward declaration of classes
class Istream;
class Ostream;

class simpleRegIOobject
{
public:
  // Constructors
    //- Construct from objectregistry inserter and name
    simpleRegIOobject
    (
      void (*fn)(const char* name, simpleRegIOobject*),
      const char* name
    )
    {
      (*fn)(name, this);
    }
  //- Destructor
  virtual ~simpleRegIOobject()
  {};
  // Member Functions
    //- Read
    virtual void readData(Istream&) = 0;
    //- Write
    virtual void writeData(Ostream&) const = 0;
};
}  // namespace mousse
#endif
