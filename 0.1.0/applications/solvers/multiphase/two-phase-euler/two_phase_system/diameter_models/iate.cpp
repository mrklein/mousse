// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iate.hpp"
#include "iate_source.hpp"
#include "two_phase_system.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
#include "fvc_average.hpp"
#include "fv_option_list.hpp"
#include "mathematical_constants.hpp"
#include "fundamental_constants.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace diameterModels
{
DEFINE_TYPE_NAME_AND_DEBUG(IATE, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  diameterModel,
  IATE,
  dictionary
);
}
}
// Constructors 
mousse::diameterModels::IATE::IATE
(
  const dictionary& diameterProperties,
  const phaseModel& phase
)
:
  diameterModel{diameterProperties, phase},
  kappai_
  {
    {
      IOobject::groupName("kappai", phase.name()),
      phase_.U().time().timeName(),
      phase_.U().mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    phase_.U().mesh()
  },
  dMax_{"dMax", dimLength, diameterProperties_},
  dMin_{"dMin", dimLength, diameterProperties_},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    diameterProperties_
  },
  d_
  {
    {
      IOobject::groupName("d", phase.name()),
      phase_.U().time().timeName(),
      phase_.U().mesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    dsm()
  },
  sources_
  {
    diameterProperties_.lookup("sources"),
    IATEsource::iNew(*this)
  }
{}
// Destructor 
mousse::diameterModels::IATE::~IATE()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::diameterModels::IATE::dsm() const
{
  return max(6/max(kappai_, 6/dMax_), dMin_);
}
void mousse::diameterModels::IATE::correct()
{
  // Initialise the accumulated source term to the dilatation effect
  volScalarField R
  {
    ((1.0/3.0)/max(fvc::average(phase_ + phase_.oldTime()),
                   residualAlpha_))
      *(fvc::ddt(phase_) + fvc::div(phase_.alphaPhi()))
  };
  // Accumulate the run-time selectable sources
  FOR_ALL(sources_, j)
  {
    R -= sources_[j].R();
  }
  // const_cast needed because the operators and functions of fvOptions
  // are currently non-const.
  fv::optionList& fvOptions = const_cast<fv::optionList&>
  (
    phase_.U().mesh().lookupObject<fv::optionList>("fvOptions")
  );
  // Construct the interfacial curvature equation
  fvScalarMatrix kappaiEqn
  {
    fvm::ddt(kappai_) + fvm::div(phase_.phi(), kappai_)
    - fvm::Sp(fvc::div(phase_.phi()), kappai_)
    ==
    - fvm::SuSp(R, kappai_)
    + fvOptions(kappai_)
  };
  kappaiEqn.relax();
  fvOptions.constrain(kappaiEqn);
  kappaiEqn.solve();
  // Update the Sauter-mean diameter
  d_ = dsm();
}
bool mousse::diameterModels::IATE::read(const dictionary& phaseProperties)
{
  diameterModel::read(phaseProperties);
  diameterProperties_.lookup("dMax") >> dMax_;
  diameterProperties_.lookup("dMin") >> dMin_;
  // Re-create all the sources updating number, type and coefficients
  PtrList<IATEsource>
  {
    diameterProperties_.lookup("sources"),
    IATEsource::iNew(*this)
  }.transfer(sources_);
  return true;
}
