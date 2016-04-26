// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_scatter.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(constantScatter, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  scatterModel,
  constantScatter,
  dictionary
);

}
}


// Constructors 
mousse::radiation::constantScatter::constantScatter
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  scatterModel{dict, mesh},
  coeffsDict_{dict.subDict(typeName + "Coeffs")},
  sigma_{coeffsDict_.lookup("sigma")},
  C_{coeffsDict_.lookup("C")}
{}


// Destructor 
mousse::radiation::constantScatter::~constantScatter()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::constantScatter::sigmaEff() const
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
      sigma_*(3.0 - C_)
    }
  };
}
