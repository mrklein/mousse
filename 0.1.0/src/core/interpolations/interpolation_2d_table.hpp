#ifndef CORE_INTERPOLATIONS_INTERPOLATION_2D_TABLE_HPP_
#define CORE_INTERPOLATIONS_INTERPOLATION_2D_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolation2DTable
// Description
//   2D table interpolation. The data must be in ascending order in both
//   dimensions x and y.
// SourceFiles
//   interpolation_2d_table.cpp
#include "list.hpp"
#include "tuple2.hpp"
#include "table_reader.hpp"
namespace mousse
{
template<class Type>
class interpolation2DTable
:
  public List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >
{
public:
  // Public data types
    //- Enumeration for handling out-of-bound values
    enum boundsHandling
    {
      ERROR,          /*!< Exit with a FatalError */
      WARN,           /*!< Issue warning and clamp value (default) */
      CLAMP           /*!< Clamp value to the start/end value */
    };
    //- Cconvenience typedef
    typedef List<Tuple2<scalar, List<Tuple2<scalar, Type> > > > table;
private:
  // Private data
    //- Enumeration for handling out-of-bound values
    boundsHandling boundsHandling_;
    //- File name
    fileName fileName_;
    //- The actual reader
    autoPtr<tableReader<Type> > reader_;
  // Private Member Functions
    //- Read the table of data from file
    void readTable();
    //- Return interpolated value in List
    Type interpolateValue
    (
      const List<Tuple2<scalar, Type> >& data,
      const scalar
    ) const;
    //- Return an X index from the matrix
    template<class BinaryOp>
    label Xi
    (
      const BinaryOp& bop,
      const scalar valueX,
      const bool reverse
    ) const;
public:
  // Constructors
    //- Construct null
    interpolation2DTable();
    //- Construct from components
    interpolation2DTable
    (
      const List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >& values,
      const boundsHandling bounds,
      const fileName& fName
    );
    //- Construct given the name of the file containing the table of data
    interpolation2DTable(const fileName& fName);
    //- Construct by reading the fileName and boundsHandling from dictionary
    interpolation2DTable(const dictionary& dict);
    //- Construct copy
    interpolation2DTable(const interpolation2DTable& interpTable);
  // Member Functions
    //- Return the out-of-bounds handling as a word
    word boundsHandlingToWord(const boundsHandling& bound) const;
    //- Return the out-of-bounds handling as an enumeration
    boundsHandling wordToBoundsHandling(const word& bound) const;
    //- Set the out-of-bounds handling from enum, return previous setting
    boundsHandling outOfBounds(const boundsHandling& bound);
    //- Check that list is monotonically increasing
    //  Exit with a FatalError if there is a problem
    void checkOrder() const;
    //- Write
    void write(Ostream& os) const;
  // Member Operators
    //- Return an element of constant Tuple2<scalar, Type>
    const List<Tuple2<scalar, Type> >& operator[](const label) const;
    //- Return an interpolated value
    Type operator()(const scalar, const scalar) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "interpolation_2d_table.cpp"
#endif
#endif
