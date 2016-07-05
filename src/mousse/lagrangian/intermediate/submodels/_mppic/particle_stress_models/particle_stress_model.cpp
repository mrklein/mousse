// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "particle_stress_model.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(ParticleStressModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(ParticleStressModel, dictionary);

}


// Constructors
mousse::ParticleStressModel::ParticleStressModel
(
  const dictionary& dict
)
:
  alphaPacked_{readScalar(dict.lookup("alphaPacked"))}
{}


mousse::ParticleStressModel::ParticleStressModel
(
  const ParticleStressModel& cm
)
:
  alphaPacked_{cm.alphaPacked_}
{}


// Selectors
mousse::autoPtr<mousse::ParticleStressModel> mousse::ParticleStressModel::New
(
  const dictionary& dict
)
{
  word modelType{dict.lookup("type")};
  Info << "Selecting particle stress model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "ParticleStressModel::New"
      "("
        "const dictionary&"
      ")"
    )
    << "Unknown particle stress model type " << modelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid particle stress model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << abort(FatalError);
  }
  return autoPtr<ParticleStressModel>{cstrIter()(dict)};
}


// Destructor
mousse::ParticleStressModel::~ParticleStressModel()
{}


// Member Functions
mousse::scalar mousse::ParticleStressModel::alphaPacked() const
{
  return alphaPacked_;
}


mousse::tmp<mousse::FieldField<mousse::Field, mousse::scalar>>
mousse::ParticleStressModel::tau
(
  const FieldField<Field, scalar>& alpha,
  const FieldField<Field, scalar>& rho,
  const FieldField<Field, scalar>& uRms
) const
{
  tmp<FieldField<Field, scalar>> value
  {
    new FieldField<Field, scalar>{alpha.size()}
  };
  FOR_ALL(alpha, i) {
    value->set(i, tau(alpha[i], rho[i], uRms[i]));
  }
  return value;
}

