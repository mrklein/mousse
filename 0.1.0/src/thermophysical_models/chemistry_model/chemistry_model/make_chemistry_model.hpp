// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_chemistry_model_hpp_
#define make_chemistry_model_hpp_
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
#define MAKE_CHEMISTRY_MODEL(SS, Comp, Thermo)                                \
                                                                              \
  typedef SS<Comp, Thermo> SS##Comp##Thermo;                                  \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    SS##Comp##Thermo,                                                         \
    (#SS"<"#Comp"," + Thermo::typeName() + ">").c_str(),                      \
    0                                                                         \
  );
}  // namespace mousse
#endif
