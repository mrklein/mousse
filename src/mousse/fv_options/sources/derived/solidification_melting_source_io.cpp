// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solidification_melting_source.hpp"


// Private Member Functions 
bool mousse::fv::solidificationMeltingSource::read(const dictionary& dict)
{
  if (cellSetOption::read(dict)) {
    coeffs_.lookup("Tmelt") >> Tmelt_;
    coeffs_.lookup("L") >> L_;
    coeffs_.readIfPresent("relax", relax_);
    mode_ = thermoModeTypeNames_.read(coeffs_.lookup("thermoMode"));
    coeffs_.lookup("rhoRef") >> rhoRef_;
    coeffs_.readIfPresent("TName", TName_);
    coeffs_.readIfPresent("UName", UName_);
    coeffs_.readIfPresent("Cu", Cu_);
    coeffs_.readIfPresent("q", q_);
    coeffs_.readIfPresent("beta", beta_);
    return true;
  }
  return false;
}

