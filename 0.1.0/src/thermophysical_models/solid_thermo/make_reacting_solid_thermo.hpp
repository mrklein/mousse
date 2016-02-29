#ifndef THERMOPHYSICAL_MODELS_SOLID_THERMO_MAKE_REACTING_SOLID_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_THERMO_MAKE_REACTING_SOLID_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "specie_mixture.hpp"

#define MAKE_SOLID_REACTION_MIXTURE_THERMO\
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo,                                                                    \
  Mixture,                                                                    \
  ThermoPhys                                                                  \
)                                                                             \
                                                                              \
typedef SpecieMixture                                                         \
  <                                                                           \
    Mixture                                                                   \
    <                                                                         \
      ThermoPhys                                                              \
    >                                                                         \
  > SpecieMixture##Mixture##ThermoPhys;                                       \
                                                                              \
typedef                                                                       \
  heThermo                                                                    \
  <                                                                           \
    BaseThermo,                                                               \
    SpecieMixture##Mixture##ThermoPhys                                        \
  > heThermo##Mixture##ThermoPhys;                                            \
                                                                              \
typedef                                                                       \
  Cthermo                                                                     \
  <                                                                           \
    BaseThermo,                                                               \
    SpecieMixture##Mixture##ThermoPhys                                        \
  > Cthermo##Mixture##ThermoPhys;                                             \
                                                                              \
                                                                              \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  Cthermo##Mixture##ThermoPhys,                                               \
  (                                                                           \
    #Cthermo"<"#Mixture"<"                                                    \
   + ThermoPhys::typeName()                                                   \
   + ">>"                                                                     \
  ).c_str(),                                                                  \
  0                                                                           \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  basicThermo,                                                                \
  Cthermo##Mixture##ThermoPhys,                                               \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##ThermoPhys,                                               \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##ThermoPhys,                                               \
  dictionary                                                                  \
);


#define MAKE_REACTING_SOLID_THERMO\
(                                                                             \
  BaseThermo,                                                                 \
  CThermo,                                                                    \
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
MAKE_SOLID_REACTION_MIXTURE_THERMO                                            \
(                                                                             \
  BaseThermo,                                                                 \
  CThermo,                                                                    \
  Mixture,                                                                    \
  Transport##Type##Thermo##EqnOfState##Specie                                 \
);
#endif
