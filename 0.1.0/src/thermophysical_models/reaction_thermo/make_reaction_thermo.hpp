// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_reaction_thermo_hpp_
#define make_reaction_thermo_hpp_
#include "add_to_run_time_selection_table.hpp"
#include "specie_mixture.hpp"
#define makeReactionMixtureThermo(BaseThermo,CThermo,MixtureThermo,Mixture,ThermoPhys)   \
                                       \
typedef MixtureThermo                                                          \
<                                                                              \
  CThermo,                                                                   \
  SpecieMixture                                                              \
  <                                                                          \
    Mixture                                                                \
    <                                                                      \
      ThermoPhys                                                         \
    >                                                                      \
  >                                                                          \
> MixtureThermo##Mixture##ThermoPhys;                                          \
                                       \
defineTemplateTypeNameAndDebugWithName                                         \
(                                                                              \
  MixtureThermo##Mixture##ThermoPhys,                                        \
  (#MixtureThermo"<"#Mixture"<" + ThermoPhys::typeName() + ">>").c_str(),    \
  0                                                                          \
);                                                                             \
                                       \
addToRunTimeSelectionTable                                                     \
(                                                                              \
  basicThermo,                                                               \
  MixtureThermo##Mixture##ThermoPhys,                                        \
  fvMesh                                                                     \
);                                                                             \
                                       \
addToRunTimeSelectionTable                                                     \
(                                                                              \
  fluidThermo,                                                               \
  MixtureThermo##Mixture##ThermoPhys,                                        \
  fvMesh                                                                     \
);                                                                             \
                                       \
addToRunTimeSelectionTable                                                     \
(                                                                              \
  BaseThermo,                                                                \
  MixtureThermo##Mixture##ThermoPhys,                                        \
  fvMesh                                                                     \
);                                                                             \
                                       \
addToRunTimeSelectionTable                                                     \
(                                                                              \
  CThermo,                                                                   \
  MixtureThermo##Mixture##ThermoPhys,                                        \
  fvMesh                                                                     \
);
#define makeReactionThermo(BaseThermo,CThermo,MixtureThermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie) \
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
makeReactionMixtureThermo                                                      \
(                                                                              \
  BaseThermo,                                                                \
  CThermo,                                                                   \
  MixtureThermo,                                                             \
  Mixture,                                                                   \
  Transport##Type##Thermo##EqnOfState##Specie                                \
);
#endif
