// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::openFoamTableReader
// Description
//   Reads an interpolation table from a file - OpenFOAM-format
// SourceFiles
//   open_foam_table_reader.cpp
#ifndef open_foam_table_reader_hpp_
#define open_foam_table_reader_hpp_

#include "table_reader.hpp"

namespace mousse
{
template<class Type>
class openFoamTableReader
:
  public tableReader<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("openFoam");
  // Constructors
    //- Construct from dictionary
    openFoamTableReader(const dictionary &dict);
    //- Construct and return a copy
    virtual autoPtr<tableReader<Type> > clone() const
    {
      return autoPtr<tableReader<Type> >
      (
        new openFoamTableReader<Type>
        (
          *this
        )
      );
    }
  //- Destructor
  virtual ~openFoamTableReader();
  // Member functions
    //- Read the table
    virtual void operator()(const fileName&, List<Tuple2<scalar, Type> > &);
    //- Read 2D table
    virtual void operator()
    (
      const fileName&,
      List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >&
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "open_foam_table_reader.cpp"
#endif
#endif
