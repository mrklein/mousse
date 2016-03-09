// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"

template<class FieldType>
void printIntegrate
(
  const fvMesh& mesh,
  const IOobject& fieldHeader,
  const label patchI,
  bool& done
)
{
  if (!done && fieldHeader.headerClassName() == FieldType::typeName)
  {
    Info << "    Reading " << fieldHeader.headerClassName() << " "
      << fieldHeader.name() << endl;
    FieldType f{fieldHeader, mesh};
    Info << "    Integral of " << fieldHeader.name()
      << " over vector area of patch "
      << mesh.boundary()[patchI].name() << '[' << patchI << ']' << " = "
      << gSum(mesh.Sf().boundaryField()[patchI]*f.boundaryField()[patchI])
      << nl;
    Info << "    Integral of " << fieldHeader.name()
      << " over area magnitude of patch "
      << mesh.boundary()[patchI].name() << '[' << patchI << ']' << " = "
      << gSum(mesh.magSf().boundaryField()[patchI]*f.boundaryField()[patchI])
      << nl;
    done = true;
  }
}

template<class FieldType>
void printSum
(
  const fvMesh& mesh,
  const IOobject& fieldHeader,
  const label patchI,
  bool& done
)
{
  if (!done && fieldHeader.headerClassName() == FieldType::typeName)
  {
    Info << "    Reading " << FieldType::typeName << " "
      << fieldHeader.name() << endl;
    FieldType f(fieldHeader, mesh);
    typename FieldType::value_type sumField = gSum(f.boundaryField()[patchI]);
    Info << "    Integral of " << fieldHeader.name() << " over patch "
      << mesh.boundary()[patchI].name() << '[' << patchI << ']' << " = "
      << sumField << nl;
    done = true;
  }
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
  FOR_ALL(timeDirs, timeI)
  {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    IOobject fieldHeader
    {
      fieldName,
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check field exists
    if (fieldHeader.headerOk())
    {
      mesh.readUpdate();
      const label patchI = mesh.boundaryMesh().findPatchID(patchName);
      if (patchI < 0)
      {
        FatalError
          << "Unable to find patch " << patchName << nl
          << exit(FatalError);
      }
      // Give patch area
      Info << "    Area vector of patch "
        << patchName << '[' << patchI << ']' << " = "
        << gSum(mesh.Sf().boundaryField()[patchI]) << endl;
      Info << "    Area magnitude of patch "
        << patchName << '[' << patchI << ']' << " = "
        << gSum(mesh.magSf().boundaryField()[patchI]) << endl;
      // Read field and calc integral
      bool done = false;
      printIntegrate<volScalarField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printIntegrate<volVectorField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printSum<surfaceScalarField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printSum<surfaceVectorField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printSum<volSphericalTensorField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printSum<volSymmTensorField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      printSum<volTensorField>
      (
        mesh,
        fieldHeader,
        patchI,
        done
      );
      if (!done)
      {
        FatalError
          << "Only possible to integrate "
          << "volFields and surfaceFields."
          << " Field " << fieldName << " is of type "
          << fieldHeader.headerClassName()
          << nl << exit(FatalError);
      }
    }
    else
    {
      Info << "    No field " << fieldName << endl;
    }
    Info << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
