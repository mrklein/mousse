// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef PHASE_COMPRESSIBLE_TURBULENCE_MODEL_FWD_HPP_
#define PHASE_COMPRESSIBLE_TURBULENCE_MODEL_FWD_HPP_
namespace mousse
{
  class phaseModel;
  template<class TransportModel>
  class PhaseCompressibleTurbulenceModel;
  template<class BasicTurbulenceModel>
  class ThermalDiffusivity;
   typedef ThermalDiffusivity<PhaseCompressibleTurbulenceModel<phaseModel> >
     phaseCompressibleTurbulenceModel;
}
#endif
