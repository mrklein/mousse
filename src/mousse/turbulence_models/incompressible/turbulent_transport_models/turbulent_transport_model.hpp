#ifndef TURBULENCE_MODELS_INCOMPRESSIBLE_TURBULENT_TRANSPORT_MODELS_TURBULENT_TRANSPORT_MODEL_HPP_
#define TURBULENCE_MODELS_INCOMPRESSIBLE_TURBULENT_TRANSPORT_MODELS_TURBULENT_TRANSPORT_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_incompressible_turbulence_model.hpp"
#include "ras_model.hpp"
#include "les_model.hpp"
#include "incompressible/transport_model.hpp"


namespace mousse {
namespace incompressible {

typedef IncompressibleTurbulenceModel<transportModel> turbulenceModel;
typedef RASModel<turbulenceModel> RASModel;
typedef LESModel<turbulenceModel> LESModel;
template<class BasicCompressibleTurbulenceModel>

autoPtr<BasicCompressibleTurbulenceModel> New
(
  const volVectorField& U,
  const surfaceScalarField& phi,
  const typename BasicCompressibleTurbulenceModel::transportModel&
    transport,
  const word& propertiesName = turbulenceModel::propertiesName
);

}
}

#include "turbulent_transport_model.ipp"

#endif
