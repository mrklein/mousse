// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_transport_model.hpp"
namespace mousse
{
  namespace incompressible
  {
    template<class BasicCompressibleTurbulenceModel>
    autoPtr<BasicCompressibleTurbulenceModel> New
    (
      const volVectorField& U,
      const surfaceScalarField& phi,
      const typename BasicCompressibleTurbulenceModel::transportModel&
        transport,
      const word& propertiesName
    )
    {
      return BasicCompressibleTurbulenceModel::New
      (
        geometricOneField(),
        geometricOneField(),
        U,
        phi,
        phi,
        transport,
        propertiesName
      );
    }
  }
}
