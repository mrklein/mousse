#ifndef THERMOPHYSICAL_MODELS_SOLID_CHEMISTRY_MODEL_MAKE_SOLID_CHEMISTRY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_CHEMISTRY_MODEL_MAKE_SOLID_CHEMISTRY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_chemistry_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#define MAKE_SOLID_CHEMISTRY_MODEL(sChemistry, SS, Comp, SThermo)             \
                                                                              \
  typedef mousse::sChemistry<mousse::Comp, mousse::SThermo>                   \
    sChemistry##Comp##SThermo;                                                \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    sChemistry##Comp##SThermo,                                                \
    (mousse::word(sChemistry##Comp##SThermo::typeName_()) + "<"#Comp","       \
    + SThermo::typeName() + ">").c_str(),                                     \
    0                                                                         \
  );


#define MAKE_SOLID_GAS_CHEMISTRY_MODEL(sChemistry, SS, Comp, SThermo, GThermo)\
                                                                              \
  typedef mousse::SS<mousse::Comp, mousse::SThermo, mousse::GThermo>          \
    SS##Comp##SThermo##GThermo;                                               \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    SS##Comp##SThermo##GThermo,                                               \
    (mousse::word(SS##Comp##SThermo##GThermo::typeName_()) + "<"#Comp","      \
    + SThermo::typeName() + "," + GThermo::typeName() + ">").c_str(),         \
    0                                                                         \
  );
#endif
