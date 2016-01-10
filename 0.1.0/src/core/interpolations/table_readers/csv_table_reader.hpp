// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::csvTableReader
// Description
//   Reads an interpolation table from a file - CSV-format
// SourceFiles
//   table_reader.cpp

#ifndef csv_table_reader_hpp_
#define csv_table_reader_hpp_

#include "table_reader.hpp"
#include "label_list.hpp"

namespace mousse
{

template<class Type>
class csvTableReader
:
  public tableReader<Type>
{
  // Private data

    //- Does the file have a header line?
    const bool headerLine_;

    //- Column of the time
    const label timeColumn_;

    //- Labels of the components
    const labelList componentColumns_;

    //- Read the next value from the splitted string
    Type readValue(const List<string>&);

    //- Separator character
    const char separator_;

public:

  //- Runtime type information
  TYPE_NAME("csv");

  // Constructors

    //- Construct from dictionary
    csvTableReader(const dictionary& dict);

    //- Construct and return a copy
    virtual autoPtr<tableReader<Type> > clone() const
    {
      return autoPtr<tableReader<Type> >
      {
        new csvTableReader<Type>
        {
          *this
        }
      };
    }

  //- Destructor
  virtual ~csvTableReader();

  // Member Functions

    //- Read the table
    virtual void operator()(const fileName&, List<Tuple2<scalar, Type> >&);

    //- Read 2D table
    virtual void operator()
    (
      const fileName&,
      List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >&
    );

    //- Write the remaining parameters
    virtual void write(Ostream& os) const;
};

}  // namespace mousse
#ifdef NoRepository
#   include "csv_table_reader.cpp"
#endif
#endif
