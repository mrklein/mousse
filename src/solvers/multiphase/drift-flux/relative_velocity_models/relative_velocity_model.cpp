// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "relative_velocity_model.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(relativeVelocityModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(relativeVelocityModel, dictionary);
}
// Constructors 
mousse::relativeVelocityModel::relativeVelocityModel
(
  const dictionary& /*dict*/,
  const incompressibleTwoPhaseInteractingMixture& mixture
)
:
  mixture_{mixture},
  alphac_{mixture.alpha2()},
  alphad_{mixture.alpha1()},
  rhoc_{mixture.rhoc()},
  rhod_{mixture.rhod()},
  Udm_
  {
    {
      "Udm",
      alphac_.time().timeName(),
      alphac_.mesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    alphac_.mesh(),
    {"Udm", dimVelocity, vector::zero},
    mixture.U().boundaryField().types()
  }
{}


// Selectors
mousse::autoPtr<mousse::relativeVelocityModel> mousse::relativeVelocityModel::New
(
  const dictionary& dict,
  const incompressibleTwoPhaseInteractingMixture& mixture
)
{
  word modelType{dict.lookup(typeName)};
  Info << "Selecting relative velocity model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "relativeVelocityModel::New"
      "("
        "const dictionary&"
      ")"
    )
    << "Unknown time scale model type " << modelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid time scale model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << abort(FatalError);
  }
  return {cstrIter()(dict.subDict(modelType + "Coeffs"), mixture)};
}


// Destructor 
mousse::relativeVelocityModel::~relativeVelocityModel()
{}


// Member Functions 
tmp<volScalarField> mousse::relativeVelocityModel::rho() const
{
  return alphac_*rhoc_ + alphad_*rhod_;
}


tmp<volSymmTensorField> mousse::relativeVelocityModel::tauDm() const
{
  volScalarField betac{alphac_*rhoc_};
  volScalarField betad{alphad_*rhod_};
  // Calculate the relative velocity of the continuous phase w.r.t the mean
  volVectorField Ucm{betad*Udm_/betac};
  return
    tmp<volSymmTensorField>
    {
      new volSymmTensorField
      {
        "tauDm",
        betad*sqr(Udm_) + betac*sqr(Ucm)
      }
    };
}

