// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(cloud, 0);
const word cloud::prefix("lagrangian");
word cloud::defaultName("defaultCloud");

}
// Constructors
mousse::cloud::cloud(const objectRegistry& obr, const word& cloudName)
:
  objectRegistry{
    IOobject{ 
      (cloudName.size() ? cloudName : defaultName),
      obr.time().timeName(),
      prefix,
      obr,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    }
  }
{}

// Destructor
mousse::cloud::~cloud()
{}

// Member Functions
void mousse::cloud::autoMap(const mapPolyMesh&)
{
  NOT_IMPLEMENTED("cloud::autoMap(const mapPolyMesh&)");
}
