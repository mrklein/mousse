// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "point_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
#include "_read_fields.hpp"
#include "fv_io_option_list.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"


using namespace mousse;


// Read all fields of type. Returns names of fields read. Guarantees all
// processors to read fields in same order.
template<class GeoField>
wordList ReadUniformFields
(
  const IOobjectList& objects,
  PtrList<GeoField>& fields,
  const bool syncPar
)
{
  // Search list of objects for wanted type
  IOobjectList fieldObjects{objects.lookupClass(GeoField::typeName)};
  wordList masterNames{fieldObjects.names()};
  if (syncPar && Pstream::parRun()) {
    // Check that I have the same fields as the master
    const wordList localNames{masterNames};
    Pstream::scatter(masterNames);
    HashSet<word> localNamesSet{localNames};
    FOR_ALL(masterNames, i) {
      const word& masterFld = masterNames[i];
      HashSet<word>::iterator iter = localNamesSet.find(masterFld);
      if (iter == localNamesSet.end()) {
        FATAL_ERROR_IN
        (
          "ReadFields<class GeoField>"
          "(const IOobjectList&, PtrList<GeoField>&, const bool)"
        )
        << "Fields not synchronised across processors." << endl
        << "Master has fields " << masterNames
        << "  processor " << Pstream::myProcNo()
        << " has fields " << localNames << exit(FatalError);
      } else {
        localNamesSet.erase(iter);
      }
    }
    FOR_ALL_CONST_ITER(HashSet<word>, localNamesSet, iter) {
      FATAL_ERROR_IN
      (
        "ReadFields<class GeoField>"
        "(const IOobjectList&, PtrList<GeoField>&, const bool)"
      )
      << "Fields not synchronised across processors." << endl
      << "Master has fields " << masterNames
      << "  processor " << Pstream::myProcNo()
      << " has fields " << localNames << exit(FatalError);
    }
  }
  fields.setSize(masterNames.size());
  // Make sure to read in masterNames order.
  FOR_ALL(masterNames, i) {
    Info << "Reading " << GeoField::typeName << ' ' << masterNames[i] << endl;
    const IOobject& io = *fieldObjects[masterNames[i]];
    fields.set
    (
      i,
      new GeoField
      {
        {
          io.name(),
          io.instance(),
          io.local(),
          io.db(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE,
          io.registerObject()
        }
      }
    );
  }
  return masterNames;
}


void calc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh,
  functionObjectList& fol
)
{
  if (args.optionFound("noFlow")) {
    Info << "    Operating in no-flow mode; no models will be loaded."
      << " All vol, surface and point fields will be loaded." << endl;
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
    // Read vol-internal fields.
    PtrList<volScalarField::DimensionedInternalField> vsiFlds;
    ReadFields(mesh, objects, vsiFlds);
    PtrList<volVectorField::DimensionedInternalField> vviFlds;
    ReadFields(mesh, objects, vviFlds);
    PtrList<volSphericalTensorField::DimensionedInternalField> vstiFlds;
    ReadFields(mesh, objects, vstiFlds);
    PtrList<volSymmTensorField::DimensionedInternalField> vsymtiFlds;
    ReadFields(mesh, objects, vsymtiFlds);
    PtrList<volTensorField::DimensionedInternalField> vtiFlds;
    ReadFields(mesh, objects, vtiFlds);
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
    // Read point fields.
    const pointMesh& pMesh = pointMesh::New(mesh);
    PtrList<pointScalarField> psFlds;
    ReadFields(pMesh, objects, psFlds);
    PtrList<pointVectorField> pvFlds;
    ReadFields(pMesh, objects, pvFlds);
    PtrList<pointSphericalTensorField> pstFlds;
    ReadFields(pMesh, objects, pstFlds);
    PtrList<pointSymmTensorField> psymtFlds;
    ReadFields(pMesh, objects, psymtFlds);
    PtrList<pointTensorField> ptFlds;
    ReadFields(pMesh, objects, ptFlds);
    // Read uniform dimensioned fields
    IOobjectList constantObjects{mesh, runTime.constant()};
    PtrList<uniformDimensionedScalarField> usFlds;
    ReadUniformFields(constantObjects, usFlds, true);
    PtrList<uniformDimensionedVectorField> uvFlds;
    ReadUniformFields(constantObjects, uvFlds, true);
    PtrList<uniformDimensionedSphericalTensorField> ustFlds;
    ReadUniformFields(constantObjects, ustFlds, true);
    PtrList<uniformDimensionedSymmTensorField> usymmtFlds;
    ReadUniformFields(constantObjects, usymmtFlds, true);
    PtrList<uniformDimensionedTensorField> utFlds;
    ReadUniformFields(constantObjects, utFlds, true);
    fol.execute(true);
  } else {
    Info << "    Reading phi" << endl;
    surfaceScalarField phi
    {
      {
        "phi",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    Info << "    Reading U" << endl;
    volVectorField U
    {
      {
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    Info << "    Reading p" << endl;
    volScalarField p
    {
      {
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ
      },
      mesh
    };
    #include "create_fv_options.inc"
    if (phi.dimensions() == dimVolume/dimTime) {
      IOobject turbulencePropertiesHeader
      {
        "turbulenceProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      };
      if (turbulencePropertiesHeader.headerOk()) {
        singlePhaseTransportModel laminarTransport{U, phi};
        autoPtr<incompressible::turbulenceModel> turbulenceModel
        {
          incompressible::turbulenceModel::New
          (
            U,
            phi,
            laminarTransport
          )
        };
        fol.execute(true);
      } else {
        IOdictionary transportProperties
        {
          {
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
          }
        };
        fol.execute(true);
      }
    } else if (phi.dimensions() == dimMass/dimTime) {
      autoPtr<fluidThermo> thermo{fluidThermo::New(mesh)};
      volScalarField rho
      {
        {
          "rho",
          runTime.timeName(),
          mesh
        },
        thermo->rho()
      };
      IOobject turbulencePropertiesHeader
      {
        "turbulenceProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      };
      if (turbulencePropertiesHeader.headerOk()) {
        autoPtr<compressible::turbulenceModel> turbulenceModel
        {
          compressible::turbulenceModel::New
          (
            rho,
            U,
            phi,
            thermo()
          )
        };
        fol.execute(true);
      } else {
        IOdictionary transportProperties
        {
          {
            "transportProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
          }
        };
        fol.execute(true);
      }
    } else {
      FATAL_ERROR_IN(args.executable())
        << "Incorrect dimensions of phi: " << phi.dimensions()
        << nl << exit(FatalError);
    }
  }
}


autoPtr<functionObjectList> readFunctionObjects
(
  const argList& args,
  const Time& runTime,
  dictionary& folDict
)
{
  autoPtr<functionObjectList> folPtr;
  if (args.optionFound("dict")) {
    folDict =
      IOdictionary
      {
        {
          args["dict"],
          runTime,
          IOobject::MUST_READ_IF_MODIFIED
        }
      };
    folPtr.reset(new functionObjectList{runTime, folDict});
  } else {
    folPtr.reset(new functionObjectList{runTime});
  }
  folPtr->start();
  return folPtr;
}


int main(int argc, char *argv[])
{
  mousse::timeSelector::addOptions();
  #include "add_region_option.inc"
  mousse::argList::addBoolOption
  (
    "noFlow",
    "suppress creating flow models"
  );
  #include "add_dict_option.inc"
  #include "set_root_case.inc"
  #include "create_time.inc"
  mousse::instantList timeDirs = mousse::timeSelector::select0(runTime, args);
  #include "create_named_mesh.inc"
  // Externally stored dictionary for functionObjectList
  // if not constructed from runTime
  dictionary folDict;
  // Construct functionObjectList
  autoPtr<functionObjectList> folPtr
  {
    readFunctionObjects(args, runTime, folDict)
  };
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    if (mesh.readUpdate() != polyMesh::UNCHANGED) {
      // Update functionObjectList if mesh changes
      folPtr = readFunctionObjects(args, runTime, folDict);
    }
    FatalIOError.throwExceptions();
    try {
      calc(args, runTime, mesh, folPtr());
    } catch (IOerror& err) {
      Warning << err << endl;
    }
    Info << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

