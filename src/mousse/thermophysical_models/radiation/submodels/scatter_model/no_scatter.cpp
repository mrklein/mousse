// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_scatter.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(noScatter, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(scatterModel, noScatter, dictionary);

}
}


// Constructors 
mousse::radiation::noScatter::noScatter
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  scatterModel{dict, mesh}
{}


// Destructor 
mousse::radiation::noScatter::~noScatter()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::radiation::noScatter::sigmaEff() const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "sigma",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh_,
      {"zero", dimless/dimLength, 0.0}
    }
  };
}
