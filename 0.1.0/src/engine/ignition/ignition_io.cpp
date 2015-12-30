// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "engine_time.hpp"
#include "ignition.hpp"
// Constructors 
mousse::ignition::ignition
(
  const dictionary& combustionProperties,
  const Time& db,
  const fvMesh& mesh
)
:
  mesh_(mesh),
  ignite_(combustionProperties.lookup("ignite")),
  ignSites_
  (
    combustionProperties.lookup("ignitionSites"),
    ignitionSite::iNew(db, mesh)
  )
{
  if (ignite_)
  {
    Info<< "\nIgnition on" << endl;
  }
  else
  {
    Info<< "\nIgnition switched off" << endl;
  }
}
mousse::ignition::ignition
(
  const dictionary& combustionProperties,
  const engineTime& edb,
  const fvMesh& mesh
)
:
  mesh_(mesh),
  ignite_(combustionProperties.lookup("ignite")),
  ignSites_
  (
    combustionProperties.lookup("ignitionSites"),
    ignitionSite::iNew(edb, mesh)
  )
{
  if (ignite_)
  {
    Info<< "\nIgnition on" << endl;
  }
  else
  {
    Info<< "\nIgnition switched off" << endl;
  }
}
