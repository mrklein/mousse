// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CSV
// Description
//   Templated CSV container data entry.  Reference column is always a scalar,
//   e.g. time
//   \verbatim
//     <entryName> csvFile;
//     <entryName>Coeffs
//     {
//       nHeaderLine         4;          // number of header lines
//       refColumn           0;          // reference column index
//       componentColumns    (1 2 3);    // component column indices
//       separator           ",";        // optional (defaults to ",")
//       mergeSeparators     no;         // merge multiple separators
//       fileName            "fileXYZ";  // name of csv data file
//       outOfBounds         clamp;      // optional out-of-bounds handling
//       interpolationScheme linear;     // optional interpolation scheme
//     }
//   \endverbatim
// SourceFiles
//   csv.cpp

#ifndef csv_hpp_
#define csv_hpp_

#include "data_entry.hpp"
#include "table_base.hpp"
#include "tuple2.hpp"
#include "label_list.hpp"
#include "isstream.hpp"

namespace mousse
{
template<class Type>
class CSV;

template<class Type>
Ostream& operator<<
(
  Ostream&,
  const CSV<Type>&
);

template<class Type>
class CSV
:
  public TableBase<Type>
{
  // Private data

    //- Coefficients dictionary (for convenience on reading)
    dictionary coeffs_;

    //- Number header lines
    label nHeaderLine_;

    //- Column of the time
    label refColumn_;

    //- Labels of the components
    labelList componentColumns_;

    //- Separator character
    char separator_;

    //- Merge separators flag, e.g. ',,,' becomes ','
    bool mergeSeparators_;

    //- File name for csv table
    fileName fName_;

  // Private Member Functions

    //- Read csv data table
    void read();

    //- Read the next value from the splitted string
    Type readValue(const List<string>&);

public:
  //- Runtime type information
  TYPE_NAME("csvFile");

  // Constructors

    //- Construct from entry name and dictionary
    CSV
    (
      const word& entryName,
      const dictionary& dict,
      const word& ext = "Coeffs"
    );

    //- Copy constructor
    CSV(const CSV<Type>& tbl);

    //- Construct and return a clone
    virtual tmp<DataEntry<Type> > clone() const
    {
      return tmp<DataEntry<Type> >(new CSV<Type>(*this));
    }

    //- Disallow default bitwise assignment
    CSV<Type>& operator=(const CSV<Type>&) = delete;

  //- Destructor
  virtual ~CSV();

  // Member Functions

    // Access

      //- Return const access to the file name
      virtual const fileName& fName() const;

  // I/O

    //- Ostream Operator
    friend Ostream& operator<< <Type>
    (
      Ostream& os,
      const CSV<Type>& cnst
    );

    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;

};
}  // namespace mousse
#ifdef NoRepository
#   include "csv.cpp"
#endif
#endif
