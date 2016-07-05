// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_option.hpp"


// Member Functions 
void mousse::fv::option::writeHeader(Ostream& os) const
{
  os << indent << name_ << nl
    << indent << token::BEGIN_BLOCK << incrIndent << nl;
}


void mousse::fv::option::writeFooter(Ostream& os) const
{
  os  << decrIndent << indent << token::END_BLOCK << endl;
}


void mousse::fv::option::writeData(Ostream& os) const
{
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  os.writeKeyword("active") << active_ << token::END_STATEMENT << nl << nl;
  os << indent << word(type() + "Coeffs");
  coeffs_.write(os);
}


bool mousse::fv::option::read(const dictionary& dict)
{
  dict.readIfPresent("active", active_);
  coeffs_ = dict.subDict(modelType_ + "Coeffs");
  return true;
}

