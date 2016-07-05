// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_change_two_phase_mixture.hpp"
#include "incompressible_two_phase_mixture.hpp"


mousse::autoPtr<mousse::phaseChangeTwoPhaseMixture>
mousse::phaseChangeTwoPhaseMixture::New
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
{
  IOdictionary transportPropertiesDict
  {
    {
      "transportProperties",
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  word phaseChangeTwoPhaseMixtureTypeName
  {
    transportPropertiesDict.lookup("phaseChangeTwoPhaseMixture")
  };
  Info << "Selecting phaseChange model "
    << phaseChangeTwoPhaseMixtureTypeName << endl;
  componentsConstructorTable::iterator cstrIter =
    componentsConstructorTablePtr_->find(phaseChangeTwoPhaseMixtureTypeName);
  if (cstrIter == componentsConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "phaseChangeTwoPhaseMixture::New"
    )
    << "Unknown phaseChangeTwoPhaseMixture type "
    << phaseChangeTwoPhaseMixtureTypeName << endl << endl
    << "Valid  phaseChangeTwoPhaseMixtures are : " << endl
    << componentsConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<phaseChangeTwoPhaseMixture>{cstrIter()(U, phi)};
}

