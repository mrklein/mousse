// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_radiation.hpp"
#include "physico_chemical_constants.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    defineTypeNameAndDebug(noRadiation, 0);
    addToRadiationRunTimeSelectionTables(noRadiation);
  }
}
// Constructors 
mousse::radiation::noRadiation::noRadiation(const volScalarField& T)
:
  radiationModel(T)
{}
mousse::radiation::noRadiation::noRadiation
(
  const dictionary& dict,
  const volScalarField& T
)
:
  radiationModel(T)
{}
// Destructor 
mousse::radiation::noRadiation::~noRadiation()
{}
// Member Functions 
bool mousse::radiation::noRadiation::read()
{
  return radiationModel::read();
}
void mousse::radiation::noRadiation::calculate()
{
  // Do nothing
}
mousse::tmp<mousse::volScalarField> mousse::radiation::noRadiation::Rp() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "Rp",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar
      (
        "Rp",
        constant::physicoChemical::sigma.dimensions()/dimLength,
        0.0
      )
    )
  );
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::radiation::noRadiation::Ru() const
{
  return tmp<DimensionedField<scalar, volMesh> >
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "Ru",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar
      (
        "Ru", dimMass/dimLength/pow3(dimTime), 0.0
      )
    )
  );
}
