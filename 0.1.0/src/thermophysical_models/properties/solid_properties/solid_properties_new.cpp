// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_properties.hpp"
#include "switch.hpp"


mousse::autoPtr<mousse::solidProperties> mousse::solidProperties::New(Istream& is)
{
  if (debug) {
    Info << "solidProperties::New(Istream&): constructing solid"
      << endl;
  }
  const word solidType(is);
  const word coeffs(is);
  if (coeffs == "defaultCoeffs") {
    ConstructorTable::iterator cstrIter =
      ConstructorTablePtr_->find(solidType);
    if (cstrIter == ConstructorTablePtr_->end()) {
      FATAL_ERROR_IN("solidProperties::New(Istream&)")
        << "Unknown solidProperties type " << solidType << nl << nl
        << "Valid solidProperties types are :" << endl
        << ConstructorTablePtr_->sortedToc()
        << exit(FatalError);
    }
    return autoPtr<solidProperties>{cstrIter()()};
  } else if (coeffs == "coeffs") {
    return autoPtr<solidProperties>{new solidProperties{is}};
  } else {
    FATAL_ERROR_IN("solidProperties::New(Istream&)")
      << "solidProperties type " << solidType
      << ", option " << coeffs << " given"
      << ", should be coeffs or defaultCoeffs"
      << exit(FatalError);
    return autoPtr<solidProperties>{NULL};
  }
}


mousse::autoPtr<mousse::solidProperties> mousse::solidProperties::New
(
  const dictionary& dict
)
{
  if (debug) {
    Info << "solidProperties::New(const dictionary&): constructing solid"
      << endl;
  }
  const word solidType{dict.dictName()};
  const Switch defaultCoeffs{dict.lookup("defaultCoeffs")};
  if (defaultCoeffs) {
    ConstructorTable::iterator cstrIter =
      ConstructorTablePtr_->find(solidType);
    if (cstrIter == ConstructorTablePtr_->end()) {
      FATAL_ERROR_IN("solidProperties::New(const dictionary&)")
        << "Unknown solidProperties type " << solidType << nl << nl
        << "Valid solidProperties types are :" << endl
        << ConstructorTablePtr_->sortedToc()
        << exit(FatalError);
    }
    return autoPtr<solidProperties>{cstrIter()()};
  } else {
    return autoPtr<solidProperties>
    {
      new solidProperties{dict.subDict(solidType + "Coeffs")}
    };
  }
}

