// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "attach_poly_topo_changer.hpp"
#include "poly_mesh.hpp"
#include "poly_topo_change.hpp"
// Constructors 
mousse::attachPolyTopoChanger::attachPolyTopoChanger
(
  const IOobject& io,
  polyMesh& mesh
)
:
  polyTopoChanger(io, mesh)
{}
mousse::attachPolyTopoChanger::attachPolyTopoChanger
(
  polyMesh& mesh
)
:
  polyTopoChanger(mesh)
{}
// Member Functions 
//- Attach mesh
void mousse::attachPolyTopoChanger::attach(const bool removeEmptyPatches)
{
  if (debug)
  {
    Pout<< "void attachPolyTopoChanger::attach(): "
      << "Attaching mesh" << endl;
  }
  // Save current file instance
  const fileName oldInst = mesh_.facesInstance();
  // Execute all polyMeshModifiers
  changeMesh(false);  // no inflation
  const pointField p = mesh_.oldPoints();
  mesh_.movePoints(p);
  if (debug)
  {
    Pout<< "Clearing mesh." << endl;
  }
  if (removeEmptyPatches)
  {
    // Re-do the boundary patches, removing the ones with zero size
    const polyBoundaryMesh& oldPatches = mesh_.boundaryMesh();
    List<polyPatch*> newPatches(oldPatches.size());
    label nNewPatches = 0;
    FOR_ALL(oldPatches, patchI)
    {
      if (oldPatches[patchI].size())
      {
        newPatches[nNewPatches] = oldPatches[patchI].clone
        (
          mesh_.boundaryMesh(),
          nNewPatches,
          oldPatches[patchI].size(),
          oldPatches[patchI].start()
        ).ptr();
        nNewPatches++;
      }
      else
      {
        if (debug)
        {
          Pout<< "Removing zero-sized patch " << patchI
            << " named " << oldPatches[patchI].name() << endl;
        }
      }
    }
    newPatches.setSize(nNewPatches);
    mesh_.removeBoundary();
    mesh_.addPatches(newPatches);
  }
  // Reset the file instance to overwrite the original mesh
  mesh_.setInstance(oldInst);
  if (debug)
  {
    Pout<< "void attachPolyTopoChanger::attach(): "
      << "Finished attaching mesh" << endl;
  }
  mesh_.checkMesh();
}
