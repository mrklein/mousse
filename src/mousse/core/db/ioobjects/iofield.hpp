#ifndef CORE_DB_IOOBJECTS_IOFIELD_HPP_
#define CORE_DB_IOOBJECTS_IOFIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOField
// Description
//   A primitive field of type \<T\> with automated input and output.

#include "reg_ioobject.hpp"
#include "field.hpp"


namespace mousse {

template<class Type>
class IOField
:
  public regIOobject,
  public Field<Type>
{
public:
  TYPE_NAME("Field");

  // Constructors

    //- Construct from IOobject
    IOField(const IOobject&);

    //- Construct from IOobject and size (does not set values)
    IOField(const IOobject&, const label size);

    //- Construct from components
    IOField(const IOobject&, const Field<Type>&);

    //- Construct by transferring the Field contents
    IOField(const IOobject&, const Xfer<Field<Type> >&);

  //- Destructor
  virtual ~IOField();

  // Member functions
    bool writeData(Ostream&) const;

  // Member operators
    void operator=(const IOField<Type>&);

    void operator=(const Field<Type>&);
};
}  // namespace mousse

#include "iofield.ipp"

#endif
