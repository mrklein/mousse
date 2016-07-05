// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_film.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noFilm, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(surfaceFilmModel, noFilm, mesh);


// Protected Member Functions 
bool noFilm::read()
{
  if (surfaceFilmModel::read()) {
    // no additional info to read
    return true;
  }
  return false;
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
  surfaceFilmModel{modelType, mesh, g, regionType}
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
  FATAL_ERROR_IN("const volScalarField& noFilm::delta() const")
    << "delta field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::alpha() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::alpha() const")
    << "alpha field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volVectorField& noFilm::U() const
{
  FATAL_ERROR_IN("const volVectorField& noFilm::U() const")
    << "U field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}


const volVectorField& noFilm::Us() const
{
  FATAL_ERROR_IN("const volVectorField& noFilm::Us() const")
    << "Us field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}


const volVectorField& noFilm::Uw() const
{
  FATAL_ERROR_IN("const volVectorField& noFilm::Uw() const")
    << "Uw field not available for " << type() << abort(FatalError);
  return volVectorField::null();
}


const volScalarField& noFilm::rho() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::rho() const")
    << "rho field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::T() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::T() const")
    << "T field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::Ts() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::Ts() const")
    << "Ts field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::Tw() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::Tw() const")
    << "Tw field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::Cp() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::Cp() const")
    << "Cp field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::kappa() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::kappa() const")
    << "kappa field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::sigma() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::sigma() const")
    << "sigma field not available for " << type() << abort(FatalError);
  return volScalarField::null();
}


tmp<volScalarField> noFilm::primaryMassTrans() const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "noFilm::primaryMassTrans",
          time().timeName(),
          primaryMesh(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        primaryMesh(),
        {"zero", dimMass/dimVolume/dimTime, 0.0}
      }
    };
}


const volScalarField& noFilm::cloudMassTrans() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::cloudMassTrans() const")
    << "cloudMassTrans field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noFilm::cloudDiameterTrans() const
{
  FATAL_ERROR_IN("const volScalarField& noFilm::cloudDiameterTrans() const")
    << "cloudDiameterTrans field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


tmp<DimensionedField<scalar, volMesh>> noFilm::Srho() const
{
  return
    tmp<DimensionedField<scalar, volMesh>>
    {
      new DimensionedField<scalar, volMesh>
      {
        {
          "noFilm::Srho",
          time().timeName(),
          primaryMesh(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        primaryMesh(),
        {"zero", dimMass/dimVolume/dimTime, 0.0}
      }
    };
}


tmp<DimensionedField<scalar, volMesh>> noFilm::Srho(const label i) const
{
  return
    tmp<DimensionedField<scalar, volMesh>>
    {
      new DimensionedField<scalar, volMesh>
      {
        {
          "noFilm::Srho(" + mousse::name(i) + ")",
          time().timeName(),
          primaryMesh(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        primaryMesh(),
        {"zero", dimMass/dimVolume/dimTime, 0.0}
      }
    };
}


tmp<DimensionedField<scalar, volMesh>> noFilm::Sh() const
{
  return
    tmp<DimensionedField<scalar, volMesh>>
    {
      new DimensionedField<scalar, volMesh>
      {
        {
          "noFilm::Sh",
          time().timeName(),
          primaryMesh(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        primaryMesh(),
        {"zero", dimEnergy/dimVolume/dimTime, 0.0}
      }
    };
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

