// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh_modifier.hpp"
#include "dictionary.hpp"
#include "poly_topo_changer.hpp"
#include "poly_mesh.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(polyMeshModifier, 0);
  defineRunTimeSelectionTable(polyMeshModifier, dictionary);
}
// Constructors 
// Construct from components
mousse::polyMeshModifier::polyMeshModifier
(
  const word& name,
  const label index,
  const polyTopoChanger& mme,
  const bool act
)
:
  name_(name),
  index_(index),
  topoChanger_(mme),
  active_(act)
{}
// Destructor 
mousse::polyMeshModifier::~polyMeshModifier()
{}
// Member Functions 
const mousse::polyTopoChanger& mousse::polyMeshModifier::topoChanger() const
{
  return topoChanger_;
}
// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const polyMeshModifier& pmm)
{
  pmm.write(os);
  os.check("Ostream& operator<<(Ostream& f, const polyMeshModifier& pmm)");
  return os;
}
