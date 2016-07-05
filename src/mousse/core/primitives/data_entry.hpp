#ifndef CORE_PRIMITIVES_FUNCTIONS_DATA_ENTRY_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_DATA_ENTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DataEntry
// Description
//   Top level data entry class for use in dictionaries. Provides a mechanism
//   to specify a variable as a certain type, e.g. constant or table, and
//   provide functions to return the (interpolated) value, and integral between
//   limits.
// SourceFiles
//   data_entry.ipp
//   data_entry_new.ipp

#include "dictionary.hpp"
#include "field.hpp"
#include "dimensioned_type.hpp"


namespace mousse {

class Time;

template<class Type>
class DataEntry;

template<class Type>
Ostream& operator<<
(
  Ostream&,
  const DataEntry<Type>&
);

template<class Type>
class DataEntry
:
  public refCount
{
protected:

  // Protected data

    //- Name of entry
    const word name_;

public:

  //- Runtime type information
  TYPE_NAME("DataEntry")

  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    DataEntry,
    dictionary,
    (
      const word& entryName,
      const dictionary& dict
    ),
    (entryName, dict)
  );

  // Constructor
    //- Construct from entry name
    DataEntry(const word& entryName);

    //- Copy constructor
    DataEntry(const DataEntry<Type>& de);

    //- Construct and return a clone
    virtual tmp<DataEntry<Type>> clone() const
    {
      return tmp<DataEntry<Type>>{new DataEntry<Type>{*this}};
    }

    //- Disallow default bitwise assignment
    DataEntry<Type>& operator=(const DataEntry<Type>&) = delete;

  //- Selector
  static autoPtr<DataEntry<Type>> New
  (
    const word& entryName,
    const dictionary& dict
  );

  //- Destructor
  virtual ~DataEntry();

  // Member Functions

    // Access

      //- Return the name of the entry
      const word& name() const;

    // Manipulation

      //- Convert time
      virtual void convertTimeBase(const Time& t);

    // Evaluation

      //- Return value as a function of (scalar) independent variable
      virtual Type value(const scalar x) const;

      //- Return value as a function of (scalar) independent variable
      virtual tmp<Field<Type>> value(const scalarField& x) const;

      //- Integrate between two (scalar) values
      virtual Type integrate(const scalar x1, const scalar x2) const;

      //- Integrate between two (scalar) values
      virtual tmp<Field<Type>> integrate
      (
        const scalarField& x1,
        const scalarField& x2
      ) const;

      //- Return dimensioned type
      virtual dimensioned<Type> dimValue(const scalar x) const;

      //- Return dimensioned type as a function of (scalar)
      virtual tmp<Field<dimensioned<Type>>> dimValue
      (
        const scalarField& x
      ) const;

      //- Integrate between two scalars and return a dimensioned type
      virtual dimensioned<Type> dimIntegrate
      (
        const scalar x1,
        const scalar x2
      ) const;

      //- Integrate between two scalar fields and return a field of
      //  dimensioned type
      virtual tmp<Field<dimensioned<Type>>> dimIntegrate
      (
        const scalarField& x1,
        const scalarField& x2
      ) const;

    // I/O
      //- Ostream Operator
      friend Ostream& operator<< <Type>
      (
        Ostream& os,
        const DataEntry<Type>& de
      );

      //- Write in dictionary format
      virtual void writeData(Ostream& os) const;
};
}  // namespace mousse

#define MAKE_DATA_ENTRY(Type)                                                 \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(DataEntry<Type>, 0);              \
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    DataEntry<Type>,                                                          \
    dictionary                                                                \
  );

#define MAKE_DATA_ENTRY_TYPE(SS, Type)                                        \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<Type>, 0);                     \
                                                                              \
  DataEntry<Type>::adddictionaryConstructorToTable<SS<Type>>                  \
    add##SS##Type##ConstructorToTable_;


#include "data_entry.ipp"
#include "data_entry_new.ipp"

#endif
