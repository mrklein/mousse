// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "fv_mesh.hpp"
#include "point_fields.hpp"
#include "istring_stream.hpp"
#include "vol_point_interpolation.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::validArgs.append("scaling factor");
  #include "set_root_case.inc"
  const scalar scaleFactor = args.argRead<scalar>(1);
  #include "create_time.inc"
  #include "create_mesh.inc"
  volPointInterpolation pInterp{mesh};
  // Get times list
  instantList Times = runTime.times();
  pointField zeroPoints{mesh.points()};
  // skip "constant" time
  for (label timeI = 1; timeI < Times.size(); ++timeI) {
    runTime.setTime(Times[timeI], timeI);
    Info << "Time = " << runTime.timeName() << endl;
    IOobject Uheader
    {
      "U",
      runTime.timeName(),
      mesh,
      IOobject::MUST_READ
    };
    // Check U exists
    if (Uheader.headerOk()) {
      Info << "    Reading U" << endl;
      volVectorField U{Uheader, mesh};
      pointField newPoints
      {
        zeroPoints + scaleFactor*pInterp.interpolate(U)().internalField()
      };
      mesh.polyMesh::movePoints(newPoints);
      mesh.write();
    } else {
      Info << "    No U" << endl;
    }
    Info << endl;
  }
  Info << "End\n" << endl;
  return 0;
}

