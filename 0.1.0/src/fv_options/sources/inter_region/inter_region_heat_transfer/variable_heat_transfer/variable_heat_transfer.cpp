// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "variable_heat_transfer.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  defineTypeNameAndDebug(variableHeatTransfer, 0);
  addToRunTimeSelectionTable
  (
    option,
    variableHeatTransfer,
    dictionary
  );
}
}
// Constructors 
mousse::fv::variableHeatTransfer::variableHeatTransfer
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  interRegionHeatTransferModel(name, modelType, dict, mesh),
  UNbrName_(coeffs_.lookupOrDefault<word>("UNbrName", "U")),
  a_(0),
  b_(0),
  c_(0),
  ds_(0),
  Pr_(0),
  AoV_()
{
  if (master_)
  {
    a_ = readScalar(coeffs_.lookup("a"));
    b_ = readScalar(coeffs_.lookup("b"));
    c_ = readScalar(coeffs_.lookup("c"));
    ds_ = readScalar(coeffs_.lookup("ds"));
    Pr_ = readScalar(coeffs_.lookup("Pr"));
    AoV_.reset
    (
      new volScalarField
      (
        IOobject
        (
          "AoV",
          mesh_.time().timeName(),
          mesh_,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        ),
        mesh_
      )
    );
  }
}
// Destructor 
mousse::fv::variableHeatTransfer::~variableHeatTransfer()
{}
// Member Functions 
void mousse::fv::variableHeatTransfer::calculateHtc()
{
  const fvMesh& nbrMesh =
    mesh_.time().lookupObject<fvMesh>(nbrRegionName());
  const compressible::turbulenceModel& nbrTurb =
    nbrMesh.lookupObject<compressible::turbulenceModel>
    (
      turbulenceModel::propertiesName
    );
  const fluidThermo& nbrThermo =
    nbrMesh.lookupObject<fluidThermo>(basicThermo::dictName);
  const volVectorField& UNbr =
    nbrMesh.lookupObject<volVectorField>(UNbrName_);
  const volScalarField ReNbr(mag(UNbr)*ds_*nbrThermo.rho()/nbrTurb.mut());
  const volScalarField NuNbr(a_*pow(ReNbr, b_)*pow(Pr_, c_));
  const scalarField htcNbr(NuNbr*nbrTurb.kappaEff()/ds_);
  const scalarField htcNbrMapped(interpolate(htcNbr));
  htc_.internalField() = htcNbrMapped*AoV_;
}
bool mousse::fv::variableHeatTransfer::read(const dictionary& dict)
{
  if (interRegionHeatTransferModel::read(dict))
  {
    coeffs_.readIfPresent("UNbrName", UNbrName_);
    coeffs_.readIfPresent("a", a_);
    coeffs_.readIfPresent("b", b_);
    coeffs_.readIfPresent("c", c_);
    coeffs_.readIfPresent("ds", ds_);
    coeffs_.readIfPresent("Pr", Pr_);
    return true;
  }
  else
  {
    return false;
  }
}
