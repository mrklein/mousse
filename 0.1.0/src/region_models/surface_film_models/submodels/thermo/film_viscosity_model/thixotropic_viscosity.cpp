// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thixotropic_viscosity.hpp"
#include "kinematic_single_layer.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvc_div.hpp"
#include "fvm_sup.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(thixotropicViscosity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmViscosityModel,
  thixotropicViscosity,
  dictionary
);
// Constructors 
thixotropicViscosity::thixotropicViscosity
(
  surfaceFilmModel& owner,
  const dictionary& dict,
  volScalarField& mu
)
:
  filmViscosityModel(typeName, owner, dict, mu),
  a_("a", dimless/dimTime, coeffDict_),
  b_("b", dimless, coeffDict_),
  d_("d", dimless, coeffDict_),
  c_("c", pow(dimTime, d_.value() - scalar(1)), coeffDict_),
  mu0_("mu0", dimPressure*dimTime, coeffDict_),
  muInf_("muInf", mu0_.dimensions(), coeffDict_),
  K_(1.0 - mousse::sqrt(muInf_/mu0_)),
  lambda_
  (
    IOobject
    (
      typeName + ":lambda",
      owner.regionMesh().time().timeName(),
      owner.regionMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    owner.regionMesh()
  )
{
  lambda_.min(1.0);
  lambda_.max(0.0);
  // Initialise viscosity to inf value because it cannot be evaluated yet
  mu_ = muInf_;
  mu_.correctBoundaryConditions();
}
// Destructor 
thixotropicViscosity::~thixotropicViscosity()
{}
// Member Functions 
void thixotropicViscosity::correct
(
  const volScalarField& /*p*/,
  const volScalarField& /*T*/
)
{
  const kinematicSingleLayer& film = filmType<kinematicSingleLayer>();
  // references to film fields
  const volVectorField& U = film.U();
  const volVectorField& Uw = film.Uw();
  const volScalarField& delta = film.delta();
  const volScalarField& deltaRho = film.deltaRho();
  const surfaceScalarField& phi = film.phi();
  const volScalarField& alpha = film.alpha();
  const Time& runTime = this->owner().regionMesh().time();
  // Shear rate
  volScalarField gDot("gDot", alpha*mag(U - Uw)/(delta + film.deltaSmall()));
  if (debug && runTime.outputTime())
  {
    gDot.write();
  }
  dimensionedScalar deltaRho0("deltaRho0", deltaRho.dimensions(), ROOTVSMALL);
  surfaceScalarField phiU(phi/fvc::interpolate(deltaRho + deltaRho0));
  dimensionedScalar c0("c0", dimless/dimTime, ROOTVSMALL);
  volScalarField coeff("coeff", -c_*pow(gDot, d_) + c0);
  // Limit the filmMass and deltaMass to calculate the effect of the added
  // droplets with lambda = 0 to the film
  const volScalarField filmMass
  (
    "thixotropicViscosity:filmMass",
    film.netMass() + dimensionedScalar("SMALL", dimMass, ROOTVSMALL)
  );
  const volScalarField deltaMass
  (
    "thixotropicViscosity:deltaMass",
    max(dimensionedScalar("zero", dimMass, 0), film.deltaMass())
  );
  fvScalarMatrix lambdaEqn
  (
    fvm::ddt(lambda_)
   + fvm::div(phiU, lambda_)
   - fvm::Sp(fvc::div(phiU), lambda_)
   ==
    a_*pow((1.0 - lambda_), b_)
   + fvm::SuSp(coeff, lambda_)
   - fvm::Sp(deltaMass/(runTime.deltaT()*filmMass), lambda_)
  );
  lambdaEqn.relax();
  lambdaEqn.solve();
  lambda_.min(1.0);
  lambda_.max(0.0);
  mu_ = muInf_/(sqr(1.0 - K_*lambda_) + ROOTVSMALL);
  mu_.correctBoundaryConditions();
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
