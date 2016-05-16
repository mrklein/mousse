// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "point_mesh.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "hex_ref8.hpp"
#include "cell_set.hpp"
#include "ofstream.hpp"
#include "mesh_tools.hpp"
#include "ifstream.hpp"
#include "poly_topo_change.hpp"
#include "map_poly_mesh.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "point_fields.hpp"
#include "_read_fields.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  #include "add_region_option.inc"
  argList::validArgs.append("cellSet");
  argList::addBoolOption
  (
    "minSet",
    "remove cells from input cellSet to keep to 2:1 ratio"
    " (default is to extend set)"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_named_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  word cellSetName{args.args()[1]};
  const bool overwrite = args.optionFound("overwrite");
  const bool minSet = args.optionFound("minSet");
  Info << "Reading cells to refine from cellSet " << cellSetName
    << nl << endl;
  cellSet cellsToRefine{mesh, cellSetName};
  Info << "Read " << returnReduce(cellsToRefine.size(), sumOp<label>())
    << " cells to refine from cellSet " << cellSetName << nl
    << endl;
  // Read objects in time directory
  IOobjectList objects{mesh, runTime.timeName()};
  // Read vol fields.
  PtrList<volScalarField> vsFlds;
  ReadFields(mesh, objects, vsFlds);
  PtrList<volVectorField> vvFlds;
  ReadFields(mesh, objects, vvFlds);
  PtrList<volSphericalTensorField> vstFlds;
  ReadFields(mesh, objects, vstFlds);
  PtrList<volSymmTensorField> vsymtFlds;
  ReadFields(mesh, objects, vsymtFlds);
  PtrList<volTensorField> vtFlds;
  ReadFields(mesh, objects, vtFlds);
  // Read surface fields.
  PtrList<surfaceScalarField> ssFlds;
  ReadFields(mesh, objects, ssFlds);
  PtrList<surfaceVectorField> svFlds;
  ReadFields(mesh, objects, svFlds);
  PtrList<surfaceSphericalTensorField> sstFlds;
  ReadFields(mesh, objects, sstFlds);
  PtrList<surfaceSymmTensorField> ssymtFlds;
  ReadFields(mesh, objects, ssymtFlds);
  PtrList<surfaceTensorField> stFlds;
  ReadFields(mesh, objects, stFlds);
  // Read point fields
  PtrList<pointScalarField> psFlds;
  ReadFields(pointMesh::New(mesh), objects, psFlds);
  PtrList<pointVectorField> pvFlds;
  ReadFields(pointMesh::New(mesh), objects, pvFlds);
  // Construct refiner without unrefinement. Read existing point/cell level.
  hexRef8 meshCutter{mesh};
  // Some stats
  Info
    << "Read mesh:" << nl
    << "    cells:" << mesh.globalData().nTotalCells() << nl
    << "    faces:" << mesh.globalData().nTotalFaces() << nl
    << "    points:" << mesh.globalData().nTotalPoints() << nl
    << "    cellLevel :"
    << " min:" << gMin(meshCutter.cellLevel())
    << " max:" << gMax(meshCutter.cellLevel()) << nl
    << "    pointLevel :"
    << " min:" << gMin(meshCutter.pointLevel())
    << " max:" << gMax(meshCutter.pointLevel()) << nl
    << endl;
  // Maintain 2:1 ratio
  labelList newCellsToRefine
  {
    meshCutter.consistentRefinement
    (
      cellsToRefine.toc(),
      !minSet                 // extend set
    )
  };
  // Mesh changing engine.
  polyTopoChange meshMod{mesh};
  // Play refinement commands into mesh changer.
  meshCutter.setRefinement(newCellsToRefine, meshMod);
  if (!overwrite) {
    runTime++;
  }
  // Create mesh, return map from old to new mesh.
  autoPtr<mapPolyMesh> map = meshMod.changeMesh(mesh, false);
  // Update fields
  mesh.updateMesh(map);
  // Update numbering of cells/vertices.
  meshCutter.updateMesh(map);
  // Optionally inflate mesh
  if (map().hasMotionPoints()) {
    mesh.movePoints(map().preMotionPoints());
  }
  Info << "Refined from " << returnReduce(map().nOldCells(), sumOp<label>())
    << " to " << mesh.globalData().nTotalCells() << " cells." << nl << endl;
  if (overwrite) {
    mesh.setInstance(oldInstance);
    meshCutter.setInstance(oldInstance);
  }
  Info << "Writing mesh to " << runTime.timeName() << endl;
  mesh.write();
  meshCutter.write();
  Info << "End\n" << endl;
  return 0;
}

