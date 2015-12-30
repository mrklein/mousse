// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_thermo_hpp_
#define make_thermo_hpp_
#include "fluid_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
#define makeThermoTypedefs(BaseThermo,Cthermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie)\
                                       \
typedef                                                                       \
  Transport                                                                 \
  <                                                                         \
    species::thermo                                                       \
    <                                                                     \
      Thermo                                                            \
      <                                                                 \
        EqnOfState                                                    \
        <                                                             \
          Specie                                                    \
        >                                                             \
      >,                                                                \
      Type                                                              \
    >                                                                     \
  > Transport##Type##Thermo##EqnOfState##Specie;                            \
                                       \
typedef                                                                       \
  Cthermo                                                                   \
  <                                                                         \
    BaseThermo,                                                           \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                  \
  > Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;          \
                                       \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  (                                                                         \
    #Cthermo"<"#Mixture"<"                                                \
   + Transport##Type##Thermo##EqnOfState##Specie::typeName()               \
   + ">>"                                                                  \
  ).c_str(),                                                                \
  0                                                                         \
);
#define makeThermo(BaseThermo,Cthermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie)\
                                       \
makeThermoTypedefs                                                            \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo,                                                                  \
  Mixture,                                                                  \
  Transport,                                                                \
  Type,                                                                     \
  Thermo,                                                                   \
  EqnOfState,                                                               \
  Specie                                                                    \
)                                                                             \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  basicThermo,                                                              \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  fluidThermo,                                                              \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  fvMesh                                                                    \
);
#endif
