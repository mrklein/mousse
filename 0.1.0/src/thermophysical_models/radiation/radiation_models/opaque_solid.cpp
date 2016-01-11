// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "opaque_solid.hpp"
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
    DEFINE_TYPE_NAME_AND_DEBUG(opaqueSolid, 0);
    addToRadiationRunTimeSelectionTables(opaqueSolid);
  }
}
// Constructors 
mousse::radiation::opaqueSolid::opaqueSolid(const volScalarField& T)
:
  radiationModel(typeName, T)
{}
mousse::radiation::opaqueSolid::opaqueSolid
(
  const dictionary& dict,
  const volScalarField& T
)
:
  radiationModel(typeName, dict, T)
{}
// Destructor 
mousse::radiation::opaqueSolid::~opaqueSolid()
{}
// Member Functions 
bool mousse::radiation::opaqueSolid::read()
{
  return radiationModel::read();
}
void mousse::radiation::opaqueSolid::calculate()
{
  // Do nothing
}
mousse::tmp<mousse::volScalarField> mousse::radiation::opaqueSolid::Rp() const
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
mousse::radiation::opaqueSolid::Ru() const
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
