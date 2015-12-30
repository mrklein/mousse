// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_chemistry_solver_types_hpp_
#define make_chemistry_solver_types_hpp_
#include "chemistry_solver.hpp"
#include "chemistry_model.hpp"
#include "no_chemistry_solver.hpp"
#include "euler_implicit.hpp"
#include "ode.hpp"

#define makeChemistrySolverType(SS, Comp, Thermo)                             \
                                                                              \
  typedef SS<chemistryModel<Comp, Thermo> > SS##Comp##Thermo;                 \
                                                                              \
  defineTemplateTypeNameAndDebugWithName                                      \
  (                                                                           \
    SS##Comp##Thermo,                                                         \
    (#SS"<" + word(Comp::typeName_())                                         \
   + "," + Thermo::typeName() + ">").c_str(),                                 \
    0                                                                         \
  );                                                                          \
                                                                              \
  addToRunTimeSelectionTable                                                  \
  (                                                                           \
    Comp,                                                                     \
    SS##Comp##Thermo,                                                         \
    fvMesh                                                                    \
  );

#define makeChemistrySolverTypes(CompChemModel,Thermo)                        \
                                                                              \
  makeChemistrySolverType                                                     \
  (                                                                           \
    noChemistrySolver,                                                        \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \
                                                                              \
  makeChemistrySolverType                                                     \
  (                                                                           \
    EulerImplicit,                                                            \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \
                                                                              \
  makeChemistrySolverType                                                     \
  (                                                                           \
    ode,                                                                      \
    CompChemModel,                                                            \
    Thermo                                                                    \
  );                                                                          \

#endif
