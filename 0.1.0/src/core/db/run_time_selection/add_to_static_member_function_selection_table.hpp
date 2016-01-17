// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef add_to_static_member_function_selection_table_hpp_
#define add_to_static_member_function_selection_table_hpp_

// add to hash-table of functions with 'lookup' as the key
#define ADD_NAMED_TO_STATIC_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,memberFunction,argNames,lookup,functionPtr)                \
                                                                              \
  /* Add the thisType memberFunction to the table, find by lookup name */     \
  baseType::add##memberFunction##argNames##                                   \
  StaticMemberFunctionToTable<thisType>                                       \
  add_##lookup##_##thisType##memberFunction##argNames##                       \
  StaticMemberFunctionTo##baseType##Table_(#lookup, functionPtr)

#endif
