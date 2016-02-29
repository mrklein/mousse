#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_MAKE_SOOT_TYPES_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_MAKE_SOOT_TYPES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#define MAKE_SOOT_TYPES_THERMO(sootModelType, Thermo)                         \
  typedef mousse::radiation::sootModelType<mousse::Thermo>                    \
    sootModelType##Thermo;                                                    \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    sootModelType##Thermo,                                                    \
    #sootModelType"<"#Thermo">",                                              \
    0                                                                         \
  );                                                                          \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace radiation                                                       \
    {                                                                         \
      typedef sootModelType<Thermo> sootModelType##Thermo;                    \
                                                                              \
      ADD_TO_RUN_TIME_SELECTION_TABLE                                         \
      (                                                                       \
        sootModel,                                                            \
        sootModelType##Thermo,                                                \
        dictionary                                                            \
      );                                                                      \
    }                                                                         \
  }
#endif
