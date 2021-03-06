// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_one_dim.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "surface_interpolate.hpp"
#include "fvm.hpp"
#include "fvc_div.hpp"
#include "fvc_volume_integrate.hpp"
#include "fv_matrices.hpp"
#include "absorption_emission_model.hpp"
#include "fvc_laplacian.hpp"


namespace mousse {
namespace regionModels {
namespace pyrolysisModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(reactingOneDim, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(pyrolysisModel, reactingOneDim, mesh);
ADD_TO_RUN_TIME_SELECTION_TABLE(pyrolysisModel, reactingOneDim, dictionary);


// Protected Member Functions 
void reactingOneDim::readReactingOneDimControls()
{
  const dictionary& solution = this->solution().subDict("SIMPLE");
  solution.lookup("nNonOrthCorr") >> nNonOrthCorr_;
  time().controlDict().lookup("maxDi") >> maxDiff_;
  coeffs().lookup("minimumDelta") >> minimumDelta_;
  coeffs().lookup("gasHSource") >> gasHSource_;
  coeffs().lookup("QrHSource") >> QrHSource_;
  useChemistrySolvers_ =
    coeffs().lookupOrDefault<bool>("useChemistrySolvers", true);
}


bool reactingOneDim::read()
{
  if (pyrolysisModel::read()) {
    readReactingOneDimControls();
    return true;
  }
  return false;
}


bool reactingOneDim::read(const dictionary& dict)
{
  if (pyrolysisModel::read(dict)) {
    readReactingOneDimControls();
    return true;
  }
  return false;
}


void reactingOneDim::updateQr()
{
  // Update local Qr from coupled Qr field
  Qr_ == dimensionedScalar{"zero", Qr_.dimensions(), 0.0};
  // Retrieve field from coupled region using mapped boundary conditions
  Qr_.correctBoundaryConditions();
  FOR_ALL(intCoupledPatchIDs_, i) {
    const label patchI = intCoupledPatchIDs_[i];
    scalarField& Qrp = Qr_.boundaryField()[patchI];
    // Qr is positive going in the solid
    // If the surface is emitting the radiative flux is set to zero
    Qrp = max(Qrp, scalar(0.0));
  }
  const vectorField& cellC = regionMesh().cellCentres();
  tmp<volScalarField> kappa = kappaRad();
  // Propagate Qr through 1-D regions
  label localPyrolysisFaceI = 0;
  FOR_ALL(intCoupledPatchIDs_, i) {
    const label patchI = intCoupledPatchIDs_[i];
    const scalarField& Qrp = Qr_.boundaryField()[patchI];
    const vectorField& Cf = regionMesh().Cf().boundaryField()[patchI];
    FOR_ALL(Qrp, faceI) {
      const scalar Qr0 = Qrp[faceI];
      point Cf0 = Cf[faceI];
      const labelList& cells = boundaryFaceCells_[localPyrolysisFaceI++];
      scalar kappaInt = 0.0;
      FOR_ALL(cells, k) {
        const label cellI = cells[k];
        const point& Cf1 = cellC[cellI];
        const scalar delta = mag(Cf1 - Cf0);
        kappaInt += kappa()[cellI]*delta;
        Qr_[cellI] = Qr0*exp(-kappaInt);
        Cf0 = Cf1;
      }
    }
  }
}


void reactingOneDim::updatePhiGas()
{
  phiHsGas_ ==  dimensionedScalar{"zero", phiHsGas_.dimensions(), 0.0};
  phiGas_ == dimensionedScalar{"zero", phiGas_.dimensions(), 0.0};
  const speciesTable& gasTable = solidChemistry_->gasTable();
  FOR_ALL(gasTable, gasI) {
    tmp<volScalarField> tHsiGas =
      solidChemistry_->gasHs(solidThermo_.p(), solidThermo_.T(), gasI);
    const volScalarField& HsiGas = tHsiGas();
    const DimensionedField<scalar, volMesh>& RRiGas =
      solidChemistry_->RRg(gasI);
    label totalFaceId = 0;
    FOR_ALL(intCoupledPatchIDs_, i) {
      const label patchI = intCoupledPatchIDs_[i];
      scalarField& phiGasp = phiGas_.boundaryField()[patchI];
      const scalarField& cellVol = regionMesh().V();
      FOR_ALL(phiGasp, faceI) {
        const labelList& cells = boundaryFaceCells_[totalFaceId++];
        scalar massInt = 0.0;
        FOR_ALL_REVERSE(cells, k) {
          const label cellI = cells[k];
          massInt += RRiGas[cellI]*cellVol[cellI];
          phiHsGas_[cellI] += massInt*HsiGas[cellI];
        }
        phiGasp[faceI] += massInt;
        if (debug) {
          Info
            << " Gas : " << gasTable[gasI]
            << " on patch : " << patchI
            << " mass produced at face(local) : "
            <<  faceI
            << " is : " << massInt
            << " [kg/s] " << endl;
        }
      }
    }
    tHsiGas().clear();
  }
}


void reactingOneDim::updateFields()
{
  if (QrHSource_) {
    updateQr();
  }
  updatePhiGas();
}


void reactingOneDim::updateMesh(const scalarField& mass0)
{
  if (!moveMesh_) {
    return;
  }
  const scalarField newV(mass0/rho_);
  Info << "Initial/final volumes = " << gSum(regionMesh().V()) << ", "
    << gSum(newV) << " [m3]" << endl;
  // move the mesh
  const labelList moveMap = moveMesh(regionMesh().V() - newV, minimumDelta_);
  // flag any cells that have not moved as non-reacting
  FOR_ALL(moveMap, i) {
    if (moveMap[i] == 0) {
      solidChemistry_->setCellReacting(i, false);
    }
  }
}


void reactingOneDim::solveContinuity()
{
  if (debug) {
    Info << "reactingOneDim::solveContinuity()" << endl;
  }
  const scalarField mass0 = rho_*regionMesh().V();
  fvScalarMatrix rhoEqn {
    fvm::ddt(rho_)
  ==
  - solidChemistry_->RRg()
  };
  if (regionMesh().moving()) {
    surfaceScalarField phiRhoMesh{fvc::interpolate(rho_)*regionMesh().phi()};
    rhoEqn += fvc::div(phiRhoMesh);
  }
  rhoEqn.solve();
  updateMesh(mass0);
}


void reactingOneDim::solveSpeciesMass()
{
  if (debug) {
    Info << "reactingOneDim::solveSpeciesMass()" << endl;
  }
  volScalarField Yt{0.0*Ys_[0]};
  for (label i=0; i<Ys_.size()-1; i++) {
    volScalarField& Yi = Ys_[i];
    fvScalarMatrix YiEqn {
      fvm::ddt(rho_, Yi)
    ==
      solidChemistry_->RRs(i)
    };
    if (regionMesh().moving()) {
      surfaceScalarField phiYiRhoMesh
      {
        fvc::interpolate(Yi*rho_)*regionMesh().phi()
      };
      YiEqn += fvc::div(phiYiRhoMesh);
    }
    YiEqn.solve(regionMesh().solver("Yi"));
    Yi.max(0.0);
    Yt += Yi;
  }
  Ys_[Ys_.size() - 1] = 1.0 - Yt;
}


void reactingOneDim::solveEnergy()
{
  if (debug) {
    Info << "reactingOneDim::solveEnergy()" << endl;
  }
  tmp<volScalarField> alpha{solidThermo_.alpha()};
  fvScalarMatrix hEqn {
    fvm::ddt(rho_, h_)
  - fvm::laplacian(alpha, h_)
  + fvc::laplacian(alpha, h_)
  - fvc::laplacian(kappa(), T())
  ==
    chemistrySh_
  - fvm::Sp(solidChemistry_->RRg(), h_)
  };
  if (gasHSource_) {
    const surfaceScalarField phiGas{fvc::interpolate(phiHsGas_)};
    hEqn += fvc::div(phiGas);
  }
  if (QrHSource_) {
    const surfaceScalarField phiQr{fvc::interpolate(Qr_)*nMagSf()};
    hEqn += fvc::div(phiQr);
  }
  if (regionMesh().moving()) {
    surfaceScalarField phihMesh
    {
      fvc::interpolate(rho_*h_)*regionMesh().phi()
    };
    hEqn += fvc::div(phihMesh);
  }
  hEqn.relax();
  hEqn.solve();
}


void reactingOneDim::calculateMassTransfer()
{
  totalGasMassFlux_ = 0;
  FOR_ALL(intCoupledPatchIDs_, i) {
    const label patchI = intCoupledPatchIDs_[i];
    totalGasMassFlux_ += gSum(phiGas_.boundaryField()[patchI]);
  }
  if (infoOutput_) {
    totalHeatRR_ = fvc::domainIntegrate(chemistrySh_);
    addedGasMass_ +=
      fvc::domainIntegrate(solidChemistry_->RRg())*time_.deltaT();
    lostSolidMass_ +=
      fvc::domainIntegrate(solidChemistry_->RRs())*time_.deltaT();
  }
}


// Constructors 
reactingOneDim::reactingOneDim
(
  const word& modelType,
  const fvMesh& mesh,
  const word& regionType
)
:
  pyrolysisModel{modelType, mesh, regionType},
  solidChemistry_{basicSolidChemistryModel::New(regionMesh())},
  solidThermo_{solidChemistry_->solidThermo()},
  radiation_{radiation::radiationModel::New(solidThermo_.T())},
  rho_
  {
    IOobject
    {
      "rho",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    solidThermo_.rho()
  },
  Ys_{solidThermo_.composition().Y()},
  h_{solidThermo_.he()},
  nNonOrthCorr_{-1},
  maxDiff_{10},
  minimumDelta_{1e-4},
  phiGas_
  {
    IOobject
    {
      "phiGas",
      time().timeName(),
      regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"zero", dimMass/dimTime, 0.0}
  },
  phiHsGas_
  {
    IOobject
    {
      "phiHsGas",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimTime, 0.0}
  },
  chemistrySh_
  {
    IOobject
    {
      "chemistrySh",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimTime/dimVolume, 0.0}
  },
  Qr_
  {
    IOobject
    {
      "Qr",
      time().timeName(),
      regionMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh()
    //dimensionedScalar("zero", dimEnergy/dimArea/dimTime, 0.0),
    //zeroGradientFvPatchVectorField::typeName
  },
  lostSolidMass_{"zero", dimMass, 0.0},
  addedGasMass_{"zero", dimMass, 0.0},
  totalGasMassFlux_{0.0},
  totalHeatRR_{"zero", dimEnergy/dimTime, 0.0},
  gasHSource_{false},
  QrHSource_{false},
  useChemistrySolvers_{true}
{
  if (active_) {
    read();
  }
}


reactingOneDim::reactingOneDim
(
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& regionType
)
:
  pyrolysisModel{modelType, mesh, dict, regionType},
  solidChemistry_{basicSolidChemistryModel::New(regionMesh())},
  solidThermo_{solidChemistry_->solidThermo()},
  radiation_{radiation::radiationModel::New(solidThermo_.T())},
  rho_
  {
    IOobject
    {
      "rho",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    solidThermo_.rho()
  },
  Ys_{solidThermo_.composition().Y()},
  h_{solidThermo_.he()},
  nNonOrthCorr_{-1},
  maxDiff_{10},
  minimumDelta_{1e-4},
  phiGas_
  {
    IOobject
    {
      "phiGas",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"zero", dimMass/dimTime, 0.0}
  },
  phiHsGas_
  {
    IOobject
    {
      "phiHsGas",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimTime, 0.0}
  },
  chemistrySh_
  {
    IOobject
    {
      "chemistrySh",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimTime/dimVolume, 0.0}
  },
  Qr_
  {
    IOobject
    {
      "Qr",
      time().timeName(),
      regionMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh()
  },
  lostSolidMass_{"zero", dimMass, 0.0},
  addedGasMass_{"zero", dimMass, 0.0},
  totalGasMassFlux_{0.0},
  totalHeatRR_{"zero", dimEnergy/dimTime, 0.0},
  gasHSource_{false},
  QrHSource_{false},
  useChemistrySolvers_{true}
{
  if (active_) {
    read(dict);
  }
}


// Destructor 
reactingOneDim::~reactingOneDim()
{}


// Member Functions 
scalar reactingOneDim::addMassSources(const label patchI, const label faceI)
{
  label index = 0;
  FOR_ALL(primaryPatchIDs_, i) {
    if (primaryPatchIDs_[i] == patchI) {
      index = i;
      break;
    }
  }
  const label localPatchId =  intCoupledPatchIDs_[index];
  const scalar massAdded = phiGas_.boundaryField()[localPatchId][faceI];
  if (debug) {
    Info << "\nPyrolysis region: " << type() << "added mass : "
      << massAdded << endl;
  }
  return massAdded;
}


scalar reactingOneDim::solidRegionDiffNo() const
{
  scalar DiNum = -GREAT;
  if (regionMesh().nInternalFaces() > 0) {
    surfaceScalarField KrhoCpbyDelta
    {
      regionMesh().surfaceInterpolation::deltaCoeffs()
      *fvc::interpolate(kappa())
      /fvc::interpolate(Cp()*rho_)
    };
    DiNum = max(KrhoCpbyDelta.internalField())*time().deltaTValue();
  }
  return DiNum;
}


scalar reactingOneDim::maxDiff() const
{
  return maxDiff_;
}


const volScalarField& reactingOneDim::rho() const
{
  return rho_;
}


const volScalarField& reactingOneDim::T() const
{
  return solidThermo_.T();
}


const tmp<volScalarField> reactingOneDim::Cp() const
{
  return solidThermo_.Cp();
}


tmp<volScalarField> reactingOneDim::kappaRad() const
{
  return radiation_->absorptionEmission().a();
}


tmp<volScalarField> reactingOneDim::kappa() const
{
  return solidThermo_.kappa();
}


const surfaceScalarField& reactingOneDim::phiGas() const
{
  return phiGas_;
}


void reactingOneDim::preEvolveRegion()
{
  pyrolysisModel::preEvolveRegion();
  // Initialise all cells as able to react
  FOR_ALL(h_, cellI) {
    solidChemistry_->setCellReacting(cellI, true);
  }
}


void reactingOneDim::evolveRegion()
{
  Info << "\nEvolving pyrolysis in region: " << regionMesh().name() << endl;
  if (useChemistrySolvers_) {
    solidChemistry_->solve(time().deltaTValue());
  } else {
    solidChemistry_->calculate();
  }
  solveContinuity();
  chemistrySh_ = solidChemistry_->Sh()();
  updateFields();
  solveSpeciesMass();
  for (int nonOrth=0; nonOrth<=nNonOrthCorr_; nonOrth++) {
    solveEnergy();
  }
  calculateMassTransfer();
  solidThermo_.correct();
  Info << "pyrolysis min/max(T) = "
    << min(solidThermo_.T().internalField())
    << ", "
    << max(solidThermo_.T().internalField())
    << endl;
}


void reactingOneDim::info()
{
  Info << "\nPyrolysis in region: " << regionMesh().name() << endl;
  Info << indent << "Total gas mass produced  [kg] = "
    << addedGasMass_.value() << nl
    << indent << "Total solid mass lost    [kg] = "
    << lostSolidMass_.value() << nl
    << indent << "Total pyrolysis gases  [kg/s] = "
    << totalGasMassFlux_ << nl
    << indent << "Total heat release rate [J/s] = "
    << totalHeatRR_.value() << nl;
}

}  // namespace mousse
}  // namespace regionModels
}  // namespace pyrolysisModels

