// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "laminar.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "fv_matrices.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "fvm_sup.hpp"
#include "kinematic_single_layer.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(laminar, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(filmTurbulenceModel, laminar, dictionary);


// Constructors 
laminar::laminar
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmTurbulenceModel{type(), owner, dict},
  Cf_{readScalar(coeffDict_.lookup("Cf"))}
{}


// Destructor 
laminar::~laminar()
{}


// Member Functions 
tmp<volVectorField> laminar::Us() const
{
  tmp<volVectorField> tUs
  {
    new volVectorField
    {
      {
        typeName + ":Us",
        owner_.regionMesh().time().timeName(),
        owner_.regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner_.regionMesh(),
      {"zero", dimVelocity, vector::zero},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  // apply quadratic profile
  tUs() = mousse::sqrt(2.0)*owner_.U();
  tUs().correctBoundaryConditions();
  return tUs;
}


tmp<volScalarField> laminar::mut() const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        typeName + ":mut",
        owner_.regionMesh().time().timeName(),
        owner_.regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner_.regionMesh(),
      {"zero", dimMass/dimLength/dimTime, 0.0}
    }
  };
}


void laminar::correct()
{
  // do nothing
}


tmp<fvVectorMatrix> laminar::Su(volVectorField& U) const
{
  // local reference to film model
  const kinematicSingleLayer& film =
    static_cast<const kinematicSingleLayer&>(owner_);
  // local references to film fields
  const volScalarField& mu = film.mu();
  const volVectorField& Uw = film.Uw();
  const volScalarField& delta = film.delta();
  const volVectorField& Up = film.UPrimary();
  const volScalarField& rhop = film.rhoPrimary();
  // employ simple coeff-based model
  volScalarField Cs{"Cs", Cf_*rhop*mag(Up - U)};
  volScalarField Cw{"Cw", mu/(0.3333*(delta + film.deltaSmall()))};
  Cw.min(5000.0);
  return
    (
      -fvm::Sp(Cs, U) + Cs*Up // surface contribution
      - fvm::Sp(Cw, U) + Cw*Uw // wall contribution
    );
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

