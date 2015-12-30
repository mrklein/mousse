// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_film.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
defineTypeNameAndDebug(noFilm, 0);
addToRunTimeSelectionTable(surfaceFilmModel, noFilm, mesh);
// Protected Member Functions 
bool noFilm::read()
{
  if (surfaceFilmModel::read())
  {
    // no additional info to read
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
noFilm::noFilm
(
  const word& modelType,
  const fvMesh& mesh,
  const dimensionedVector& g,
  const word& regionType
)
:
  surfaceFilmModel(modelType, mesh, g, regionType)
{}
// Destructor 
noFilm::~noFilm()
{}
// Member Functions 
void noFilm::addSources
(
  const label,
  const label,
  const scalar,
  const vector&,
  const scalar,
  const scalar
)
{
  // do nothing
}
const volScalarField& noFilm::delta() const
{
  FatalErrorIn("const volScalarField& noFilm::delta() const")
    << "delta field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::alpha() const
{
  FatalErrorIn("const volScalarField& noFilm::alpha() const")
    << "alpha field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volVectorField& noFilm::U() const
{
  FatalErrorIn("const volVectorField& noFilm::U() const")
    << "U field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}
const volVectorField& noFilm::Us() const
{
  FatalErrorIn("const volVectorField& noFilm::Us() const")
    << "Us field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}
const volVectorField& noFilm::Uw() const
{
  FatalErrorIn("const volVectorField& noFilm::Uw() const")
    << "Uw field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}
const volScalarField& noFilm::rho() const
{
  FatalErrorIn("const volScalarField& noFilm::rho() const")
    << "rho field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::T() const
{
  FatalErrorIn("const volScalarField& noFilm::T() const")
    << "T field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::Ts() const
{
  FatalErrorIn("const volScalarField& noFilm::Ts() const")
    << "Ts field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::Tw() const
{
  FatalErrorIn("const volScalarField& noFilm::Tw() const")
    << "Tw field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::Cp() const
{
  FatalErrorIn("const volScalarField& noFilm::Cp() const")
    << "Cp field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::kappa() const
{
  FatalErrorIn("const volScalarField& noFilm::kappa() const")
    << "kappa field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::sigma() const
{
  FatalErrorIn("const volScalarField& noFilm::sigma() const")
    << "sigma field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}
tmp<volScalarField> noFilm::primaryMassTrans() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "noFilm::primaryMassTrans",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      primaryMesh(),
      dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
    )
  );
}
const volScalarField& noFilm::cloudMassTrans() const
{
  FatalErrorIn("const volScalarField& noFilm::cloudMassTrans() const")
    << "cloudMassTrans field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noFilm::cloudDiameterTrans() const
{
  FatalErrorIn("const volScalarField& noFilm::cloudDiameterTrans() const")
    << "cloudDiameterTrans field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
tmp<DimensionedField<scalar, volMesh> > noFilm::Srho() const
{
  return tmp<DimensionedField<scalar, volMesh> >
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "noFilm::Srho",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      primaryMesh(),
      dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
    )
  );
}
tmp<DimensionedField<scalar, volMesh> > noFilm::Srho(const label i) const
{
  return tmp<DimensionedField<scalar, volMesh> >
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "noFilm::Srho(" + mousse::name(i) + ")",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      primaryMesh(),
      dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
    )
  );
}
tmp<DimensionedField<scalar, volMesh> > noFilm::Sh() const
{
  return tmp<DimensionedField<scalar, volMesh> >
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "noFilm::Sh",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      primaryMesh(),
      dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0.0)
    )
  );
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
