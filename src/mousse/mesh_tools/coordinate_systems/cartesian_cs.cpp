// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cartesian_cs.hpp"
#include "one.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cartesianCS, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(coordinateSystem, cartesianCS, dictionary);

}


// Constructors
mousse::cartesianCS::cartesianCS()
:
  coordinateSystem{}
{}


mousse::cartesianCS::cartesianCS
(
  const coordinateSystem& cs
)
:
  coordinateSystem{cs}
{}


mousse::cartesianCS::cartesianCS
(
  const word& name,
  const coordinateSystem& cs
)
:
  coordinateSystem{name, cs}
{}


mousse::cartesianCS::cartesianCS
(
  const word& name,
  const point& origin,
  const coordinateRotation& cr
)
:
  coordinateSystem{name, origin, cr}
{}


mousse::cartesianCS::cartesianCS
(
  const word& name,
  const point& origin,
  const vector& axis,
  const vector& dirn
)
:
  coordinateSystem{name, origin, axis, dirn}
{}


mousse::cartesianCS::cartesianCS
(
  const word& name,
  const dictionary& dict
)
:
  coordinateSystem{name, dict}
{}


mousse::cartesianCS::cartesianCS
(
  const objectRegistry& obr,
  const dictionary& dict
)
:
  coordinateSystem{obr, dict}
{}


// Destructor
mousse::cartesianCS::~cartesianCS()
{}


// Member Functions
mousse::vector mousse::cartesianCS::localToGlobal
(
  const vector& local,
  bool translate
) const
{
  return coordinateSystem::localToGlobal(local, translate);
}


mousse::tmp<mousse::vectorField> mousse::cartesianCS::localToGlobal
(
  const vectorField& local,
  bool translate
) const
{
  return coordinateSystem::localToGlobal(local, translate);
}


mousse::vector mousse::cartesianCS::globalToLocal
(
  const vector& global,
  bool translate
) const
{
  return coordinateSystem::globalToLocal(global, translate);
}


mousse::tmp<mousse::vectorField> mousse::cartesianCS::globalToLocal
(
  const vectorField& global,
  bool translate
) const
{
  return coordinateSystem::globalToLocal(global, translate);
}

