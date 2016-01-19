// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "single_phase_transport_model.hpp"
#include "turbulent_transport_model.hpp"
#include "wall_fv_patch.hpp"
#include "make_graph.hpp"

int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"
  #include "interrogate_wall_patches.inc"
  Info<< "\nStarting time loop\n" << endl;
  while (runTime.loop()) {
    Info<< "Time = " << runTime.timeName() << nl << endl;
    fvVectorMatrix divR(turbulence->divDevReff(U));
    divR.source() = flowMask & divR.source();
    fvVectorMatrix UEqn
    (
      divR == gradP
    );
    UEqn.relax();
    UEqn.solve();
    // Correct driving force for a constant volume flow rate
    dimensionedVector UbarStar = flowMask & U.weightedAverage(mesh.V());
    U += (Ubar - UbarStar);
    gradP += (Ubar - UbarStar)/(1.0/UEqn.A())().weightedAverage(mesh.V());
    laminarTransport.correct();
    turbulence->correct();
    Info<< "Uncorrected Ubar = " << (flowDirection & UbarStar.value())
      << ", pressure gradient = " << (flowDirection & gradP.value())
      << endl;
    #include "evaluate_near_wall.inc"
    if (runTime.outputTime())
    {
      #include "make_graphs.inc"
    }
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
