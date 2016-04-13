#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_AVERAGE_IO_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_AVERAGE_IO_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AverageIOField
// Description
//   A primitive field + average with IO.

#include "reg_ioobject.hpp"
#include "field.hpp"


namespace mousse {

template<class Type>
class AverageIOField
:
  public regIOobject,
  public Field<Type>
{
  // Private data
    //- The average of the field
    Type average_;
public:
  TYPE_NAME("AverageField");
  // Constructors
    //- Construct from IOobject
    AverageIOField
    (
      const IOobject&
    );
    //- Construct from IOobject and size (does not set values)
    AverageIOField
    (
      const IOobject&,
      const label size
    );
    //- Construct from components
    AverageIOField
    (
      const IOobject&,
      const Type& average,
      const Field<Type>&
    );
  // Member functions
    const Type& average() const
    {
      return average_;
    }
    Type& average()
    {
      return average_;
    }
    bool writeData(Ostream& os) const;
};
}  // namespace mousse

#include "average_io_field.ipp"

#endif
