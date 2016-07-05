#ifndef CORE_PRIMITIVES_FUNCTIONS_TABLE_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Table
// Description
//   Templated table container data entry. Items are stored in a list of
//   Tuple2's. First column is always stored as scalar entries. Data is read
//   in Tuple2 form, e.g. for an entry \<entryName\> that is (scalar, vector):
//   \verbatim
//     <entryName>   table
//     (
//       (0.0 (1 2 3))
//       (1.0 (4 5 6))
//     );
//   \endverbatim
// SourceFiles
//   table.cpp

#include "data_entry.hpp"
#include "tuple2.hpp"


namespace mousse {

template<class Type>
class Table;

template<class Type>
Ostream& operator<<
(
  Ostream&,
  const Table<Type>&
);


template<class Type>
class Table
:
  public TableBase<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("table");
  // Constructors
    //- Construct from entry name and Istream
    Table(const word& entryName, const dictionary& dict);
    //- Copy constructor
    Table(const Table<Type>& tbl);
    //- Construct and return a clone
    virtual tmp<DataEntry<Type>> clone() const
    {
      return tmp<DataEntry<Type>>{new Table<Type>{*this}};
    }
    //- Disallow default bitwise assignment
    Table<Type>& operator=(const Table<Type>&) = delete;
  //- Destructor
  virtual ~Table();
};

}  // namespace mousse

#include "table.ipp"

#endif
