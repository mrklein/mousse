// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_combustion_types_hpp_
#define make_combustion_types_hpp_
#include "add_to_run_time_selection_table.hpp"
#define makeCombustionTypesThermo(CombModel, CombType, Thermo, Table)         \
                                                                              \
  typedef mousse::combustionModels::CombModel                                 \
    <mousse::combustionModels::CombType, mousse::Thermo>                      \
    CombModel##CombType##Thermo;                                              \
                                                                              \
  defineTemplateTypeNameAndDebugWithName                                      \
  (                                                                           \
    CombModel##CombType##Thermo,                                              \
    #CombModel"<"#CombType","#Thermo">",                                      \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace combustionModels                                                \
    {                                                                         \
      typedef CombModel<CombType, Thermo> CombModel##CombType##Thermo;        \
      addToRunTimeSelectionTable                                              \
      (                                                                       \
        Table,                                                                \
        CombModel##CombType##Thermo,                                          \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }

#define makeCombustionTypes(CombModel, CombType, Table)                       \
                                                                              \
  typedef mousse::combustionModels::CombModel                                 \
    <mousse::combustionModels::CombType>                                      \
    CombModel##CombType;                                                      \
                                                                              \
  defineTemplateTypeNameAndDebugWithName                                      \
  (                                                                           \
    CombModel##CombType,                                                      \
    #CombModel"<"#CombType">",                                                \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace combustionModels                                                \
    {                                                                         \
      typedef CombModel<CombType> CombModel##CombType;                        \
                                                                              \
      addToRunTimeSelectionTable                                              \
      (                                                                       \
        Table,                                                                \
        CombModel##CombType,                                                  \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }

#endif
