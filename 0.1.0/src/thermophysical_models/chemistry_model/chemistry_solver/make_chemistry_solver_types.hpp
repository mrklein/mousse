#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_MAKE_CHEMISTRY_SOLVER_TYPES_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_MAKE_CHEMISTRY_SOLVER_TYPES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "chemistry_solver.hpp"
#include "chemistry_model.hpp"
#include "no_chemistry_solver.hpp"
#include "euler_implicit.hpp"
#include "ode.hpp"

#define MAKE_CHEMISTRY_SOLVER_TYPE(SS, Comp, Thermo)                          \
                                                                              \
  typedef SS<chemistryModel<Comp, Thermo> > SS##Comp##Thermo;                 \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    SS##Comp##Thermo,                                                         \
    (#SS"<" + word(Comp::typeName_())                                         \
   + "," + Thermo::typeName() + ">").c_str(),                                 \
    0                                                                         \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    Comp,                                                                     \
    SS##Comp##Thermo,                                                         \
    fvMesh                                                                    \
  );


#define MAKE_CHEMISTRY_SOLVER_TYPES(CompChemModel,Thermo)                     \
                                                                              \
  MAKE_CHEMISTRY_SOLVER_TYPE                                                  \
  (                                                                           \
    noChemistrySolver,                                                        \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \
                                                                              \
  MAKE_CHEMISTRY_SOLVER_TYPE                                                  \
  (                                                                           \
    EulerImplicit,                                                            \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \
                                                                              \
  MAKE_CHEMISTRY_SOLVER_TYPE                                                  \
  (                                                                           \
    ode,                                                                      \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \

#endif
