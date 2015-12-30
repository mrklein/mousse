// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_set_option.hpp"
// Member Functions 
bool mousse::fv::cellSetOption::read(const dictionary& dict)
{
  if (option::read(dict))
  {
    if (coeffs_.readIfPresent("timeStart", timeStart_))
    {
      coeffs_.lookup("duration") >> duration_;
    }
  }
  return true;
}
