// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "y_plus.hpp"
#include "vol_fields.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(yPlus, 0);
}
// Private Member Functions 
void mousse::yPlus::writeFileHeader(const label /*i*/)
{
  writeHeader(file(), "y+ ()");
  writeCommented(file(), "Time");
  writeTabbed(file(), "patch");
  writeTabbed(file(), "min");
  writeTabbed(file(), "max");
  writeTabbed(file(), "average");
  file() << endl;
}
// Constructors 
mousse::yPlus::yPlus
(
  const word& name,
  const objectRegistry& obr,
  const dictionary&,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile(obr, name, typeName),
  name_(name),
  obr_(obr),
  active_(true),
  log_(true),
  phiName_("phi")
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WARNING_IN
    (
      "yPlus::yPlus"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_ << nl
      << endl;
  }
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* yPlusPtr
    (
      new volScalarField
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
    mesh.objectRegistry::store(yPlusPtr);
  }
}
// Destructor 
mousse::yPlus::~yPlus()
{}
// Member Functions 
void mousse::yPlus::read(const dictionary& dict)
{
  if (active_)
  {
    log_ = dict.lookupOrDefault<Switch>("log", true);
    phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
  }
}
void mousse::yPlus::execute()
{
  typedef compressible::turbulenceModel cmpModel;
  typedef incompressible::turbulenceModel icoModel;
  if (active_)
  {
    functionObjectFile::write();
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField& yPlus =
      const_cast<volScalarField&>
      (
        mesh.lookupObject<volScalarField>(type())
      );
    if (log_) Info<< type() << " " << name_ << " output:" << nl;
    tmp<volSymmTensorField> Reff;
    if (mesh.foundObject<cmpModel>(turbulenceModel::propertiesName))
    {
      const cmpModel& model =
        mesh.lookupObject<cmpModel>(turbulenceModel::propertiesName);
      calcYPlus(model, mesh, yPlus);
    }
    else if (mesh.foundObject<icoModel>(turbulenceModel::propertiesName))
    {
      const icoModel& model =
        mesh.lookupObject<icoModel>(turbulenceModel::propertiesName);
      calcYPlus(model, mesh, yPlus);
    }
    else
    {
      FATAL_ERROR_IN("void mousse::yPlus::write()")
        << "Unable to find turbulence model in the "
        << "database" << exit(FatalError);
    }
  }
}
void mousse::yPlus::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::yPlus::timeSet()
{
  // Do nothing
}
void mousse::yPlus::write()
{
  if (active_)
  {
    functionObjectFile::write();
    const volScalarField& yPlus =
      obr_.lookupObject<volScalarField>(type());
    if (log_) Info<< "    writing field " << yPlus.name() << nl << endl;
    yPlus.write();
  }
}
