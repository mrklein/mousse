// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_thermo_hpp_
#define make_thermo_hpp_
#include "fluid_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"

#define MAKE_THERMO_TYPEDEFS\
(                                                                             \
 BaseThermo,                                                                  \
 Cthermo,                                                                     \
 Mixture,                                                                     \
 Transport,                                                                   \
 Type,                                                                        \
 Thermo,                                                                      \
 EqnOfState,                                                                  \
 Specie                                                                       \
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
typedef                                                                       \
  Cthermo                                                                     \
  <                                                                           \
    BaseThermo,                                                               \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                      \
  > Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;            \
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
);


#define MAKE_THERMO\
(                                                                             \
 BaseThermo,                                                                  \
 Cthermo,                                                                     \
 Mixture,                                                                     \
 Transport,                                                                   \
 Type,                                                                        \
 Thermo,                                                                      \
 EqnOfState,                                                                  \
 Specie                                                                       \
)                                                                             \
                                                                              \
MAKE_THERMO_TYPEDEFS                                                          \
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
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  basicThermo,                                                                \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  fluidThermo,                                                                \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  fvMesh                                                                      \
);                                                                            \
                                                                              \
ADD_TO_RUN_TIME_SELECTION_TABLE                                               \
(                                                                             \
  BaseThermo,                                                                 \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,              \
  fvMesh                                                                      \
);
#endif
