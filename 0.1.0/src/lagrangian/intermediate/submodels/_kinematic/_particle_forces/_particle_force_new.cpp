// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_force.hpp"
// Member Functions 
template<class CloudType>
mousse::autoPtr<mousse::ParticleForce<CloudType> >
mousse::ParticleForce<CloudType>::New
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& forceType
)
{
  Info<< "    Selecting particle force " << forceType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(forceType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "ParticleForce<CloudType>::New"
      "("
        "CloudType&, "
        "const fvMesh&, "
        "const dictionary&"
      ")"
    )   << "Unknown particle force type "
      << forceType
      << ", constructor not in hash table" << nl << nl
      << "    Valid particle force types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<ParticleForce<CloudType> >
  (
    cstrIter()
    (
      owner,
      mesh,
      dict
    )
  );
}
