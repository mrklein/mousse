// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "tri_surface.hpp"
#include "tri_surface_search.hpp"
#include "cell_set.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Create a cellSet for cells with their centres inside the defined "
    "surface.\n"
    "Surface must be closed and singly connected."
  );
  argList::noParallel();
  argList::validArgs.append("surfaceFile");
  argList::validArgs.append("cellSet");
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_poly_mesh.inc"
  const fileName surfName = args[1];
  const fileName setName  = args[2];
  // Read surface
  Info << "Reading surface from " << surfName << endl;
  triSurface surf{surfName};
  // Destination cellSet.
  cellSet insideCells{mesh, setName, IOobject::NO_READ};
  // Construct search engine on surface
  triSurfaceSearch querySurf{surf};
  boolList inside{querySurf.calcInside(mesh.cellCentres())};
  FOR_ALL(inside, cellI) {
    if (inside[cellI]) {
      insideCells.insert(cellI);
    }
  }
  Info << "Selected " << insideCells.size() << " of " << mesh.nCells()
    << " cells" << nl << nl
    << "Writing selected cells to cellSet " << insideCells.name()
    << nl << nl
    << "Use this cellSet e.g. with subsetMesh : " << nl << nl
    << "    subsetMesh " << insideCells.name()
    << nl << endl;
  insideCells.write();
  Info << "End\n" << endl;
  return 0;
}
