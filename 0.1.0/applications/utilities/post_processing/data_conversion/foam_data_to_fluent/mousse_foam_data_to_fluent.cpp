// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "write_fluent_fields.hpp"
#include "ofstream.hpp"
#include "ioobject_list.hpp"

int main(int argc, char *argv[])
{
  argList::noParallel();
  timeSelector::addOptions(false);   // no constant
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_mesh.inc"
  // make a directory called proInterface in the case
  mkDir(runTime.rootPath()/runTime.caseName()/"fluent_interface");
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info<< "Time = " << runTime.timeName() << endl;
    if (mesh.readUpdate())
    {
      Info<< "    Read new mesh" << endl;
    }
    // make a directory called proInterface in the case
    mkDir(runTime.rootPath()/runTime.caseName()/"fluent_interface");
    // open a file for the mesh
    OFstream fluentDataFile
    {
      runTime.rootPath()/
      runTime.caseName()/
      "fluent_interface"/
      runTime.caseName() + runTime.timeName() + ".dat"
    };
    fluentDataFile
      << "(0 \"FOAM to Fluent data File\")" << endl << endl;
    // Writing number of faces
    label nFaces = mesh.nFaces();
    FOR_ALL(mesh.boundary(), patchI)
    {
      nFaces += mesh.boundary()[patchI].size();
    }
    fluentDataFile
      << "(33 (" << mesh.nCells() << " " << nFaces << " "
      << mesh.nPoints() << "))" << endl;
    IOdictionary foamDataToFluentDict
    {
      {
        "foamDataToFluentDict",
        runTime.system(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    };
    // Search for list of objects for this time
    IOobjectList objects{mesh, runTime.timeName()};
    // Converting volScalarField
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Search list of objects for volScalarFields
    IOobjectList scalarFields{objects.lookupClass("volScalarField")};
    FOR_ALL_ITER(IOobjectList, scalarFields, iter)
    {
      // Read field
      volScalarField field{*iter(), mesh};
      // lookup field from dictionary and convert field
      label unitNumber;
      if (foamDataToFluentDict.readIfPresent(field.name(), unitNumber)
          && unitNumber > 0)
      {
        Info<< "    Converting field " << field.name() << endl;
        writeFluentField(field, unitNumber, fluentDataFile);
      }
    }
    // Converting volVectorField
    // Search list of objects for volVectorFields
    IOobjectList vectorFields{objects.lookupClass("volVectorField")};
    FOR_ALL_ITER(IOobjectList, vectorFields, iter)
    {
      // Read field
      volVectorField field{*iter(), mesh};
      // lookup field from dictionary and convert field
      label unitNumber;
      if (foamDataToFluentDict.readIfPresent(field.name(), unitNumber)
          && unitNumber > 0)
      {
        Info<< "    Converting field " << field.name() << endl;
        writeFluentField(field, unitNumber, fluentDataFile);
      }
    }
    Info<< endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
