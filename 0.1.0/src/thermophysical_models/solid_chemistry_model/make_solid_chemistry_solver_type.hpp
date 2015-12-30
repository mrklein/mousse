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
#define makeSolidChemistrySolverType(SS, Schem, Comp, SThermo, GThermo)       \
                                       \
  typedef SS<Schem<Comp, SThermo, GThermo> >                                \
    SS##Schem##Comp##SThermo##GThermo;                                    \
                                       \
  defineTemplateTypeNameAndDebugWithName                                    \
  (                                                                         \
    SS##Schem##Comp##SThermo##GThermo,                                    \
    (#SS"<" + word(Schem<Comp, SThermo, GThermo>::typeName_())            \
   + "<"#Comp"," + SThermo::typeName()                                     \
   + ","  + GThermo::typeName() + ">>").c_str(),                           \
    0                                                                     \
  );                                                                        \
                                       \
  addToRunTimeSelectionTable                                                \
  (                                                                         \
    Comp,                                                                 \
    SS##Schem##Comp##SThermo##GThermo,                                    \
    fvMesh                                                                \
  );
#define makeSolidChemistrySolverTypes(SolidChem, Comp, SThermo, GThermo)      \
                                       \
  makeSolidChemistrySolverType                                              \
  (                                                                         \
    noChemistrySolver,                                                    \
    SolidChem,                                                            \
    Comp,                                                                 \
    SThermo,                                                              \
    GThermo                                                               \
  );                                                                        \
                                       \
  makeSolidChemistrySolverType                                              \
  (                                                                         \
    ode,                                                                  \
    SolidChem,                                                            \
    Comp,                                                                 \
    SThermo,                                                              \
    GThermo                                                               \
  );
}  // namespace mousse
#endif
