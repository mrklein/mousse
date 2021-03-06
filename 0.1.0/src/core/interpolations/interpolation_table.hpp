#ifndef CORE_INTERPOLATIONS_INTERPOLATION_TABLE_HPP_
#define CORE_INTERPOLATIONS_INTERPOLATION_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationTable
// Description
//   An interpolation/look-up table of scalar vs \<Type\> values.
//   The reference scalar values must be monotonically increasing.
//   The handling of out-of-bounds values depends on the current setting
//   of \a outOfBounds.
//   If \a REPEAT is chosen for the out-of-bounds handling, the final time
//   value is treated as being equivalent to time=0 for the following periods.
//   The construct from dictionary reads a filename from a dictionary and
//   has an optional readerType. Default is to read OpenFOAM format. The only
//   other format is csv (comma separated values):
//   Read csv format:
//     readerType      csv;
//     fileName        "$MOUSSE_CASE/constant/p0vsTime.csv";
//     hasHeaderLine   true;   // skip first line
//     timeColumn      0;      // time is in column 0
//     valueColumns    (1);    // value starts in column 1
// Note
//   - Accessing an empty list results in an error.
//   - Accessing a list with a single element always returns the same value.

#include "list.hpp"
#include "tuple2.hpp"
#include "table_reader.hpp"
#include "auto_ptr.hpp"


namespace mousse {

template<class Type>
class interpolationTable
:
  public List<Tuple2<scalar, Type>>
{
public:
  // Public data types
    //- Enumeration for handling out-of-bound values
    enum boundsHandling
    {
      ERROR,          /*!< Exit with a FatalError */
      WARN,           /*!< Issue warning and clamp value (default) */
      CLAMP,          /*!< Clamp value to the start/end value */
      REPEAT          /*!< Treat as a repeating list */
    };

private:

  // Private data
    //- Enumeration for handling out-of-bound values
    boundsHandling boundsHandling_;

    //- File name
    fileName fileName_;

    //- The actual reader
    autoPtr<tableReader<Type>> reader_;

  // Private Member Functions

    //- Read the table of data from file
    void readTable();

public:

  // Constructors

    //- Construct null
    interpolationTable();

    //- Construct from components
    interpolationTable
    (
      const List<Tuple2<scalar, Type>>& values,
      const boundsHandling bounds,
      const fileName& fName
    );

    //- Construct given the name of the file containing the table of data
    interpolationTable(const fileName& fName);

    //- Construct by reading the fileName and boundsHandling from dictionary
    //  and read the table from that file.
    //  This is a specialised constructor used by patchFields
    interpolationTable(const dictionary& dict);

    //- Construct copy
    interpolationTable(const interpolationTable& interpTable);

  // Member Functions

    //- Return the out-of-bounds handling as a word
    word boundsHandlingToWord(const boundsHandling& bound) const;

    //- Return the out-of-bounds handling as an enumeration
    boundsHandling wordToBoundsHandling(const word& bound) const;

    //- Set the out-of-bounds handling from enum, return previous setting
    boundsHandling outOfBounds(const boundsHandling& bound);

    //- Check that list is monotonically increasing
    //  Exit with a FatalError if there is a problem
    void check() const;

    //- Write
    void write(Ostream& os) const;

    //- Return the rate of change at the interpolation location
    //  for the give value
    Type rateOfChange(const scalar) const;

  // Member Operators
 
    //- Return an element of constant Tuple2<scalar, Type>
    const Tuple2<scalar, Type>& operator[](const label) const;

    //- Return an interpolated value
    Type operator()(const scalar) const;

};

}  // namespace mousse

#include "interpolation_table.ipp"

#endif
