// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "_read_fields.hpp"
#include "point_fields.hpp"
#include "transform_field.hpp"
#include "transform_geometric_field.hpp"
#include "istring_stream.hpp"
#include "mathematical_constants.hpp"

using namespace mousse;
using namespace mousse::constant::mathematical;
template<class GeoField>
void readAndRotateFields
(
  PtrList<GeoField>& flds,
  const fvMesh& mesh,
  const tensor& T,
  const IOobjectList& objects
)
{
  ReadFields(mesh, objects, flds);
  forAll(flds, i)
  {
    Info<< "Transforming " << flds[i].name() << endl;
    dimensionedTensor dimT("t", flds[i].dimensions(), T);
    transform(flds[i], dimT, flds[i]);
  }
}
void rotateFields(const argList& args, const Time& runTime, const tensor& T)
{
  #include "create_named_mesh.hpp"
  // Read objects in time directory
  IOobjectList objects(mesh, runTime.timeName());
  // Read vol fields.
  PtrList<volScalarField> vsFlds;
  readAndRotateFields(vsFlds, mesh, T, objects);
  PtrList<volVectorField> vvFlds;
  readAndRotateFields(vvFlds, mesh, T, objects);
  PtrList<volSphericalTensorField> vstFlds;
  readAndRotateFields(vstFlds, mesh, T, objects);
  PtrList<volSymmTensorField> vsymtFlds;
  readAndRotateFields(vsymtFlds, mesh, T, objects);
  PtrList<volTensorField> vtFlds;
  readAndRotateFields(vtFlds, mesh, T, objects);
  // Read surface fields.
  PtrList<surfaceScalarField> ssFlds;
  readAndRotateFields(ssFlds, mesh, T, objects);
  PtrList<surfaceVectorField> svFlds;
  readAndRotateFields(svFlds, mesh, T, objects);
  PtrList<surfaceSphericalTensorField> sstFlds;
  readAndRotateFields(sstFlds, mesh, T, objects);
  PtrList<surfaceSymmTensorField> ssymtFlds;
  readAndRotateFields(ssymtFlds, mesh, T, objects);
  PtrList<surfaceTensorField> stFlds;
  readAndRotateFields(stFlds, mesh, T, objects);
  mesh.write();
}
int main(int argc, char *argv[])
{
  argList::addOption
  (
    "translate",
    "vector",
    "translate by the specified <vector> - eg, '(1 0 0)'"
  );
  argList::addOption
  (
    "rotate",
    "(vectorA vectorB)",
    "transform in terms of a rotation between <vectorA> and <vectorB> "
    "- eg, '( (1 0 0) (0 0 1) )'"
  );
  argList::addOption
  (
    "rollPitchYaw",
    "vector",
    "transform in terms of '(roll pitch yaw)' in degrees"
  );
  argList::addOption
  (
    "yawPitchRoll",
    "vector",
    "transform in terms of '(yaw pitch roll)' in degrees"
  );
  argList::addBoolOption
  (
    "rotateFields",
    "read and transform vector and tensor fields too"
  );
  argList::addOption
  (
    "scale",
    "vector",
    "scale by the specified amount - eg, '(0.001 0.001 0.001)' for a "
    "uniform [mm] to [m] scaling"
  );
  #include "add_region_option.hpp"
  #include "set_root_case.hpp"
  #include "create_time.hpp"
  word regionName = polyMesh::defaultRegion;
  fileName meshDir;
  if (args.optionReadIfPresent("region", regionName))
  {
    meshDir = regionName/polyMesh::meshSubDir;
  }
  else
  {
    meshDir = polyMesh::meshSubDir;
  }
  pointIOField points
  (
    IOobject
    (
      "points",
      runTime.findInstance(meshDir, "points"),
      meshDir,
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    )
  );
  const bool doRotateFields = args.optionFound("rotateFields");
  // this is not actually stringent enough:
  if (args.options().empty())
  {
    FatalErrorIn(args.executable())
      << "No options supplied, please use one or more of "
       "-translate, -rotate or -scale options."
      << exit(FatalError);
  }
  vector v;
  if (args.optionReadIfPresent("translate", v))
  {
    Info<< "Translating points by " << v << endl;
    points += v;
  }
  if (args.optionFound("rotate"))
  {
    Pair<vector> n1n2
    (
      args.optionLookup("rotate")()
    );
    n1n2[0] /= mag(n1n2[0]);
    n1n2[1] /= mag(n1n2[1]);
    tensor T = rotationTensor(n1n2[0], n1n2[1]);
    Info<< "Rotating points by " << T << endl;
    points = transform(T, points);
    if (doRotateFields)
    {
      rotateFields(args, runTime, T);
    }
  }
  else if (args.optionReadIfPresent("rollPitchYaw", v))
  {
    Info<< "Rotating points by" << nl
      << "    roll  " << v.x() << nl
      << "    pitch " << v.y() << nl
      << "    yaw   " << v.z() << nl;
    // Convert to radians
    v *= pi/180.0;
    quaternion R(v.x(), v.y(), v.z());
    Info<< "Rotating points by quaternion " << R << endl;
    points = transform(R, points);
    if (doRotateFields)
    {
      rotateFields(args, runTime, R.R());
    }
  }
  else if (args.optionReadIfPresent("yawPitchRoll", v))
  {
    Info<< "Rotating points by" << nl
      << "    yaw   " << v.x() << nl
      << "    pitch " << v.y() << nl
      << "    roll  " << v.z() << nl;
    // Convert to radians
    v *= pi/180.0;
    scalar yaw = v.x();
    scalar pitch = v.y();
    scalar roll = v.z();
    quaternion R = quaternion(vector(0, 0, 1), yaw);
    R *= quaternion(vector(0, 1, 0), pitch);
    R *= quaternion(vector(1, 0, 0), roll);
    Info<< "Rotating points by quaternion " << R << endl;
    points = transform(R, points);
    if (doRotateFields)
    {
      rotateFields(args, runTime, R.R());
    }
  }
  if (args.optionReadIfPresent("scale", v))
  {
    Info<< "Scaling points by " << v << endl;
    points.replace(vector::X, v.x()*points.component(vector::X));
    points.replace(vector::Y, v.y()*points.component(vector::Y));
    points.replace(vector::Z, v.z()*points.component(vector::Z));
  }
  // Set the precision of the points data to 10
  IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));
  Info<< "Writing points into directory " << points.path() << nl << endl;
  points.write();
  return 0;
}
