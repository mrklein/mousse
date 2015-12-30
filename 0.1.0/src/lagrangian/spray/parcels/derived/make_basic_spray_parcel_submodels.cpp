// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_spray_cloud.hpp"
#include "make_parcel_cloud_function_objects.hpp"
// Kinematic
#include "make_thermo_parcel_forces.hpp" // thermo variant
#include "make_thermo_parcel_turbulence_forces.hpp" // add turbulence variant
#include "make_parcel_dispersion_models.hpp"
#include "make_parcel_turbulence_dispersion_models.hpp" // add turbulence variant
#include "make_spray_parcel_injection_models.hpp" // Spray variant
#include "make_parcel_patch_interaction_models.hpp"
#include "make_spray_parcel_stochastic_collision_models.hpp" // Spray variant
// Thermodynamic
#include "make_parcel_heat_transfer_models.hpp"
// Reacting
#include "make_reacting_parcel_composition_models.hpp"
#include "make_reacting_parcel_phase_change_models.hpp"
#include "make_reacting_parcel_surface_film_models.hpp"
// Spray
#include "_distorted_sphere_drag_force.hpp"
#include "make_spray_parcel_atomization_models.hpp"
#include "make_spray_parcel_breakup_models.hpp"
makeParcelCloudFunctionObjects(basicSprayCloud);
// Kinematic sub-models
makeThermoParcelForces(basicSprayCloud);
makeThermoParcelTurbulenceForces(basicSprayCloud);
makeParcelDispersionModels(basicSprayCloud);
makeParcelTurbulenceDispersionModels(basicSprayCloud);
makeSprayParcelInjectionModels(basicSprayCloud);
makeParcelPatchInteractionModels(basicSprayCloud);
makeSprayParcelStochasticCollisionModels(basicSprayCloud);
// Thermo sub-models
makeParcelHeatTransferModels(basicSprayCloud);
// Reacting sub-models
makeReactingParcelCompositionModels(basicSprayCloud);
makeReactingParcelPhaseChangeModels(basicSprayCloud);
makeReactingParcelSurfaceFilmModels(basicSprayCloud);
// Spray sub-models
makeParticleForceModelType(DistortedSphereDragForce, basicSprayCloud);
makeSprayParcelAtomizationModels(basicSprayCloud);
makeSprayParcelBreakupModels(basicSprayCloud);
