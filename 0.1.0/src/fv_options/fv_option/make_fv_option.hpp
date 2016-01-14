// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_fv_option_hpp_
#define make_fv_option_hpp_
#include "fv_option.hpp"
#include "add_to_run_time_selection_table.hpp"
#define MAKE_FV_OPTION(Option, Type)                                          \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    mousse::fv::Option<mousse::Type>,                                         \
    #Type#Option,                                                             \
    0                                                                         \
  );                                                                          \
                                                                              \
  mousse::fv::option::adddictionaryConstructorToTable                         \
    <mousse::fv::Option<mousse::Type> >                                       \
    add##Option##Type##dictionary##ConstructorTooptionTable_
#endif
