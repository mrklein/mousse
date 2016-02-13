// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "write_cell_graph.hpp"
#include "os_specific.hpp"

int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_mesh.inc"
  #include "create_fields.inc"

  Info<< nl << "Calculating value(price of comodities)" << endl;
  surfaceScalarField phi{"phi", (sigmaSqr - r)*(Pf & mesh.Sf())};
  volScalarField DV{"DV", 0.5*sigmaSqr*sqr(P.component(mousse::vector::X))};
  Info<< "Starting time loop\n" << endl;
  while (runTime.loop())
  {
    delta == fvc::grad(V)().component(mousse::vector::X);
    solve
    (
      fvm::ddt(V)
      + fvm::div(phi, V)
      - fvm::Sp(fvc::div(phi), V)
      - fvm::laplacian(DV, V)
      ==
      - fvm::Sp(r, V)
    );
    runTime.write();
    if (runTime.outputTime())
    {
      writeCellGraph(V, runTime.graphFormat());
      writeCellGraph(delta, runTime.graphFormat());
    }
    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info<< "End\n" << endl;
  return 0;
}
