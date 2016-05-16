// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"


template<class FieldType>
void printAverage
(
  const fvMesh& mesh,
  const IOobject& fH,
  const scalar area,
  const label pI,
  bool& done
)
{
  if (done || fH.headerClassName() != FieldType::typeName)
    return;
  Info << "    Reading " << fH.headerClassName() << " "
    << fH.name() << endl;
  FieldType f{fH, mesh};
  typename FieldType::value_type sumField =
    pTraits<typename FieldType::value_type>::zero;
  if (area > 0) {
    sumField =
      gSum(mesh.magSf().boundaryField()[pI]*f.boundaryField()[pI])/area;
  }
  Info << "    Average of " << fH.headerClassName()
    << " over patch "
    << mesh.boundary()[pI].name()
    << '[' << pI << ']' << " = "
    << sumField << endl;
  done = true;
}


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  #include "add_region_option.inc"
  argList::validArgs.append("fieldName");
  argList::validArgs.append("patchName");
  #include "set_root_case.inc"
  #include "create_time.inc"
  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  const word fieldName = args[1];
  const word patchName = args[2];
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    IOobject io
    {
      fieldName,
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check field exists
    if (io.headerOk()) {
      mesh.readUpdate();
      const label patchI = mesh.boundaryMesh().findPatchID(patchName);
      if (patchI < 0) {
        FatalError
          << "Unable to find patch " << patchName << nl
          << exit(FatalError);
      }
      scalar area = gSum(mesh.magSf().boundaryField()[patchI]);
      bool done = false;
      printAverage<volScalarField>(mesh, io, area, patchI, done);
      printAverage<volVectorField>(mesh, io, area, patchI, done);
      printAverage<volSphericalTensorField>(mesh, io, area, patchI, done);
      printAverage<volSymmTensorField>(mesh, io, area, patchI, done);
      printAverage<volTensorField>(mesh, io, area, patchI, done);
      if (!done) {
        FatalError
          << "Only possible to average volFields."
          << " Field " << fieldName << " is of type "
          << io.headerClassName()
          << nl << exit(FatalError);
      }
    } else {
      Info << "    No field " << fieldName << endl;
    }
    Info << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

