// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"


// IOstream Operators 
template<class Type>
void mousse::TableFile<Type>::writeData(Ostream& os) const
{
  DataEntry<Type>::writeData(os);
  os << token::END_STATEMENT << nl
    << indent << word(this->name() + "Coeffs") << nl
    << indent << token::BEGIN_BLOCK << nl << incrIndent;
  // Note: for TableBase write the dictionary entries it needs but not
  // the values themselves
  TableBase<Type>::writeEntries(os);
  os.writeKeyword("fileName")<< fName_ << token::END_STATEMENT << nl;
  os << decrIndent << indent << token::END_BLOCK << endl;
}
