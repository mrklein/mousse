#ifndef CORE_INTERPOLATIONS_TABLE_READERS_TABLE_READER_HPP_
#define CORE_INTERPOLATIONS_TABLE_READERS_TABLE_READER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tableReader
// Description
//   Base class to read table data for the interpolationTable

#include "file_name.hpp"
#include "word_list.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "dictionary.hpp"
#include "tuple2.hpp"


namespace mousse {

template<class Type>
class tableReader
{
public:
  //- Runtime type information
  TYPE_NAME("tableReader");

  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      tableReader,
      dictionary,
      (const dictionary& dict),
      (dict)
    );

  // Constructors

    //- Construct from dictionary
    tableReader(const dictionary& dict);

    //- Construct and return a clone
    virtual autoPtr<tableReader<Type> > clone() const = 0;

  // Selectors

    //- Return a reference to the selected tableReader
    static autoPtr<tableReader> New(const dictionary& spec);

  //- Destructor
  virtual ~tableReader();

  // Member functions

    //- Read the table
    virtual void operator()
    (
      const fileName&,
      List<Tuple2<scalar, Type> >&
    ) = 0;

    //- Read the 2D table
    virtual void operator()
    (
      const fileName&,
      List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >&
    ) = 0;

    //- Write additional information
    virtual void write(Ostream& os) const;
};

}  // namespace mousse


#include "table_reader.ipp"

#endif
