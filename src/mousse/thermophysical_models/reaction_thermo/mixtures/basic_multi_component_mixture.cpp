// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_multi_component_mixture.hpp"


// Static Member Functions
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(basicMultiComponentMixture, 0);

}


// Constructors 
mousse::basicMultiComponentMixture::basicMultiComponentMixture
(
  const dictionary& /*thermoDict*/,
  const wordList& specieNames,
  const fvMesh& mesh,
  const word& phaseName
)
:
  species_{specieNames},
  Y_{species_.size()}
{
  FOR_ALL(species_, i) {
    IOobject header
    {
      IOobject::groupName(species_[i], phaseName),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ
    };
    // Check if field exists and can be read
    if (header.headerOk()) {
      Y_.set
      (
        i,
        new volScalarField
        {
          IOobject
          {
            IOobject::groupName(species_[i], phaseName),
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
          },
          mesh
        }
      );
    } else {
      volScalarField Ydefault
      {
        IOobject
        {
          "Ydefault",
          mesh.time().timeName(),
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        mesh
      };
      Y_.set
      (
        i,
        new volScalarField
        {
          IOobject
          {
            IOobject::groupName(species_[i], phaseName),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
          },
          Ydefault
        }
      );
    }
  }
  // Do not enforce constraint of sum of mass fractions to equal 1 here
  // - not applicable to all models
}

