// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   turbulent_fluid_thermo_model.cpp
//   turbulent_fluid_thermo_models.cpp
#ifndef turbulent_fluid_thermo_model_hpp_
#define turbulent_fluid_thermo_model_hpp_
#include "_compressible_turbulence_model.hpp"
#include "thermal_diffusivity.hpp"
#include "eddy_diffusivity.hpp"
#include "ras_model.hpp"
#include "les_model.hpp"
#include "fluid_thermo.hpp"
namespace mousse
{
  namespace compressible
  {
    typedef ThermalDiffusivity<CompressibleTurbulenceModel<fluidThermo> >
      turbulenceModel;
    typedef RASModel<EddyDiffusivity<turbulenceModel> > RASModel;
    typedef LESModel<EddyDiffusivity<turbulenceModel> > LESModel;
    template<class BasicCompressibleTurbulenceModel>
    autoPtr<BasicCompressibleTurbulenceModel> New
    (
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi,
      const typename BasicCompressibleTurbulenceModel::transportModel&
        transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  }
}
#ifdef NoRepository
#   include "turbulent_fluid_thermo_model.cpp"
#endif
#endif
