// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_soot_types_hpp_
#define make_soot_types_hpp_
#include "add_to_run_time_selection_table.hpp"
#define makeSootTypesThermo(sootModelType, Thermo)                            \
  typedef mousse::radiation::sootModelType<mousse::Thermo>                      \
    sootModelType##Thermo;                                                \
                                       \
  defineTemplateTypeNameAndDebugWithName                                    \
  (                                                                         \
    sootModelType##Thermo,                                                \
    #sootModelType"<"#Thermo">",                                          \
    0                                                                     \
  );                                                                        \
                                       \
  namespace mousse                                                            \
  {                                                                         \
    namespace radiation                                                   \
    {                                                                     \
      typedef sootModelType<Thermo> sootModelType##Thermo;              \
                                       \
      addToRunTimeSelectionTable                                        \
      (                                                                 \
        sootModel,                                                    \
        sootModelType##Thermo,                                        \
        dictionary                                                    \
      );                                                                \
    }                                                                     \
  }
#endif
