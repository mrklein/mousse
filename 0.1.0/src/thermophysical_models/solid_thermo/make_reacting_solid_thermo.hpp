// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reacting_solid_thermo_hpp_
#define make_reacting_solid_thermo_hpp_
#include "add_to_run_time_selection_table.hpp"
#include "specie_mixture.hpp"
#define makeSolidReactionMixtureThermo(BaseThermo,Cthermo,Mixture,ThermoPhys) \
                                       \
typedef SpecieMixture                                                         \
  <                                                                         \
    Mixture                                                               \
    <                                                                     \
      ThermoPhys                                                        \
    >                                                                     \
  > SpecieMixture##Mixture##ThermoPhys;                                     \
                                       \
typedef                                                                       \
  heThermo                                                                  \
  <                                                                         \
    BaseThermo,                                                           \
    SpecieMixture##Mixture##ThermoPhys                                    \
  > heThermo##Mixture##ThermoPhys;                                          \
                                       \
typedef                                                                       \
  Cthermo                                                                   \
  <                                                                         \
    BaseThermo,                                                           \
    SpecieMixture##Mixture##ThermoPhys                                    \
  > Cthermo##Mixture##ThermoPhys;                                           \
                                       \
                                       \
defineTemplateTypeNameAndDebugWithName                                        \
(                                                                             \
  Cthermo##Mixture##ThermoPhys,                                             \
  (                                                                         \
    #Cthermo"<"#Mixture"<"                                                \
   + ThermoPhys::typeName()                                                \
   + ">>"                                                                  \
  ).c_str(),                                                                \
  0                                                                         \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  basicThermo,                                                              \
  Cthermo##Mixture##ThermoPhys,                                             \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##ThermoPhys,                                             \
  fvMesh                                                                    \
);                                                                            \
                                       \
addToRunTimeSelectionTable                                                    \
(                                                                             \
  BaseThermo,                                                               \
  Cthermo##Mixture##ThermoPhys,                                             \
  dictionary                                                                \
);
#define makeReactingSolidThermo(BaseThermo,CThermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie) \
                                       \
typedef                                                                        \
  Transport                                                                  \
  <                                                                          \
    species::thermo                                                        \
    <                                                                      \
      Thermo                                                             \
      <                                                                  \
        EqnOfState                                                     \
        <                                                              \
          Specie                                                     \
        >                                                              \
      >,                                                                 \
      Type                                                               \
    >                                                                      \
  > Transport##Type##Thermo##EqnOfState##Specie;                             \
                                       \
makeSolidReactionMixtureThermo                                                 \
(                                                                              \
  BaseThermo,                                                                \
  CThermo,                                                                   \
  Mixture,                                                                   \
  Transport##Type##Thermo##EqnOfState##Specie                                \
);
#endif
