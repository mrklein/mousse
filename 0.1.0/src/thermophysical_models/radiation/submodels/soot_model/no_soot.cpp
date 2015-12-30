// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_soot.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    defineTypeNameAndDebug(noSoot, 0);
    addToRunTimeSelectionTable
    (
      sootModel,
      noSoot,
      dictionary
    );
  }
}
// Constructors 
mousse::radiation::noSoot::noSoot
(
  const dictionary& dict,
  const fvMesh& mesh,
  const word& modelType
)
:
  sootModel(dict, mesh, modelType)
{}
// Destructor 
mousse::radiation::noSoot::~noSoot()
{}
void mousse::radiation::noSoot::correct()
{
  //Do nothing
}
const mousse::volScalarField& mousse::radiation::noSoot::soot() const
{
  notImplemented
  (
    "mousse::volScalarField& mousse::radiation::noSoot::soot() const"
  );
  return tmp<volScalarField>(NULL);
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
