// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "inter_region_heat_transfer_model.hpp"
#include "basic_thermo.hpp"
#include "fvm_sup.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fvc_volume_integrate.hpp"
#include "fv_option_list.hpp"
// Static Data Members
namespace mousse
{
namespace fv
{
  defineTypeNameAndDebug(interRegionHeatTransferModel, 0);
}
}
//  Protected member functions
void mousse::fv::interRegionHeatTransferModel::setNbrModel()
{
  if (!firstIter_)
  {
    return;
  }
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName_);
  const optionList& fvOptions = nbrMesh.lookupObject<optionList>("fvOptions");
  bool nbrModelFound = false;
  forAll(fvOptions, i)
  {
    if (fvOptions[i].name() == nbrModelName_)
    {
      nbrModel_ = &const_cast<interRegionHeatTransferModel&>
      (
        refCast<const interRegionHeatTransferModel>(fvOptions[i])
      );
      nbrModelFound = true;
      break;
    }
  }
  if (!nbrModelFound)
  {
    FatalErrorIn("interRegionHeatTransferModel::setNbrModel()")
      << "Neighbour model not found" << nbrModelName_
      << " in region " << nbrMesh.name() << nl
      << exit(FatalError);
  }
  firstIter_ = false;
  // Set nbr model's nbr model to avoid construction order problems
  nbrModel_->setNbrModel();
}
void mousse::fv::interRegionHeatTransferModel::correct()
{
  if (master_)
  {
    if (mesh_.time().timeIndex() != timeIndex_)
    {
      calculateHtc();
      timeIndex_ = mesh_.time().timeIndex();
    }
  }
  else
  {
    nbrModel().correct();
    interpolate(nbrModel().htc(), htc_);
  }
}
// Constructors 
mousse::fv::interRegionHeatTransferModel::interRegionHeatTransferModel
(
  const word& name,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  interRegionOption
  (
    name,
    modelType,
    dict,
    mesh
  ),
  nbrModelName_(coeffs_.lookup("nbrModelName")),
  nbrModel_(NULL),
  firstIter_(true),
  timeIndex_(-1),
  htc_
  (
    IOobject
    (
      type() + ":htc",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar
    (
      "htc",
      dimEnergy/dimTime/dimTemperature/dimVolume,
      0.0
    ),
    zeroGradientFvPatchScalarField::typeName
  ),
  semiImplicit_(false),
  TName_(coeffs_.lookupOrDefault<word>("TName", "T")),
  TNbrName_(coeffs_.lookupOrDefault<word>("TNbrName", "T"))
{
  if (active())
  {
    coeffs_.lookup("fieldNames") >> fieldNames_;
    applied_.setSize(fieldNames_.size(), false);
    coeffs_.lookup("semiImplicit") >> semiImplicit_;
  }
}
// Destructor 
mousse::fv::interRegionHeatTransferModel::~interRegionHeatTransferModel()
{}
// Member Functions 
void mousse::fv::interRegionHeatTransferModel::addSup
(
  fvMatrix<scalar>& eqn,
  const label fieldI
)
{
  setNbrModel();
  correct();
  const volScalarField& he = eqn.psi();
  const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);
  tmp<volScalarField> tTmapped
  (
    new volScalarField
    (
      IOobject
      (
        type() + ":Tmapped",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      T
    )
  );
  volScalarField& Tmapped = tTmapped();
  const fvMesh& nbrMesh = mesh_.time().lookupObject<fvMesh>(nbrRegionName_);
  const volScalarField& Tnbr =
    nbrMesh.lookupObject<volScalarField>(TNbrName_);
  interpolate(Tnbr, Tmapped.internalField());
  if (debug)
  {
    Info<< "Volumetric integral of htc: "
      << fvc::domainIntegrate(htc_).value()
      << endl;
    if (mesh_.time().outputTime())
    {
      Tmapped.write();
      htc_.write();
    }
  }
  if (semiImplicit_)
  {
    if (he.dimensions() == dimEnergy/dimMass)
    {
      if (mesh_.foundObject<basicThermo>(basicThermo::dictName))
      {
        const basicThermo& thermo =
         mesh_.lookupObject<basicThermo>(basicThermo::dictName);
        volScalarField htcByCpv(htc_/thermo.Cpv());
        eqn += htc_*(Tmapped - T) + htcByCpv*he - fvm::Sp(htcByCpv, he);
        if (debug)
        {
          const dimensionedScalar energy =
            fvc::domainIntegrate(htc_*(he/thermo.Cp() - Tmapped));
          Info<< "Energy exchange from region " << nbrMesh.name()
            << " To " << mesh_.name() << " : " <<  energy.value()
            << endl;
        }
      }
      else
      {
        FatalErrorIn
        (
          "void mousse::fv::interRegionHeatTransferModel::addSup"
          "("
          "   fvMatrix<scalar>&, "
          "   const label "
          ")"
        )   << " on mesh " << mesh_.name()
          << " could not find object basicThermo."
          << " The available objects are: "
          << mesh_.names()
          << exit(FatalError);
      }
    }
    else if (he.dimensions() == dimTemperature)
    {
      eqn += htc_*Tmapped - fvm::Sp(htc_, he);
    }
  }
  else
  {
    eqn += htc_*(Tmapped - T);
  }
}
void mousse::fv::interRegionHeatTransferModel::addSup
(
  const volScalarField& rho,
  fvMatrix<scalar>& eqn,
  const label fieldI
)
{
  addSup(eqn, fieldI);
}
