// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pdr_kepsilon.hpp"
#include "pdr_drag_model.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace compressible
{
namespace RASModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(PDRkEpsilon, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(RASModel, PDRkEpsilon, dictionary);
// Constructors 
PDRkEpsilon::PDRkEpsilon
(
  const geometricOneField& /*alpha*/,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& /*alphaRhoPhi*/,
  const surfaceScalarField& phi,
  const fluidThermo& thermophysicalModel,
  const word& turbulenceModelName,
  const word& modelName
)
:
  mousse::RASModels::kEpsilon<EddyDiffusivity<compressible::turbulenceModel> >
  {
    geometricOneField(),
    rho,
    U,
    phi,
    phi,
    thermophysicalModel,
    turbulenceModelName,
    modelName
  },
  C4_
  {
    dimensioned<scalar>::lookupOrAddToDict
    (
      "C4",
      coeffDict_,
      0.1
    )
  }
{}
// Destructor 
PDRkEpsilon::~PDRkEpsilon()
{}
// Member Functions 
bool PDRkEpsilon::read()
{
  if (RASModel::read())
  {
    C4_.readIfPresent(coeffDict_);
    return true;
  }
  else
  {
    return false;
  }
}
void PDRkEpsilon::correct()
{
  if (!turbulence_)
  {
    // Re-calculate viscosity
    nut_ = Cmu_*sqr(k_)/epsilon_;
    nut_.correctBoundaryConditions();
    // Re-calculate thermal diffusivity
    //***HGWalphat_ = mut_/Prt_;
    //alphat_.correctBoundaryConditions();
    return;
  }
  RASModel::correct();
  volScalarField divU{fvc::div(phi_/fvc::interpolate(rho_))};
  if (mesh_.moving())
  {
    divU += fvc::div(mesh_.phi());
  }
  tmp<volTensorField> tgradU = fvc::grad(U_);
  volScalarField G{GName(), rho_*nut_*(tgradU() && dev(twoSymm(tgradU())))};
  tgradU.clear();
  // Update espsilon and G at the wall
  epsilon_.boundaryField().updateCoeffs();
  // Add the blockage generation term so that it is included consistently
  // in both the k and epsilon equations
  const volScalarField& betav = U_.db().lookupObject<volScalarField>("betav");
  const volScalarField& Lobs =
    U_.db().lookupObject<volScalarField>("Lobs");
  const PDRDragModel& drag =
    U_.db().lookupObject<PDRDragModel>("PDRDragModel");
  volScalarField GR{drag.Gk()};
  volScalarField LI
  {
    C4_*(Lobs + dimensionedScalar("minLength", dimLength, VSMALL))
  };
  // Dissipation equation
  tmp<fvScalarMatrix> epsEqn
  {
    betav*fvm::ddt(rho_, epsilon_)
    + fvm::div(phi_, epsilon_)
    - fvm::laplacian(rho_*DepsilonEff(), epsilon_)
    ==
    C1_*betav*G*epsilon_/k_
    + 1.5*pow(Cmu_, 3.0/4.0)*GR*sqrt(k_)/LI
    - fvm::SuSp(((2.0/3.0)*C1_)*betav*rho_*divU, epsilon_)
    - fvm::Sp(C2_*betav*rho_*epsilon_/k_, epsilon_)
  };
  epsEqn().relax();
  epsEqn().boundaryManipulate(epsilon_.boundaryField());
  solve(epsEqn);
  bound(epsilon_, epsilonMin_);
  // Turbulent kinetic energy equation
  tmp<fvScalarMatrix> kEqn
  {
    betav*fvm::ddt(rho_, k_)
    + fvm::div(phi_, k_)
    - fvm::laplacian(rho_*DkEff(), k_)
    ==
    betav*G + GR
    - fvm::SuSp((2.0/3.0)*betav*rho_*divU, k_)
    - fvm::Sp(betav*rho_*epsilon_/k_, k_)
  };
  kEqn().relax();
  solve(kEqn);
  bound(k_, kMin_);
  // Re-calculate viscosity
  nut_ = Cmu_*sqr(k_)/epsilon_;
  nut_.correctBoundaryConditions();
  // Re-calculate thermal diffusivity
  //***HGWalphat_ = mut_/Prt_;
  //alphat_.correctBoundaryConditions();
}
}  // namespace RASModels
}  // namespace compressible
}  // namespace mousse
