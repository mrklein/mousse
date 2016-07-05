// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time_selector.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "transform_geometric_field.hpp"
#include "ioobject_list.hpp"


using namespace mousse;


template<class GeometricField>
void RotateFields
(
  const fvMesh& mesh,
  const IOobjectList& objects,
  const tensor& T
)
{
  // Search list of objects for volScalarFields
  IOobjectList fields{objects.lookupClass(GeometricField::typeName)};
  FOR_ALL_ITER(IOobjectList, fields, fieldIter) {
    Info << "    Rotating " << fieldIter()->name() << endl;
    GeometricField theta{*fieldIter(), mesh};
    transform(theta, dimensionedTensor(T), theta);
    theta.write();
  }
}


int main(int argc, char *argv[])
{
  timeSelector::addOptions();
  argList::validArgs.append("n1");
  argList::validArgs.append("n2");
  #include "set_root_case.inc"
  #include "create_time.inc"
  vector n1{args.argRead<vector>(1)};
  n1 /= mag(n1);
  vector n2{args.argRead<vector>(2)};
  n2 /= mag(n2);
  tensor T{rotationTensor(n1, n2)};

  {
    pointIOField points
    {
      {
        "points",
        runTime.findInstance(polyMesh::meshSubDir, "points"),
        polyMesh::meshSubDir,
        runTime,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      }
    };
    points = transform(T, points);
    // Set the precision of the points data to 10
    IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
    Info << "Writing points into directory " << points.path() << nl << endl;
    points.write();
  }

  instantList timeDirs = timeSelector::select0(runTime, args);
  #include "create_mesh.inc"
  FOR_ALL(timeDirs, timeI) {
    runTime.setTime(timeDirs[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    // Search for list of objects for this time
    IOobjectList objects{mesh, runTime.timeName()};
    RotateFields<volVectorField>(mesh, objects, T);
    RotateFields<volSphericalTensorField>(mesh, objects, T);
    RotateFields<volSymmTensorField>(mesh, objects, T);
    RotateFields<volTensorField>(mesh, objects, T);
    RotateFields<surfaceVectorField>(mesh, objects, T);
    RotateFields<surfaceSphericalTensorField>(mesh, objects, T);
    RotateFields<surfaceSymmTensorField>(mesh, objects, T);
    RotateFields<surfaceTensorField>(mesh, objects, T);
  }
  Info << "\nEnd\n" << endl;
  return 0;
}

