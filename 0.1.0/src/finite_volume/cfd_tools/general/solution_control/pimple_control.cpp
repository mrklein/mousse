// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pimple_control.hpp"
#include "switch.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pimpleControl, 0);

}


// Protected Member Functions 
void mousse::pimpleControl::read()
{
  solutionControl::read(false);
  // Read solution controls
  const dictionary& pimpleDict = dict();
  nCorrPIMPLE_ = pimpleDict.lookupOrDefault<label>("nOuterCorrectors", 1);
  nCorrPISO_ = pimpleDict.lookupOrDefault<label>("nCorrectors", 1);
  turbOnFinalIterOnly_ =
    pimpleDict.lookupOrDefault<Switch>("turbOnFinalIterOnly", true);
}


bool mousse::pimpleControl::criteriaSatisfied()
{
  // no checks on first iteration - nothing has been calculated yet
  if ((corr_ == 1) || residualControl_.empty() || finalIter()) {
    return false;
  }
  bool storeIni = this->storeInitialResiduals();
  bool achieved = true;
  bool checked = false;    // safety that some checks were indeed performed
  const dictionary& solverDict = mesh_.solverPerformanceDict();
  FOR_ALL_CONST_ITER(dictionary, solverDict, iter) {
    const word& variableName = iter().keyword();
    const label fieldI = applyToField(variableName);
    if (fieldI != -1) {
      const List<solverPerformance> sp{iter().stream()};
      const scalar residual = sp.last().initialResidual();
      checked = true;
      if (storeIni) {
        residualControl_[fieldI].initialResidual =
          sp.first().initialResidual();
      }
      const bool absCheck = residual < residualControl_[fieldI].absTol;
      bool relCheck = false;
      scalar relative = 0.0;
      if (!storeIni) {
        const scalar iniRes =
          residualControl_[fieldI].initialResidual + ROOTVSMALL;
        relative = residual/iniRes;
        relCheck = relative < residualControl_[fieldI].relTol;
      }
      achieved = achieved && (absCheck || relCheck);
      if (debug) {
        Info << algorithmName_ << " loop:" << endl;
        Info << "    " << variableName
          << " PIMPLE iter " << corr_
          << ": ini res = "
          << residualControl_[fieldI].initialResidual
          << ", abs tol = " << residual
          << " (" << residualControl_[fieldI].absTol << ")"
          << ", rel tol = " << relative
          << " (" << residualControl_[fieldI].relTol << ")"
          << endl;
      }
    }
  }
  return checked && achieved;
}


// Constructors 
mousse::pimpleControl::pimpleControl(fvMesh& mesh, const word& dictName)
:
  solutionControl{mesh, dictName},
  nCorrPIMPLE_{0},
  nCorrPISO_{0},
  corrPISO_{0},
  turbOnFinalIterOnly_{true},
  converged_{false}
{
  read();
  if (nCorrPIMPLE_ > 1) {
    Info << nl;
    if (residualControl_.empty()) {
      Info << algorithmName_ << ": no residual control data found. "
        << "Calculations will employ " << nCorrPIMPLE_
        << " corrector loops" << nl << endl;
    } else {
      Info << algorithmName_ << ": max iterations = " << nCorrPIMPLE_ << endl;
      FOR_ALL(residualControl_, i) {
        Info << "    field " << residualControl_[i].name << token::TAB
          << ": relTol " << residualControl_[i].relTol
          << ", tolerance " << residualControl_[i].absTol
          << nl;
      }
      Info << endl;
    }
  } else {
    Info << nl << algorithmName_ << ": Operating solver in PISO mode" << nl
      << endl;
  }
}


// Destructor 
mousse::pimpleControl::~pimpleControl()
{}


// Member Functions 
bool mousse::pimpleControl::loop()
{
  read();
  corr_++;
  if (debug) {
    Info << algorithmName_ << " loop: corr = " << corr_ << endl;
  }
  if (corr_ == nCorrPIMPLE_ + 1) {
    if ((!residualControl_.empty()) && (nCorrPIMPLE_ != 1)) {
      Info << algorithmName_ << ": not converged within "
        << nCorrPIMPLE_ << " iterations" << endl;
    }
    corr_ = 0;
    mesh_.data::remove("finalIteration");
    return false;
  }
  bool completed = false;
  if (converged_ || criteriaSatisfied()) {
    if (converged_) {
      Info << algorithmName_ << ": converged in " << corr_ - 1
        << " iterations" << endl;
      mesh_.data::remove("finalIteration");
      corr_ = 0;
      converged_ = false;
      completed = true;
    } else {
      Info << algorithmName_ << ": iteration " << corr_ << endl;
      storePrevIterFields();
      mesh_.data::add("finalIteration", true);
      converged_ = true;
    }
  } else {
    if (finalIter()) {
      mesh_.data::add("finalIteration", true);
    }
    if (corr_ <= nCorrPIMPLE_) {
      Info << algorithmName_ << ": iteration " << corr_ << endl;
      storePrevIterFields();
      completed = false;
    }
  }
  return !completed;
}

