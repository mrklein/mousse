// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const TableBase<Type>& tbl
)
{
  os << static_cast<const DataEntry<Type>&>(tbl);
  if (os.format() == IOstream::ASCII)
  {
    os << token::SPACE << tbl.table_;
  }
  else
  {
    os.write
    (
      reinterpret_cast<const char*>(&tbl.table_),
      tbl.table_.byteSize()
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const TableBase<Type>&, const bool)"
  );
  return os;
}


template<class Type>
void mousse::TableBase<Type>::writeData(Ostream& os) const
{
  DataEntry<Type>::writeData(os);
  os << nl << indent << table_ << token::END_STATEMENT << nl;
  writeEntries(os);
}


template<class Type>
void mousse::TableBase<Type>::writeEntries(Ostream& os) const
{
  if (boundsHandling_ != CLAMP)
  {
    os.writeKeyword("outOfBounds") << boundsHandlingToWord(boundsHandling_)
      << token::END_STATEMENT << nl;
  }
  if (interpolationScheme_ != "linear")
  {
    os.writeKeyword("interpolationScheme") << interpolationScheme_
      << token::END_STATEMENT << nl;
  }
}
