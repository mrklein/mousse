// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_two_phase_mixture.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "fvc.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(incompressibleTwoPhaseMixture, 0);
}
// Private Member Functions 
//- Calculate and return the laminar viscosity
void mousse::incompressibleTwoPhaseMixture::calcNu()
{
  nuModel1_->correct();
  nuModel2_->correct();
  const volScalarField limitedAlpha1
  (
    "limitedAlpha1",
    min(max(alpha1_, scalar(0)), scalar(1))
  );
  // Average kinematic viscosity calculated from dynamic viscosity
  nu_ = mu()/(limitedAlpha1*rho1_ + (scalar(1) - limitedAlpha1)*rho2_);
}
// Constructors 
mousse::incompressibleTwoPhaseMixture::incompressibleTwoPhaseMixture
(
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  IOdictionary
  (
    IOobject
    (
      "transportProperties",
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  ),
  twoPhaseMixture(U.mesh(), *this),
  nuModel1_
  (
    viscosityModel::New
    (
      "nu1",
      subDict(phase1Name_),
      U,
      phi
    )
  ),
  nuModel2_
  (
    viscosityModel::New
    (
      "nu2",
      subDict(phase2Name_),
      U,
      phi
    )
  ),
  rho1_("rho", dimDensity, nuModel1_->viscosityProperties()),
  rho2_("rho", dimDensity, nuModel2_->viscosityProperties()),
  U_(U),
  phi_(phi),
  nu_
  (
    IOobject
    (
      "nu",
      U_.time().timeName(),
      U_.db()
    ),
    U_.mesh(),
    dimensionedScalar("nu", dimViscosity, 0),
    calculatedFvPatchScalarField::typeName
  )
{
  calcNu();
}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::incompressibleTwoPhaseMixture::mu() const
{
  const volScalarField limitedAlpha1
  (
    min(max(alpha1_, scalar(0)), scalar(1))
  );
  return tmp<volScalarField>
  (
    new volScalarField
    (
      "mu",
      limitedAlpha1*rho1_*nuModel1_->nu()
     + (scalar(1) - limitedAlpha1)*rho2_*nuModel2_->nu()
    )
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::incompressibleTwoPhaseMixture::muf() const
{
  const surfaceScalarField alpha1f
  (
    min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
  );
  return tmp<surfaceScalarField>
  (
    new surfaceScalarField
    (
      "muf",
      alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
     + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
    )
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::incompressibleTwoPhaseMixture::nuf() const
{
  const surfaceScalarField alpha1f
  (
    min(max(fvc::interpolate(alpha1_), scalar(0)), scalar(1))
  );
  return tmp<surfaceScalarField>
  (
    new surfaceScalarField
    (
      "nuf",
      (
        alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
       + (scalar(1) - alpha1f)*rho2_*fvc::interpolate(nuModel2_->nu())
      )/(alpha1f*rho1_ + (scalar(1) - alpha1f)*rho2_)
    )
  );
}
bool mousse::incompressibleTwoPhaseMixture::read()
{
  if (regIOobject::read())
  {
    if
    (
      nuModel1_().read
      (
        subDict(phase1Name_ == "1" ? "phase1": phase1Name_)
      )
    && nuModel2_().read
      (
        subDict(phase2Name_ == "2" ? "phase2": phase2Name_)
      )
    )
    {
      nuModel1_->viscosityProperties().lookup("rho") >> rho1_;
      nuModel2_->viscosityProperties().lookup("rho") >> rho2_;
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}
