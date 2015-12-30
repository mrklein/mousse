// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_solid_chemistry_model_hpp_
#define make_solid_chemistry_model_hpp_
#include "solid_chemistry_model.hpp"
#include "add_to_run_time_selection_table.hpp"
#define makeSolidChemistryModel(sChemistry, SS, Comp, SThermo)                \
                                       \
  typedef mousse::sChemistry<mousse::Comp, mousse::SThermo>                       \
    sChemistry##Comp##SThermo;                                            \
                                       \
  defineTemplateTypeNameAndDebugWithName                                    \
  (                                                                         \
    sChemistry##Comp##SThermo,                                            \
    (mousse::word(sChemistry##Comp##SThermo::typeName_()) + "<"#Comp","     \
    + SThermo::typeName() + ">").c_str(),                                 \
    0                                                                     \
  );
#define makeSolidGasChemistryModel(sChemistry, SS, Comp, SThermo, GThermo)    \
                                       \
  typedef mousse::SS<mousse::Comp, mousse::SThermo, mousse::GThermo>                \
    SS##Comp##SThermo##GThermo;                                           \
                                       \
  defineTemplateTypeNameAndDebugWithName                                    \
  (                                                                         \
    SS##Comp##SThermo##GThermo,                                           \
    (mousse::word(SS##Comp##SThermo##GThermo::typeName_()) + "<"#Comp","    \
    + SThermo::typeName() + "," + GThermo::typeName() + ">").c_str(),     \
    0                                                                     \
  );
#endif
