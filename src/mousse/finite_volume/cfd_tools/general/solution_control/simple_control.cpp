// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "simple_control.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(simpleControl, 0);

}


// Protected Member Functions 
void mousse::simpleControl::read()
{
  solutionControl::read(true);
}


bool mousse::simpleControl::criteriaSatisfied()
{
  if (residualControl_.empty()) {
    return false;
  }
  bool achieved = true;
  bool checked = false;    // safety that some checks were indeed performed
  const dictionary& solverDict = mesh_.solverPerformanceDict();
  FOR_ALL_CONST_ITER(dictionary, solverDict, iter) {
    const word& variableName = iter().keyword();
    const label fieldI = applyToField(variableName);
    if (fieldI != -1) {
      const List<solverPerformance> sp{iter().stream()};
      const scalar residual = sp.first().initialResidual();
      checked = true;
      bool absCheck = residual < residualControl_[fieldI].absTol;
      achieved = achieved && absCheck;
      if (debug) {
        Info << algorithmName_ << " solution statistics:" << endl;
        Info << "    " << variableName << ": tolerance = " << residual
          << " (" << residualControl_[fieldI].absTol << ")"
          << endl;
      }
    }
  }
  return checked && achieved;
}


// Constructors 
mousse::simpleControl::simpleControl(fvMesh& mesh)
:
  solutionControl{mesh, "SIMPLE"},
  initialised_{false}
{
  read();
  Info << nl;
  if (residualControl_.empty()) {
    Info << algorithmName_ << ": no convergence criteria found. "
      << "Calculations will run for " << mesh_.time().endTime().value()
      << " steps." << nl << endl;
  } else {
    Info << algorithmName_ << ": convergence criteria" << nl;
    FOR_ALL(residualControl_, i) {
      Info << "    field " << residualControl_[i].name << token::TAB
        << " tolerance " << residualControl_[i].absTol
        << nl;
    }
    Info << endl;
  }
}


// Destructor 
mousse::simpleControl::~simpleControl()
{}


// Member Functions 
bool mousse::simpleControl::loop()
{
  read();
  Time& time = const_cast<Time&>(mesh_.time());
  if (initialised_) {
    if (criteriaSatisfied()) {
      Info << nl << algorithmName_ << " solution converged in "
        << time.timeName() << " iterations" << nl << endl;
      // Set to finalise calculation
      time.writeAndEnd();
    } else {
      storePrevIterFields();
    }
  } else {
    initialised_ = true;
    storePrevIterFields();
  }
  return time.loop();
}

