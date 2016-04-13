// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CSV<Type>& tbl
)
{
  if (os.format() == IOstream::ASCII) {
    os  << static_cast<const DataEntry<Type>& >(tbl)
      << token::SPACE << tbl.nHeaderLine_
      << token::SPACE << tbl.timeColumn_
      << token::SPACE << tbl.componentColumns_
      << token::SPACE << tbl.separator_
      << token::SPACE << tbl.mergeSeparators_
      << token::SPACE << tbl.fileName_;
  } else {
    os << static_cast<const DataEntry<Type>& >(tbl);
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const CSV<Type>&)");
  return os;
}


template<class Type>
void mousse::CSV<Type>::writeData(Ostream& os) const
{
  DataEntry<Type>::writeData(os);
  os << token::END_STATEMENT << nl;
  os << indent << word(this->name() + "Coeffs") << nl;
  os << indent << token::BEGIN_BLOCK << incrIndent << nl;
  // Note: for TableBase write the dictionary entries it needs but not
  // the values themselves
  TableBase<Type>::writeEntries(os);
  os.writeKeyword("nHeaderLine") << nHeaderLine_ << token::END_STATEMENT << nl;
  os.writeKeyword("refColumn") << refColumn_ << token::END_STATEMENT << nl;
  // Force writing labelList in ascii
  os.writeKeyword("componentColumns");
  if (os.format() == IOstream::BINARY) {
    os.format(IOstream::ASCII);
    os << componentColumns_;
    os.format(IOstream::BINARY);
  } else {
    os << componentColumns_;
  }
  os << token::END_STATEMENT << nl;
  os.writeKeyword("separator") << string(separator_) << token::END_STATEMENT
    << nl;
  os.writeKeyword("mergeSeparators") << mergeSeparators_
    << token::END_STATEMENT << nl;
  os.writeKeyword("fileName") << fName_ << token::END_STATEMENT << nl;
  os  << decrIndent << indent << token::END_BLOCK << endl;
}
