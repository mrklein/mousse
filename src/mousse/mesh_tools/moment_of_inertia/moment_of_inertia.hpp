#ifndef MESH_TOOLS_MOMENT_OF_INERTIA_MOMENT_OF_INERTIA_HPP_
#define MESH_TOOLS_MOMENT_OF_INERTIA_MOMENT_OF_INERTIA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::momentOfInertia
// Description
//   Calculates the inertia tensor and principal axes and moments of a
//   polyhedra/cells/triSurfaces.  Inertia can either be of the solid body or
//   of a thin shell.

#include "tri_face_list.hpp"
#include "tri_surface.hpp"
#include "poly_mesh.hpp"


namespace mousse {

class momentOfInertia
{
public:
  static void massPropertiesSolid
  (
    const pointField& pts,
    const triFaceList& triFaces,
    scalar density,
    scalar& mass,
    vector& cM,
    tensor& J
  );
  static void massPropertiesShell
  (
    const pointField& pts,
    const triFaceList& triFaces,
    scalar density,
    scalar& mass,
    vector& cM,
    tensor& J
  );
  static void massPropertiesSolid
  (
    const triSurface& surf,
    scalar density,
    scalar& mass,
    vector& cM,
    tensor& J
  );
  static void massPropertiesShell
  (
    const triSurface& surf,
    scalar density,
    scalar& mass,
    vector& cM,
    tensor& J
  );
  static tensor applyParallelAxisTheorem
  (
    scalar mass,
    const vector& cM,
    const tensor& J,
    const vector& refPt
  );
  // Calculate the inertia tensor for all cells in the mesh
  static tmp<tensorField> meshInertia
  (
    const polyMesh& mesh
  );
  // Calculate the inertia tensor the given cell
  static tensor meshInertia
  (
    const polyMesh& mesh,
    label cellI
  );
};

}  // namespace mousse
#endif

