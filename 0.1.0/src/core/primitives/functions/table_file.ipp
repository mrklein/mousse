// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "table_file.hpp"


// Constructors 
template<class Type>
mousse::TableFile<Type>::TableFile(const word& entryName, const dictionary& dict)
:
  TableBase<Type>{entryName, dict.subDict(entryName + "Coeffs")},
  fName_{"none"}
{
  const dictionary coeffs{dict.subDict(entryName + "Coeffs")};
  coeffs.lookup("fileName") >> fName_;
  if (coeffs.found("dimensions"))
  {
    coeffs.lookup("dimensions") >> this->dimensions_;
  }
  fileName expandedFile{fName_};
  IFstream is{expandedFile.expand()};
  if (!is.good())
  {
    FATAL_IO_ERROR_IN
    (
      "TableFile<Type>::TableFile(const word&, const dictionary&)",
      is
    )
    << "Cannot open file." << exit(FatalIOError);
  }
  is >> this->table_;
  TableBase<Type>::check();
}


template<class Type>
mousse::TableFile<Type>::TableFile(const TableFile<Type>& tbl)
:
  TableBase<Type>{tbl},
  fName_{tbl.fName_}
{}


// Destructor 
template<class Type>
mousse::TableFile<Type>::~TableFile()
{}

//  IOStream operators
#include "table_file_io.ipp"
