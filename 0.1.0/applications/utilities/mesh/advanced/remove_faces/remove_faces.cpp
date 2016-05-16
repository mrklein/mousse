// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "poly_topo_change.hpp"
#include "face_set.hpp"
#include "remove_faces.hpp"
#include "_read_fields.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  #include "add_overwrite_option.inc"
  argList::validArgs.append("faceSet");
  #include "set_root_case.inc"
  #include "create_time.inc"
  runTime.functionObjects().off();
  #include "create_mesh.inc"
  const word oldInstance = mesh.pointsInstance();
  const word setName = args[1];
  const bool overwrite = args.optionFound("overwrite");
  // Read faces
  faceSet candidateSet{mesh, setName};
  Pout << "Read " << candidateSet.size() << " faces to remove" << nl
    << endl;
  labelList candidates{candidateSet.toc()};
  // Face removal engine. No checking for not merging boundary faces.
  removeFaces faceRemover{mesh, 2};
  // Get compatible set of faces and connected sets of cells.
  labelList cellRegion;
  labelList cellRegionMaster;
  labelList facesToRemove;
  faceRemover.compatibleRemoves
    (
      candidates,
      cellRegion,
      cellRegionMaster,
      facesToRemove
    );

  {
    faceSet t{mesh, "compatibleRemoves", facesToRemove};
    Pout << "Original faces to be removed:" << candidateSet.size() << nl
      << "New faces to be removed:" << t.size() << nl
      << endl;
    Pout << "Writing new faces to be removed to faceSet "
      << t.instance()/t.local()/t.name()
      << endl;
    t.write();
  }
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
  // Topo changes container
  polyTopoChange meshMod{mesh};
  // Insert mesh refinement into polyTopoChange.
  faceRemover.setRefinement
    (
      facesToRemove,
      cellRegion,
      cellRegionMaster,
      meshMod
    );
  autoPtr<mapPolyMesh> morphMap = meshMod.changeMesh(mesh, false);
  mesh.updateMesh(morphMap);
  // Move mesh (since morphing does not do this)
  if (morphMap().hasMotionPoints()) {
    mesh.movePoints(morphMap().preMotionPoints());
  }
  // Update numbering of cells/vertices.
  faceRemover.updateMesh(morphMap);
  if (!overwrite) {
    runTime++;
  } else {
    mesh.setInstance(oldInstance);
  }
  // Take over refinement levels and write to new time directory.
  Pout << "Writing mesh to time " << runTime.timeName() << endl;
  mesh.write();
  Pout << "End\n" << endl;
  return 0;
}

