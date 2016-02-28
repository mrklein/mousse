// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_model.hpp"
#include "phase_system.hpp"
#include "diameter_model.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(phaseModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(phaseModel, phaseSystem);
}
// Constructors 
mousse::phaseModel::phaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  volScalarField
  {
    {
      IOobject::groupName("alpha", phaseName),
      fluid.mesh().time().timeName(),
      fluid.mesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    fluid.mesh(),
    {"alpha", dimless, 0}
  },
  fluid_{fluid},
  name_{phaseName},
  index_{index},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    fluid.subDict(phaseName).lookup("residualAlpha")
  },
  alphaMax_{fluid.subDict(phaseName).lookupOrDefault("alphaMax", 1.0)}
{
  diameterModel_ = diameterModel::New(fluid.subDict(phaseName), *this);
}
mousse::autoPtr<mousse::phaseModel> mousse::phaseModel::clone() const
{
  NOT_IMPLEMENTED("phaseModel::clone() const");
  return autoPtr<phaseModel>{NULL};
}
// Destructor 
mousse::phaseModel::~phaseModel()
{}
// Member Functions 
const mousse::word& mousse::phaseModel::name() const
{
  return name_;
}
const mousse::word& mousse::phaseModel::keyword() const
{
  return name_;
}
mousse::label mousse::phaseModel::index() const
{
  return index_;
}
const mousse::phaseSystem& mousse::phaseModel::fluid() const
{
  return fluid_;
}
const mousse::dimensionedScalar& mousse::phaseModel::residualAlpha() const
{
  return residualAlpha_;
}
mousse::scalar mousse::phaseModel::alphaMax() const
{
  return alphaMax_;
}
mousse::tmp<mousse::volScalarField> mousse::phaseModel::d() const
{
  return diameterModel_().d();
}
void mousse::phaseModel::correct()
{
  diameterModel_->correct();
}
void mousse::phaseModel::correctKinematics()
{}
void mousse::phaseModel::correctThermo()
{}
void mousse::phaseModel::correctTurbulence()
{}
void mousse::phaseModel::correctEnergyTransport()
{}
bool mousse::phaseModel::read()
{
  return diameterModel_->read(fluid_.subDict(name_));
}
bool mousse::phaseModel::compressible() const
{
  return false;
}
const mousse::tmp<mousse::volScalarField>& mousse::phaseModel::divU() const
{
  NOT_IMPLEMENTED("mousse::phaseModel::divU()");
  static tmp<mousse::volScalarField> divU_{NULL};
  return divU_;
}
void mousse::phaseModel::divU(const tmp<volScalarField>& /*divU*/)
{
  WARNING_IN("phaseModel::divU(const tmp<volScalarField>& divU)")
    << "Attempt to set the dilatation rate of an incompressible phase"
    << endl;
}
const mousse::volScalarField& mousse::phaseModel::K() const
{
  NOT_IMPLEMENTED("mousse::phaseModel::K()");
  return volScalarField::null();
}
const mousse::surfaceScalarField& mousse::phaseModel::DbyA() const
{
  return surfaceScalarField::null();
}
void mousse::phaseModel::DbyA(const tmp<surfaceScalarField>& /*DbyA*/)
{
  WARNING_IN("phaseModel::DbyA(const surfaceScalarField& DbyA)")
    << "Attempt to set the dilatation rate of an incompressible phase"
    << endl;
}
