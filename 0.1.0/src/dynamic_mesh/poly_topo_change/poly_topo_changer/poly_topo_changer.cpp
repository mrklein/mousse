// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_topo_changer.hpp"
#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(polyTopoChanger, 0);
}
// Constructors 
void mousse::polyTopoChanger::readModifiers()
{
  if
  (
    readOpt() == IOobject::MUST_READ
  || readOpt() == IOobject::MUST_READ_IF_MODIFIED
  || (readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    if (readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
      WARNING_IN("polyTopoChanger::readModifiers()")
        << "Specified IOobject::MUST_READ_IF_MODIFIED but class"
        << " does not support automatic re-reading."
        << endl;
    }
    PtrList<polyMeshModifier>& modifiers = *this;
    // Read modifiers
    Istream& is = readStream(typeName);
    PtrList<entry> patchEntries(is);
    modifiers.setSize(patchEntries.size());
    FOR_ALL(modifiers, modifierI)
    {
      modifiers.set
      (
        modifierI,
        polyMeshModifier::New
        (
          patchEntries[modifierI].keyword(),
          patchEntries[modifierI].dict(),
          modifierI,
          *this
        )
      );
    }
    // Check state of IOstream
    is.check("polyTopoChanger::readModifiers()");
    close();
  }
}
mousse::polyTopoChanger::polyTopoChanger
(
  const IOobject& io,
  polyMesh& mesh
)
:
  PtrList<polyMeshModifier>(),
  regIOobject(io),
  mesh_(mesh)
{
  readModifiers();
}
mousse::polyTopoChanger::polyTopoChanger(polyMesh& mesh)
:
  PtrList<polyMeshModifier>(),
  regIOobject
  (
    IOobject
    (
      "meshModifiers",
      mesh.time().findInstance
      (
        mesh.meshDir(),
        "meshModifiers",
        IOobject::READ_IF_PRESENT
      ),
      mesh.meshSubDir,
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    )
  ),
  mesh_(mesh)
{
  readModifiers();
}
mousse::wordList mousse::polyTopoChanger::types() const
{
  const PtrList<polyMeshModifier>& modifiers = *this;
  wordList t(modifiers.size());
  FOR_ALL(modifiers, modifierI)
  {
    t[modifierI] = modifiers[modifierI].type();
  }
  return t;
}
mousse::wordList mousse::polyTopoChanger::names() const
{
  const PtrList<polyMeshModifier>& modifiers = *this;
  wordList t(modifiers.size());
  FOR_ALL(modifiers, modifierI)
  {
    t[modifierI] = modifiers[modifierI].name();
  }
  return t;
}
bool mousse::polyTopoChanger::changeTopology() const
{
  // Go through all mesh modifiers and accumulate the morphing information
  const PtrList<polyMeshModifier>& topoChanges = *this;
  bool triggerChange = false;
  FOR_ALL(topoChanges, morphI)
  {
    if (topoChanges[morphI].active())
    {
      bool curTriggerChange = topoChanges[morphI].changeTopology();
      if (debug)
      {
        Info<< "Modifier " << morphI << " named "
          << topoChanges[morphI].name();
        if (curTriggerChange)
        {
          Info<< " morphing" << endl;
        }
        else
        {
          Info<< " unchanged" << endl;
        }
      }
      triggerChange = triggerChange || curTriggerChange;
    }
    else
    {
      if (debug)
      {
        Info<< "Modifier " << morphI  << " named "
          << topoChanges[morphI].name() << " inactive" << endl;
      }
    }
  }
  return triggerChange;
}
mousse::autoPtr<mousse::polyTopoChange>
mousse::polyTopoChanger::topoChangeRequest() const
{
  // Collect changes from all modifiers
  const PtrList<polyMeshModifier>& topoChanges = *this;
  polyTopoChange* refPtr(new polyTopoChange(mesh()));
  polyTopoChange& ref = *refPtr;
  FOR_ALL(topoChanges, morphI)
  {
    if (topoChanges[morphI].active())
    {
      topoChanges[morphI].setRefinement(ref);
    }
  }
  return autoPtr<polyTopoChange>(refPtr);
}
void mousse::polyTopoChanger::modifyMotionPoints(pointField& p) const
{
  const PtrList<polyMeshModifier>& topoChanges = *this;
  FOR_ALL(topoChanges, morphI)
  {
    if (topoChanges[morphI].active())
    {
      topoChanges[morphI].modifyMotionPoints(p);
    }
  }
}
void mousse::polyTopoChanger::update(const mapPolyMesh& m)
{
  // Go through all mesh modifiers and accumulate the morphing information
  PtrList<polyMeshModifier>& topoChanges = *this;
  FOR_ALL(topoChanges, morphI)
  {
    topoChanges[morphI].updateMesh(m);
  }
  // Force the mesh modifiers to auto-write.  This allows us to
  // preserve the current state of modifiers corresponding with
  // the mesh.
  writeOpt() = IOobject::AUTO_WRITE;
  instance() = mesh_.time().timeName();
}
mousse::autoPtr<mousse::mapPolyMesh> mousse::polyTopoChanger::changeMesh
(
  const bool inflate,
  const bool syncParallel,
  const bool orderCells,
  const bool orderPoints
)
{
  if (changeTopology())
  {
    autoPtr<polyTopoChange> ref = topoChangeRequest();
    autoPtr<mapPolyMesh> topoChangeMap = ref().changeMesh
    (
      mesh_,
      inflate,
      syncParallel,
      orderCells,
      orderPoints
    );
    update(topoChangeMap());
    mesh_.updateMesh(topoChangeMap());
    return topoChangeMap;
  }
  else
  {
    return autoPtr<mapPolyMesh>(NULL);
  }
}
void mousse::polyTopoChanger::addTopologyModifiers
(
  const List<polyMeshModifier*>& tm
)
{
  setSize(tm.size());
  // Copy the patch pointers
  FOR_ALL(tm, tmI)
  {
    if (tm[tmI]->topoChanger() != *this)
    {
      FATAL_ERROR_IN
      (
        "void polyTopoChanger::addTopologyModifiers"
        "("
          "const List<polyMeshModifier*>&"
        ")"
      )   << "Mesh modifier created with different mesh reference."
        << abort(FatalError);
    }
    set(tmI, tm[tmI]);
  }
  writeOpt() = IOobject::AUTO_WRITE;
}
mousse::label mousse::polyTopoChanger::findModifierID
(
  const word& modName
) const
{
  const PtrList<polyMeshModifier>& topoChanges = *this;
  FOR_ALL(topoChanges, morphI)
  {
    if (topoChanges[morphI].name() == modName)
    {
      return morphI;
    }
  }
  // Modifier not found
  if (debug)
  {
    WARNING_IN("label polyTopoChanger::findModifierID(const word&) const")
      << "Modifier named " << modName << " not found.  "
      << "List of available modifier names: " << names() << endl;
  }
  // Not found, return -1
  return -1;
}
bool mousse::polyTopoChanger::writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}
// Member Operators 
bool mousse::polyTopoChanger::operator!=(const polyTopoChanger& me) const
{
  return &me != this;
}
bool mousse::polyTopoChanger::operator==(const polyTopoChanger& me) const
{
  return &me == this;
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const polyTopoChanger& mme)
{
  os  << mme.size() << nl << token::BEGIN_LIST;
  FOR_ALL(mme, mmeI)
  {
    mme[mmeI].writeDict(os);
  }
  os  << token::END_LIST;
  return os;
}
