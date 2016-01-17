// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AverageIOField
// Description
//   A primitive field + average with IO.
// SourceFiles
//   average_io_field.cpp
#ifndef average_io_field_hpp_
#define average_io_field_hpp_
#include "reg_ioobject.hpp"
#include "field.hpp"
namespace mousse
{
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
#ifdef NoRepository
#   include "average_io_field.cpp"
#endif
#endif
