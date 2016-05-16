// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_cfd.hpp"
#include "psi_reaction_thermo.hpp"
#include "psi_chemistry_model.hpp"
#include "chemistry_solver.hpp"
#include "ofstream.hpp"
#include "thermo_physics_types.hpp"
#include "basic_multi_component_mixture.hpp"
#include "cell_modeller.hpp"


int main(int argc, char *argv[])
{
  argList::noParallel();
  #include "set_root_case.inc"
  #include "create_time.inc"
  #include "create_single_cell_mesh.inc"
  #include "create_fields.inc"
  #include "read_initial_conditions.inc"
  #include "create_controls.inc"
  Info << "\nStarting time loop\n" << endl;
  while (runTime.run()) {
    #include "read_controls.inc"
    #include "set_delta_t.inc"
    runTime++;
    Info << "Time = " << runTime.timeName() << nl << endl;
    #include "solve_chemistry.inc"
    #include "y_eqn.inc"
    #include "h_eqn.inc"
    #include "p_eqn.inc"
    #include "output.inc"
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
      << "  ClockTime = " << runTime.elapsedClockTime() << " s"
      << nl << endl;
  }
  Info << "Number of steps = " << runTime.timeIndex() << endl;
  Info << "End" << nl << endl;
  return 0;
}

