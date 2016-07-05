#ifndef CORE_DB_RUN_TIME_SELECTION_ADD_TO_GLOBAL_FUNCTION_SELECTION_TABLE_HPP_
#define CORE_DB_RUN_TIME_SELECTION_ADD_TO_GLOBAL_FUNCTION_SELECTION_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


// add to hash-table of functions with 'lookup' as the key
#define ADD_NAMED_TO_GLOBAL_FUNCTION_SELECTION_TABLE\
(memberFunction,argNames,lookup,functionPtr)                                  \
                                                                              \
  /* Add to the table, find by lookup name */                                 \
  add##memberFunction##argNames##GlobalMemberFunctionToTable                  \
  add_##lookup##_##memberFunction##argNames##GlobalMemberFunctionTo##Table_   \
  (#lookup, functionPtr)

#endif
