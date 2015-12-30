// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_solid_thermo_hpp_
#define make_solid_thermo_hpp_
#include "add_to_run_time_selection_table.hpp"
#define makeSolidThermo(BaseThermo,Cthermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie)\
                                       \
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
  heThermo                                                                  \
  <                                                                         \
    BaseThermo,                                                           \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                  \
  > heThermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;         \
                                       \
typedef                                                                       \
  Cthermo                                                                   \
  <                                                                         \
    BaseThermo,                                                           \
    Mixture<Transport##Type##Thermo##EqnOfState##Specie>                  \
  > Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie;          \
                                       \
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
);                                                                            \
                                       \
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
  BaseThermo,                                                               \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##Transport##Type##Thermo##EqnOfState##Specie,            \
  dictionary                                                                \
);
#define makeSolidThermoPhysicsType(BaseThermo,Cthermo,Mixture,SolidPhysicsType)\
                                       \
                                       \
                                       \
typedef                                                                       \
  heThermo                                                                  \
  <                                                                         \
    BaseThermo,                                                           \
    Mixture<SolidPhysicsType>                                             \
  > heThermo##Mixture##SolidPhysicsType;                                    \
                                       \
typedef                                                                       \
  Cthermo                                                                   \
  <                                                                         \
    BaseThermo,                                                           \
    Mixture<SolidPhysicsType>                                             \
  > Cthermo##Mixture##SolidPhysicsType;                                     \
                                       \
                                       \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
  Cthermo##Mixture##SolidPhysicsType,                                       \
  (                                                                         \
    #Cthermo"<"#Mixture"<"                                                \
   + SolidPhysicsType::typeName()                                          \
   + ">>"                                                                  \
  ).c_str(),                                                                \
  0                                                                         \
);                                                                            \
                                       \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  basicThermo,                                                              \
  Cthermo##Mixture##SolidPhysicsType,                                       \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##SolidPhysicsType,                                       \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##SolidPhysicsType,                                       \
  dictionary                                                                \
);
#endif
