// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cylindrical_cs.hpp"
#include "one.hpp"
#include "mathematical_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Constructors 
mousse::cylindricalCS::cylindricalCS(const bool inDegrees)
:
  coordinateSystem(),
  inDegrees_(inDegrees)
{}
mousse::cylindricalCS::cylindricalCS
(
  const coordinateSystem& cs,
  const bool inDegrees
)
:
  coordinateSystem(cs),
  inDegrees_(inDegrees)
{}
mousse::cylindricalCS::cylindricalCS
(
  const word& name,
  const coordinateSystem& cs,
  const bool inDegrees
)
:
  coordinateSystem(name, cs),
  inDegrees_(inDegrees)
{}
mousse::cylindricalCS::cylindricalCS
(
  const word& name,
  const point& origin,
  const coordinateRotation& cr,
  const bool inDegrees
)
:
  coordinateSystem(name, origin, cr),
  inDegrees_(inDegrees)
{}
mousse::cylindricalCS::cylindricalCS
(
  const word& name,
  const point& origin,
  const vector& axis,
  const vector& dirn,
  const bool inDegrees
)
:
  coordinateSystem(name, origin, axis, dirn),
  inDegrees_(inDegrees)
{}
mousse::cylindricalCS::cylindricalCS
(
  const word& name,
  const dictionary& dict
)
:
  coordinateSystem(name, dict),
  inDegrees_(dict.lookupOrDefault("degrees", true))
{}
mousse::cylindricalCS::cylindricalCS
(
  const objectRegistry& obr,
  const dictionary& dict
)
:
  coordinateSystem(obr, dict),
  inDegrees_(dict.lookupOrDefault("degrees", true))
{}
// Destructor 
mousse::cylindricalCS::~cylindricalCS()
{}
// Member Functions 
bool mousse::cylindricalCS::inDegrees() const
{
  return inDegrees_;
}
bool& mousse::cylindricalCS::inDegrees()
{
  return inDegrees_;
}
mousse::vector mousse::cylindricalCS::localToGlobal
(
  const vector& local,
  bool translate
) const
{
  scalar theta
  (
    local.y()*(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
  );
  return coordinateSystem::localToGlobal
  (
    vector(local.x()*cos(theta), local.x()*sin(theta), local.z()),
    translate
  );
}
mousse::tmp<mousse::vectorField> mousse::cylindricalCS::localToGlobal
(
  const vectorField& local,
  bool translate
) const
{
  scalarField theta
  (
    local.component(vector::Y)
   *(inDegrees_ ? constant::mathematical::pi/180.0 : 1.0)
  );
  vectorField lc(local.size());
  lc.replace(vector::X, local.component(vector::X)*cos(theta));
  lc.replace(vector::Y, local.component(vector::X)*sin(theta));
  lc.replace(vector::Z, local.component(vector::Z));
  return coordinateSystem::localToGlobal(lc, translate);
}
mousse::vector mousse::cylindricalCS::globalToLocal
(
  const vector& global,
  bool translate
) const
{
  const vector lc
  (
    coordinateSystem::globalToLocal(global, translate)
  );
  return vector
  (
    sqrt(sqr(lc.x()) + sqr(lc.y())),
    atan2
    (
      lc.y(),
      lc.x()
    )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0),
    lc.z()
  );
}
mousse::tmp<mousse::vectorField> mousse::cylindricalCS::globalToLocal
(
  const vectorField& global,
  bool translate
) const
{
  const vectorField lc
  (
    coordinateSystem::globalToLocal(global, translate)
  );
  tmp<vectorField> tresult(new vectorField(lc.size()));
  vectorField& result = tresult();
  result.replace
  (
    vector::X,
    sqrt(sqr(lc.component(vector::X)) + sqr(lc.component(vector::Y)))
  );
  result.replace
  (
    vector::Y,
    atan2
    (
      lc.component(vector::Y),
      lc.component(vector::X)
    )*(inDegrees_ ? 180.0/constant::mathematical::pi : 1.0)
  );
  result.replace(vector::Z, lc.component(vector::Z));
  return tresult;
}
