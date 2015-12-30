// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_scatter.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    defineTypeNameAndDebug(noScatter, 0);
    addToRunTimeSelectionTable(scatterModel, noScatter, dictionary);
  }
}
// Constructors 
mousse::radiation::noScatter::noScatter
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  scatterModel(dict, mesh)
{}
// Destructor 
mousse::radiation::noScatter::~noScatter()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::radiation::noScatter::sigmaEff() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "sigma",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh_,
      dimensionedScalar("zero", dimless/dimLength, 0.0)
    )
  );
}
