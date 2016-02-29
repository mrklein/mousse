#ifndef MESH_TOOLS_SETS_CELL_SOURCES_FIELD_TO_CELL_FIELD_DICTIONARY_HPP_
#define MESH_TOOLS_SETS_CELL_SOURCES_FIELD_TO_CELL_FIELD_DICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldDictionary
// Description
//   Read field as dictionary (without mesh).
#include "reg_ioobject.hpp"
#include "dictionary.hpp"
namespace mousse
{
class fieldDictionary
:
  public regIOobject,
  public dictionary
{
  const word type_;
public:
  // Redefine type name to be of the instantiated type
  virtual const word& type() const
  {
    return type_;
  }
  // Constructors
    //- Construct from ioobject and overloaded typename.
    fieldDictionary(const IOobject& io, const word& type)
    :
      regIOobject(io),
      dictionary(readStream(type)),
      type_(type)
    {
      close();
    }
  // Member functions
    bool writeData(Ostream& os) const
    {
      static_cast<const dictionary&>(*this).write(os, false);
      return os.good();
    }
};
}  // namespace mousse
#endif
