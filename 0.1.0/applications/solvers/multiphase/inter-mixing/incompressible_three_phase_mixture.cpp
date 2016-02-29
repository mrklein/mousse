// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "incompressible_three_phase_mixture.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "fvc.hpp"
// Private Member Functions 
//- Calculate and return the laminar viscosity
void mousse::incompressibleThreePhaseMixture::calcNu()
{
  nuModel1_->correct();
  nuModel2_->correct();
  nuModel3_->correct();
  // Average kinematic viscosity calculated from dynamic viscosity
  nu_ = mu()/(alpha1_*rho1_ + alpha2_*rho2_ + alpha3_*rho3_);
}
// Constructors 
mousse::incompressibleThreePhaseMixture::incompressibleThreePhaseMixture
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
  phase1Name_(wordList(lookup("phases"))[0]),
  phase2Name_(wordList(lookup("phases"))[1]),
  phase3Name_(wordList(lookup("phases"))[2]),
  alpha1_
  (
    IOobject
    (
      IOobject::groupName("alpha", phase1Name_),
      U.time().timeName(),
      U.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    U.mesh()
  ),
  alpha2_
  (
    IOobject
    (
      IOobject::groupName("alpha", phase2Name_),
      U.time().timeName(),
      U.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    U.mesh()
  ),
  alpha3_
  (
    IOobject
    (
      IOobject::groupName("alpha", phase3Name_),
      U.time().timeName(),
      U.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    U.mesh()
  ),
  U_(U),
  phi_(phi),
  nu_
  (
    IOobject
    (
      "nu",
      U.time().timeName(),
      U.db()
    ),
    U.mesh(),
    dimensionedScalar("nu", dimensionSet(0, 2, -1, 0, 0), 0),
    calculatedFvPatchScalarField::typeName
  ),
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
  nuModel3_
  (
    viscosityModel::New
    (
      "nu3",
      subDict(phase3Name_),
      U,
      phi
    )
  ),
  rho1_("rho", dimDensity, nuModel1_->viscosityProperties()),
  rho2_("rho", dimDensity, nuModel2_->viscosityProperties()),
  rho3_("rho", dimDensity, nuModel3_->viscosityProperties())
{
  alpha3_ == 1.0 - alpha1_ - alpha2_;
  calcNu();
}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::incompressibleThreePhaseMixture::mu() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      "mu",
      alpha1_*rho1_*nuModel1_->nu()
     + alpha2_*rho2_*nuModel2_->nu()
     + alpha3_*rho3_*nuModel3_->nu()
    )
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::incompressibleThreePhaseMixture::muf() const
{
  surfaceScalarField alpha1f(fvc::interpolate(alpha1_));
  surfaceScalarField alpha2f(fvc::interpolate(alpha2_));
  surfaceScalarField alpha3f(fvc::interpolate(alpha3_));
  return tmp<surfaceScalarField>
  (
    new surfaceScalarField
    (
      "mu",
      alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
     + alpha2f*rho2_*fvc::interpolate(nuModel2_->nu())
     + alpha3f*rho3_*fvc::interpolate(nuModel3_->nu())
    )
  );
}
mousse::tmp<mousse::surfaceScalarField>
mousse::incompressibleThreePhaseMixture::nuf() const
{
  surfaceScalarField alpha1f(fvc::interpolate(alpha1_));
  surfaceScalarField alpha2f(fvc::interpolate(alpha2_));
  surfaceScalarField alpha3f(fvc::interpolate(alpha3_));
  return tmp<surfaceScalarField>
  (
    new surfaceScalarField
    (
      "nu",
      (
        alpha1f*rho1_*fvc::interpolate(nuModel1_->nu())
       + alpha2f*rho2_*fvc::interpolate(nuModel2_->nu())
       + alpha3f*rho3_*fvc::interpolate(nuModel3_->nu())
      )/(alpha1f*rho1_ + alpha2f*rho2_ + alpha3f*rho3_)
    )
  );
}
bool mousse::incompressibleThreePhaseMixture::read()
{
  if (transportModel::read())
  {
    if
    (
      nuModel1_().read(*this)
    && nuModel2_().read(*this)
    && nuModel3_().read(*this)
    )
    {
      nuModel1_->viscosityProperties().lookup("rho") >> rho1_;
      nuModel2_->viscosityProperties().lookup("rho") >> rho2_;
      nuModel3_->viscosityProperties().lookup("rho") >> rho3_;
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
