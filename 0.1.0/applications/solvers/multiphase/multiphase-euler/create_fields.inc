Info << "Reading field p_rgh\n" << endl;
volScalarField p_rgh
{
  {
    "p_rgh",
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE
  },
  mesh
};
volVectorField U
{
  {
    "U",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  mesh,
  {"U", dimVelocity, vector::zero}
};
surfaceScalarField phi
{
  {
    "phi",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  mesh,
  {"phi", dimArea*dimVelocity, 0}
};
multiphaseSystem fluid{U, phi};
FOR_ALL_ITER(PtrDictionary<phaseModel>, fluid.phases(), iter)
{
  phaseModel& phase = iter();
  const volScalarField& alpha = phase;
  U += alpha*phase.U();
  phi += fvc::interpolate(alpha)*phase.phi();
}
scalar slamDampCoeff{fluid.lookupOrDefault<scalar>("slamDampCoeff", 1)};
dimensionedScalar maxSlamVelocity
{
  "maxSlamVelocity",
  dimVelocity,
  fluid.lookupOrDefault<scalar>("maxSlamVelocity", GREAT)
};
volScalarField rho
{
  {
    "rho",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  fluid.rho()
};
// Construct incompressible turbulence model
autoPtr<incompressible::turbulenceModel> turbulence
{
  incompressible::turbulenceModel::New(U, phi, fluid)
};
#include "read_gravitational_acceleration.inc"
#include "read_href.inc"
#include "gh.inc"
volScalarField p
{
  {
    "p",
    runTime.timeName(),
    mesh,
    IOobject::NO_READ,
    IOobject::AUTO_WRITE
  },
  p_rgh + rho*gh
};
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell
(
  p,
  p_rgh,
  pimple.dict(),
  pRefCell,
  pRefValue
);
mesh.setFluxRequired(p_rgh.name());
