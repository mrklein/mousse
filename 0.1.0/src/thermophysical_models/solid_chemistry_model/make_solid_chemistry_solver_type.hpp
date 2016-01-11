// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef make_solid_chemistry_solver_type_hpp_
#define make_solid_chemistry_solver_type_hpp_
#include "add_to_run_time_selection_table.hpp"
#include "no_chemistry_solver.hpp"
#include "euler_implicit.hpp"
#include "ode.hpp"
namespace mousse
{
#define MAKE_SOLID_CHEMISTRY_SOLVER_TYPE(SS, Schem, Comp, SThermo, GThermo)   \
                                                                              \
  typedef SS<Schem<Comp, SThermo, GThermo> >                                  \
    SS##Schem##Comp##SThermo##GThermo;                                        \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    SS##Schem##Comp##SThermo##GThermo,                                        \
    (#SS"<" + word(Schem<Comp, SThermo, GThermo>::typeName_())                \
   + "<"#Comp"," + SThermo::typeName()                                        \
   + ","  + GThermo::typeName() + ">>").c_str(),                              \
    0                                                                         \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    Comp,                                                                     \
    SS##Schem##Comp##SThermo##GThermo,                                        \
    fvMesh                                                                    \
  );


#define MAKE_SOLID_CHEMISTRY_SOLVER_TYPES(SolidChem, Comp, SThermo, GThermo)  \
                                                                              \
  MAKE_SOLID_CHEMISTRY_SOLVER_TYPE                                            \
  (                                                                           \
    noChemistrySolver,                                                        \
    SolidChem,                                                                \
    Comp,                                                                     \
    SThermo,                                                                  \
    GThermo                                                                   \
  );                                                                          \
                                                                              \
  MAKE_SOLID_CHEMISTRY_SOLVER_TYPE                                            \
  (                                                                           \
    ode,                                                                      \
    SolidChem,                                                                \
    Comp,                                                                     \
    SThermo,                                                                  \
    GThermo                                                                   \
  );
}  // namespace mousse
#endif
