// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "set_updater.hpp"
#include "poly_topo_changer.hpp"
#include "poly_topo_change.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map_poly_mesh.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "point_set.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(setUpdater, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    polyMeshModifier,
    setUpdater,
    dictionary
  );
}
// Private Member Functions 
// Constructors 
// Construct from dictionary
mousse::setUpdater::setUpdater
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyTopoChanger& mme
)
:
  polyMeshModifier(name, index, mme, Switch(dict.lookup("active")))
{}
// Destructor 
mousse::setUpdater::~setUpdater()
{}
// Member Functions 
bool mousse::setUpdater::changeTopology() const
{
  // I am never cause of changeTopo
  return false;
}
void mousse::setUpdater::setRefinement(polyTopoChange&) const
{}
void mousse::setUpdater::modifyMotionPoints(pointField&) const
{}
void mousse::setUpdater::updateMesh(const mapPolyMesh& morphMap)
{
  // Mesh has changed topologically. Update all sets.
  if (debug)
  {
    Pout<< "setUpdater::updateMesh(const mapPolyMesh& morphMap)"
      << endl;
  }
  updateSets<cellSet>(morphMap);
  updateSets<faceSet>(morphMap);
  updateSets<pointSet>(morphMap);
}
void mousse::setUpdater::write(Ostream& os) const
{
  os  << nl << type() << nl;
}
void mousse::setUpdater::writeDict(Ostream& os) const
{
  os  << nl << name() << nl << token::BEGIN_BLOCK << nl
    << "    type " << type()
    << token::END_STATEMENT << nl
    << "    active " << active()
    << token::END_STATEMENT << nl
    << token::END_BLOCK << endl;
}
// Member Operators 
// Friend Functions 
// Friend Operators
