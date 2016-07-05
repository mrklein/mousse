// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "time.hpp"
#include "_read_fields.hpp"
#include "single_cell_fv_mesh.hpp"
#include "time_selector.hpp"


using namespace mousse;


// Name of region to create
const string singleCellName = "singleCell";


template<class GeoField>
void interpolateFields
(
  const singleCellFvMesh& scMesh,
  const PtrList<GeoField>& flds
)
{
  FOR_ALL(flds, i) {
    tmp<GeoField> scFld = scMesh.interpolate(flds[i]);
    GeoField* scFldPtr = scFld.ptr();
    scFldPtr->writeOpt() = IOobject::AUTO_WRITE;
    scFldPtr->store();
  }
}


int main(int argc, char *argv[])
{
  // constant, not false
  timeSelector::addOptions(true, false);
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  if (regionName == singleCellName) {
    FATAL_ERROR_IN(args.executable())
      << "Cannot convert region " << singleCellName
      << " since result would overwrite it. Please rename your region."
      << exit(FatalError);
  }
  // Create the mesh
  Info << "Creating singleCell mesh" << nl << endl;
  autoPtr<singleCellFvMesh> scMesh
  {
    new singleCellFvMesh
    {
      {
        singleCellName,
        mesh.polyMesh::instance(),
        runTime,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
      },
      mesh
    }
  };
  // For convenience create any fv* files
  if (!exists(scMesh().fvSolution::objectPath())) {
    mkDir(scMesh().fvSolution::path());
    ln("../fvSolution", scMesh().fvSolution::objectPath());
  }
  if (!exists(scMesh().fvSchemes::objectPath())) {
    mkDir(scMesh().fvSolution::path());
    ln("../fvSchemes", scMesh().fvSchemes::objectPath());
  }
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << nl << "Time = " << runTime.timeName() << endl;
    // Check for new mesh
    if (mesh.readUpdate() != polyMesh::UNCHANGED) {
      Info << "Detected changed mesh. Recreating singleCell mesh." << endl;
      scMesh.clear(); // remove any registered objects
      scMesh.reset
      (
        new singleCellFvMesh
        {
          {
            singleCellName,
            mesh.polyMesh::instance(),
            runTime,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
          },
          mesh
        }
      );
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
    // Map and store the fields on the scMesh.
    interpolateFields(scMesh(), vsFlds);
    interpolateFields(scMesh(), vvFlds);
    interpolateFields(scMesh(), vstFlds);
    interpolateFields(scMesh(), vsymtFlds);
    interpolateFields(scMesh(), vtFlds);
    // Write
    Info << "Writing mesh to time " << runTime.timeName() << endl;
    scMesh().write();
  }
  Info << "End\n" << endl;
  return 0;
}

