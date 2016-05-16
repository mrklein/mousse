// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_fluid_thermo_model.hpp"


namespace mousse {
namespace compressible {

template<class BasicCompressibleTurbulenceModel>
autoPtr<BasicCompressibleTurbulenceModel> New
(
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi,
  const typename BasicCompressibleTurbulenceModel::transportModel& transport,
  const word& propertiesName
)
{
  return
    BasicCompressibleTurbulenceModel::New
    (
      geometricOneField(),
      rho,
      U,
      phi,
      phi,
      transport,
      propertiesName
    );
}

}
}

