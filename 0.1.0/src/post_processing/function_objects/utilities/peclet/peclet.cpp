// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "peclet.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "surface_fields.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(Peclet, 0);
}
// Constructors 
mousse::Peclet::Peclet
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  obr_(obr),
  active_(true),
  phiName_("phi"),
  rhoName_("rho")
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "Peclet::Peclet"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_ << nl
      << endl;
  }
  read(dict);
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    surfaceScalarField* PecletPtr
    (
      new surfaceScalarField
      (
        IOobject
        (
          type(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless, 0.0)
      )
    );
    mesh.objectRegistry::store(PecletPtr);
  }
}
// Destructor 
mousse::Peclet::~Peclet()
{}
// Member Functions 
void mousse::Peclet::read(const dictionary& dict)
{
  if (active_)
  {
    phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
    rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");
  }
}
void mousse::Peclet::execute()
{
  typedef compressible::turbulenceModel cmpTurbModel;
  typedef incompressible::turbulenceModel icoTurbModel;
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    tmp<volScalarField> nuEff;
    if (mesh.foundObject<cmpTurbModel>(turbulenceModel::propertiesName))
    {
      const cmpTurbModel& model =
        mesh.lookupObject<cmpTurbModel>
        (
          turbulenceModel::propertiesName
        );
      const volScalarField& rho =
        mesh.lookupObject<volScalarField>(rhoName_);
      nuEff = model.muEff()/rho;
    }
    else if
    (
      mesh.foundObject<icoTurbModel>(turbulenceModel::propertiesName)
    )
    {
      const icoTurbModel& model =
        mesh.lookupObject<icoTurbModel>
        (
          turbulenceModel::propertiesName
        );
      nuEff = model.nuEff();
    }
    else if (mesh.foundObject<dictionary>("transportProperties"))
    {
      const dictionary& model =
        mesh.lookupObject<dictionary>("transportProperties");
      nuEff =
        tmp<volScalarField>
        (
          new volScalarField
          (
            IOobject
            (
              "nuEff",
              mesh.time().timeName(),
              mesh,
              IOobject::NO_READ,
              IOobject::NO_WRITE
            ),
            mesh,
            dimensionedScalar(model.lookup("nu"))
          )
        );
    }
    else
    {
      FatalErrorIn("void mousse::Peclet::write()")
        << "Unable to determine the viscosity"
        << exit(FatalError);
    }
    const surfaceScalarField& phi =
      mesh.lookupObject<surfaceScalarField>(phiName_);
    surfaceScalarField& Peclet =
      const_cast<surfaceScalarField&>
      (
        mesh.lookupObject<surfaceScalarField>(type())
      );
    Peclet =
      mag(phi)
     /(
        mesh.magSf()
       *mesh.surfaceInterpolation::deltaCoeffs()
       *fvc::interpolate(nuEff)
      );
  }
}
void mousse::Peclet::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::Peclet::timeSet()
{
  // Do nothing
}
void mousse::Peclet::write()
{
  if (active_)
  {
    const surfaceScalarField& Peclet =
      obr_.lookupObject<surfaceScalarField>(type());
    Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << Peclet.name() << nl
      << endl;
    Peclet.write();
  }
}
