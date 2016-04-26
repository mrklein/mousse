// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "radiation_model.hpp"
#include "absorption_emission_model.hpp"
#include "scatter_model.hpp"
#include "soot_model.hpp"
#include "fvm_sup.hpp"
#include "fluid_thermo.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(radiationModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(radiationModel, T);
DEFINE_RUN_TIME_SELECTION_TABLE(radiationModel, dictionary);

}
}


// Private Member Functions 
mousse::IOobject mousse::radiation::radiationModel::createIOobject
(
  const fvMesh& mesh
) const
{
  IOobject io
  {
    "radiationProperties",
    mesh.time().constant(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  };
  if (io.headerOk()) {
    io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
    return io;
  } else {
    io.readOpt() = IOobject::NO_READ;
    return io;
  }
}


void mousse::radiation::radiationModel::initialise()
{
  if (!radiation_)
    return;
  solverFreq_ = max(1, lookupOrDefault<label>("solverFreq", 1));
  absorptionEmission_.reset
  (
    absorptionEmissionModel::New(*this, mesh_).ptr()
  );
  scatter_.reset(scatterModel::New(*this, mesh_).ptr());
  soot_.reset(sootModel::New(*this, mesh_).ptr());
}


// Constructors 
mousse::radiation::radiationModel::radiationModel(const volScalarField& T)
:
  IOdictionary
  {
    {
      "radiationProperties",
      T.time().constant(),
      T.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    }
  },
  mesh_{T.mesh()},
  time_{T.time()},
  T_{T},
  radiation_{false},
  coeffs_{dictionary::null},
  solverFreq_{0},
  firstIter_{true},
  absorptionEmission_{NULL},
  scatter_{NULL},
  soot_{NULL}
{}


mousse::radiation::radiationModel::radiationModel
(
  const word& type,
  const volScalarField& T
)
:
  IOdictionary{createIOobject(T.mesh())},
  mesh_{T.mesh()},
  time_{T.time()},
  T_{T},
  radiation_{lookupOrDefault("radiation", true)},
  coeffs_{subOrEmptyDict(type + "Coeffs")},
  solverFreq_{1},
  firstIter_{true},
  absorptionEmission_{NULL},
  scatter_{NULL},
  soot_{NULL}
{
  if (readOpt() == IOobject::NO_READ) {
    radiation_ = false;
  }
  initialise();
}


mousse::radiation::radiationModel::radiationModel
(
  const word& type,
  const dictionary& dict,
  const volScalarField& T
)
:
  IOdictionary
  {
    {
      "radiationProperties",
      T.time().constant(),
      T.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    dict
  },
  mesh_{T.mesh()},
  time_{T.time()},
  T_{T},
  radiation_{lookupOrDefault("radiation", true)},
  coeffs_{subOrEmptyDict(type + "Coeffs")},
  solverFreq_{1},
  firstIter_{true},
  absorptionEmission_{NULL},
  scatter_{NULL},
  soot_{NULL}
{
  initialise();
}


// Destructor   
mousse::radiation::radiationModel::~radiationModel()
{}


// Member Functions 
bool mousse::radiation::radiationModel::read()
{
  if (regIOobject::read()) {
    lookup("radiation") >> radiation_;
    coeffs_ = subOrEmptyDict(type() + "Coeffs");
    solverFreq_ = lookupOrDefault<label>("solverFreq", 1);
    solverFreq_ = max(1, solverFreq_);
    return true;
  }
  return false;
}


void mousse::radiation::radiationModel::correct()
{
  if (!radiation_) {
    return;
  }
  if (firstIter_ || (time_.timeIndex() % solverFreq_ == 0)) {
    calculate();
    firstIter_ = false;
  }
  if (!soot_.empty()) {
    soot_->correct();
  }
}


mousse::tmp<mousse::fvScalarMatrix> mousse::radiation::radiationModel::Sh
(
  fluidThermo& thermo
) const
{
  volScalarField& he = thermo.he();
  const volScalarField Cpv{thermo.Cpv()};
  const volScalarField T3{pow3(T_)};

  return Ru() - fvm::Sp(4.0*Rp()*T3/Cpv, he) - Rp()*T3*(T_ - 4.0*he/Cpv);
}


mousse::tmp<mousse::fvScalarMatrix> mousse::radiation::radiationModel::ST
(
  const dimensionedScalar& rhoCp,
  volScalarField& T
) const
{
  return Ru()/rhoCp - fvm::Sp(Rp()*pow3(T)/rhoCp, T);
}


const mousse::radiation::absorptionEmissionModel&
mousse::radiation::radiationModel::absorptionEmission() const
{
  if (!absorptionEmission_.valid()) {
    FATAL_ERROR_IN
    (
      "const mousse::radiation::absorptionEmissionModel&"
      "mousse::radiation::radiationModel::absorptionEmission() const"
    )
    << "Requested radiation absorptionEmission model, but model is "
    << "not activate" << abort(FatalError);
  }
  return absorptionEmission_();
}


const mousse::radiation::sootModel&
mousse::radiation::radiationModel::soot() const
{
  if (!soot_.valid()) {
    FATAL_ERROR_IN
    (
      "const mousse::radiation::sootModel&"
      "mousse::radiation::radiationModel::soot() const"
    )
    << "Requested radiation sootModel model, but model is "
    << "not activate" << abort(FatalError);
  }

  return soot_();
}

