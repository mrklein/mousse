#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MAKE_REACTION_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MAKE_REACTION_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "specie_mixture.hpp"

#define MAKE_REACTION_MIXTURE_THERMO\
(                                                                             \
  BaseThermo,                                                                 \
  CThermo,                                                                    \
  MixtureThermo,                                                              \
  Mixture,                                                                    \
  ThermoPhys                                                                  \
)                                                                             \
                                                                              \
typedef MixtureThermo                                                         \
<                                                                             \
  CThermo,                                                                    \
  SpecieMixture                                                               \
  <                                                                           \
    Mixture                                                                   \
    <                                                                         \
      ThermoPhys                                                              \
    >                                                                         \
  >                                                                           \
> MixtureThermo##Mixture##ThermoPhys;                                         \
                                                                              \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  MixtureThermo##Mixture##ThermoPhys,                                         \
  (#MixtureThermo"<"#Mixture"<" + ThermoPhys::typeName() + ">>").c_str(),     \
  0                                                                           \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  basicThermo,                                                                \
  MixtureThermo##Mixture##ThermoPhys,                                         \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                              \
  fluidThermo,                                                                \
  MixtureThermo##Mixture##ThermoPhys,                                         \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                              \
  BaseThermo,                                                                 \
  MixtureThermo##Mixture##ThermoPhys,                                         \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  CThermo,                                                                    \
  MixtureThermo##Mixture##ThermoPhys,                                         \
  fvMesh                                                                      \
);


#define MAKE_REACTION_THERMO\
(                                                                             \
  BaseThermo,                                                                 \
  CThermo,                                                                    \
  MixtureThermo,                                                              \
  Mixture,                                                                    \
  Transport,                                                                  \
  Type,                                                                       \
  Thermo,                                                                     \
  EqnOfState,                                                                 \
  Specie                                                                      \
)                                                                             \
                                                                              \
typedef                                                                       \
  Transport                                                                   \
  <                                                                           \
    species::thermo                                                           \
    <                                                                         \
      Thermo                                                                  \
      <                                                                       \
        EqnOfState                                                            \
        <                                                                     \
          Specie                                                              \
        >                                                                     \
      >,                                                                      \
      Type                                                                    \
    >                                                                         \
  > Transport##Type##Thermo##EqnOfState##Specie;                              \
                                                                              \
MAKE_REACTION_MIXTURE_THERMO                                                  \
(                                                                             \
  BaseThermo,                                                                 \
  CThermo,                                                                    \
  MixtureThermo,                                                              \
  Mixture,                                                                    \
  Transport##Type##Thermo##EqnOfState##Specie                                 \
);
#endif
