#ifndef THERMOPHYSICAL_MODELS_SOLID_THERMO_MAKE_SOLID_THERMO_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_THERMO_MAKE_SOLID_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"


#define MAKE_SOLID_THERMO\
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo,                                                                    \
  Mixture,                                                                    \
  Transport,                                                                  \
  Type,                                                                       \
  Thermo,                                                                     \
  EqnOfState,                                                                 \
  Specie                                                                      \
)                                                                             \
                                                                              \
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
typedef                                                                       \
  heThermo                                                                    \
  <                                                                           \
    BaseThermo,                                                               \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                      \
  > heThermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;           \
                                                                              \
typedef                                                                       \
  Cthermo                                                                     \
  <                                                                           \
    BaseThermo,                                                               \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                      \
  > Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;            \
                                                                              \
                                                                              \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  (                                                                           \
    #Cthermo"<"#Mixture"<"                                                    \
   + Transport##Type##Thermo##EqnOfState##Specie::typeName()                  \
   + ">>"                                                                     \
  ).c_str(),                                                                  \
  0                                                                           \
);                                                                            \
                                                                              \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  basicThermo,                                                                \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  dictionary                                                                  \
);


#define MAKE_SOLID_THERMO_PHYSICS_TYPE\
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo,                                                                    \
  Mixture,                                                                    \
  SolidPhysicsType                                                            \
 )                                                                            \
                                                                              \
                                                                              \
                                                                              \
typedef                                                                       \
  heThermo                                                                    \
  <                                                                           \
    BaseThermo,                                                               \
    Mixture<SolidPhysicsType>                                                 \
  > heThermo##Mixture##SolidPhysicsType;                                      \
                                                                              \
typedef                                                                       \
  Cthermo                                                                     \
  <                                                                           \
    BaseThermo,                                                               \
    Mixture<SolidPhysicsType>                                                 \
  > Cthermo##Mixture##SolidPhysicsType;                                       \
                                                                              \
                                                                              \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  Cthermo##Mixture##SolidPhysicsType,                                         \
  (                                                                           \
    #Cthermo"<"#Mixture"<"                                                    \
   + SolidPhysicsType::typeName()                                             \
   + ">>"                                                                     \
  ).c_str(),                                                                  \
  0                                                                           \
);                                                                            \
                                                                              \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  basicThermo,                                                                \
  Cthermo##Mixture##SolidPhysicsType,                                         \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                                    \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##SolidPhysicsType,                                         \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##SolidPhysicsType,                                         \
  dictionary                                                                  \
);

#endif

