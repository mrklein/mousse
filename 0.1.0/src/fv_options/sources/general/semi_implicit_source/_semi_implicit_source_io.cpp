// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_semi_implicit_source.hpp"
// Member Functions 
template<class Type>
bool mousse::fv::SemiImplicitSource<Type>::read(const dictionary& dict)
{
  if (cellSetOption::read(dict))
  {
    volumeMode_ = wordToVolumeModeType(coeffs_.lookup("volumeMode"));
    setFieldData(coeffs_.subDict("injectionRateSuSp"));
    return true;
  }
  else
  {
    return false;
  }
}
