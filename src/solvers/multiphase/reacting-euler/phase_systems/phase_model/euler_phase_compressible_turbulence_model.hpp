// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODEL_HPP_
#define EULER_PHASE_COMPRESSIBLE_TURBULENCE_MODEL_HPP_
#include "euler_phase_compressible_turbulence_model_fwd.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "thermal_diffusivity.hpp"
#include "phase_model.hpp"
namespace mousse
{
   typedef ThermalDiffusivity<PhaseCompressibleTurbulenceModel<phaseModel> >
     phaseCompressibleTurbulenceModel;
}
#endif
