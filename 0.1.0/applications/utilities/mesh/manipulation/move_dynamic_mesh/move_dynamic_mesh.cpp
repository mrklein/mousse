// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "dynamic_fv_mesh.hpp"
#include "pimple_control.hpp"
#include "vtk_surface_writer.hpp"
#include "cyclic_ami_poly_patch.hpp"

using namespace mousse;

// Dump patch + weights to vtk file
void writeWeights
(
  const scalarField& wghtSum,
  const primitivePatch& patch,
  const fileName& directory,
  const fileName& prefix,
  const word& timeName
)
{
  vtkSurfaceWriter writer;
  writer.write
  (
    directory,
    prefix + "_proc" + mousse::name(Pstream::myProcNo()) + "_" + timeName,
    patch.localPoints(),
    patch.localFaces(),
    "weightsSum",
    wghtSum,
    false
  );
}

void writeWeights(const polyMesh& mesh)
{
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  const word tmName(mesh.time().timeName());
  FOR_ALL(pbm, patchI)
  {
    if (isA<cyclicAMIPolyPatch>(pbm[patchI]))
    {
      const cyclicAMIPolyPatch& cpp =
        refCast<const cyclicAMIPolyPatch>(pbm[patchI]);
      if (cpp.owner())
      {
        Info << "Calculating AMI weights between owner patch: "
          << cpp.name() << " and neighbour patch: "
          << cpp.neighbPatch().name() << endl;
        const AMIPatchToPatchInterpolation& ami =
          cpp.AMI();
        writeWeights
        (
          ami.tgtWeightsSum(),
          cpp.neighbPatch(),
          "postProcessing",
          "tgt",
          tmName
        );
        writeWeights
        (
          ami.srcWeightsSum(),
          cpp,
          "postProcessing",
          "src",
          tmName
        );
      }
    }
  }
}

int main(int argc, char *argv[])
{
  #include "add_region_option.inc"
  argList::addBoolOption
  (
    "checkAMI",
    "check AMI weights"
  );
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_named_dynamic_fv_mesh.inc"
  const bool checkAMI  = args.optionFound("checkAMI");
  if (checkAMI)
  {
    Info << "Writing VTK files with weights of AMI patches." << nl << endl;
  }
  pimpleControl pimple{mesh};
  bool moveMeshOuterCorrectors
  {
    pimple.dict().lookupOrDefault<Switch>("moveMeshOuterCorrectors", false)
  };
  while (runTime.loop())
  {
    Info << "Time = " << runTime.timeName() << endl;
    while (pimple.loop())
    {
      if (pimple.firstIter() || moveMeshOuterCorrectors)
      {
        mesh.update();
      }
    }
    mesh.checkMesh(true);
    if (checkAMI)
    {
      writeWeights(mesh);
    }
    runTime.write();
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "End\n" << endl;
  return 0;
}
