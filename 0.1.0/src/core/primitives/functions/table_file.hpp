#ifndef CORE_PRIMITIVES_FUNCTIONS_TABLE_FILE_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_TABLE_FILE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TableFile
// Description
//   Templated table container data entry where data is read from file.
//   \verbatim
//     <entryName> tableFile;
//     <entryName>Coeffs
//     {
//       dimensions          [0 0 1 0 0]; // optional dimensions
//       fileName            dataFile;    // name of data file
//       outOfBounds         clamp;       // optional out-of-bounds handling
//       interpolationScheme linear;      // optional interpolation method
//     }
//   \endverbatim
//   Items are stored in a list of Tuple2's. First column is always stored as
//   scalar entries.  Data is read in the form, e.g. for an entry \<entryName\>
//   that is (scalar, vector):
//   \verbatim
//     (
//       (0.0 (1 2 3))
//       (1.0 (4 5 6))
//     );
//   \endverbatim
// SourceFiles
//   table_file.cpp


#include "data_entry.hpp"
#include "tuple2.hpp"

namespace mousse
{
template<class Type>
class TableFile;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const TableFile<Type>&
);

template<class Type>
class TableFile
:
  public TableBase<Type>
{
  // Private data
    //- File name for csv table (optional)
    fileName fName_;
public:
  //- Runtime type information
  TYPE_NAME("tableFile");
  // Constructors
    //- Construct from entry name and Istream
    TableFile(const word& entryName, const dictionary& dict);
    //- Copy constructor
    TableFile(const TableFile<Type>& tbl);
    //- Construct and return a clone
    virtual tmp<DataEntry<Type>> clone() const
    {
      return tmp<DataEntry<Type>>(new TableFile<Type>(*this));
    }
    //- Disallow default bitwise assignment
    TableFile<Type>& operator=(const TableFile<Type>&) = delete;
  //- Destructor
  virtual ~TableFile();
  // I/O
    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "table_file.cpp"
#endif
#endif
