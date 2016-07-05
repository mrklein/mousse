// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_explicit_set_value.hpp"


// Member Functions 
template<class Type>
bool mousse::fv::ExplicitSetValue<Type>::read(const dictionary& dict)
{
  if (cellSetOption::read(dict)) {
    setFieldData(coeffs_.subDict("injectionRate"));
    return true;
  }
  return false;
}

