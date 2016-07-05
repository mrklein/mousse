// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_scatter.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "thermo_cloud.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(cloudScatter, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  scatterModel,
  cloudScatter,
  dictionary
);

}
}


// Constructors 
mousse::radiation::cloudScatter::cloudScatter
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  scatterModel{dict, mesh},
  coeffsDict_{dict.subDict(typeName + "Coeffs")},
  cloudNames_{coeffsDict_.lookup("cloudNames")}
{}


// Destructor 
mousse::radiation::cloudScatter::~cloudScatter()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::cloudScatter::sigmaEff() const
{
  tmp<volScalarField> tsigma
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
  FOR_ALL(cloudNames_, i) {
    const thermoCloud& tc =
      mesh_.objectRegistry::lookupObject<thermoCloud>(cloudNames_[i]);
    tsigma() += tc.sigmap();
  }
  return 3.0*tsigma;
}

