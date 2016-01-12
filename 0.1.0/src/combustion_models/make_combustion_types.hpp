// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_combustion_types_hpp_
#define make_combustion_types_hpp_
#include "add_to_run_time_selection_table.hpp"

#define MAKE_COMBUSTION_TYPES_THERMO(CombModel, CombType, Thermo, Table)      \
                                                                              \
  typedef mousse::combustionModels::CombModel                                 \
    <mousse::combustionModels::CombType, mousse::Thermo>                      \
    CombModel##CombType##Thermo;                                              \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
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
      ADD_TO_RUN_TIME_SELECTION_TABLE                                         \
      (                                                                       \
        Table,                                                                \
        CombModel##CombType##Thermo,                                          \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }


#define MAKE_COMBUSTION_TYPES(CombModel, CombType, Table)                     \
                                                                              \
  typedef mousse::combustionModels::CombModel                                 \
    <mousse::combustionModels::CombType>                                      \
    CombModel##CombType;                                                      \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
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
      ADD_TO_RUN_TIME_SELECTION_TABLE                                         \
      (                                                                       \
        Table,                                                                \
        CombModel##CombType,                                                  \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }

#endif
