#ifndef CORE_DB_RUN_TIME_SELECTION_ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE_HPP_
#define CORE_DB_RUN_TIME_SELECTION_ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


// add to hash-table of functions with typename as the key
#define ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,memberFunction,argNames)                                   \
                                                                              \
  /* Add the thisType memberFunction to the table */                          \
  baseType::add##memberFunction##argNames##MemberFunctionToTable<thisType>    \
  add##thisType##memberFunction##argNames##MemberFunctionTo##baseType##Table_

// add to hash-table of functions with 'lookup' as the key
#define ADD_NAMED_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,memberFunction,argNames,lookup)                            \
                                                                              \
  /* Add the thisType memberFunction to the table, find by lookup name */     \
  baseType::add##memberFunction##argNames##MemberFunctionToTable<thisType>    \
  add_##lookup##_##thisType##memberFunction##argNames##MemberFunctionTo##     \
  baseType##Table_(#lookup)

// add to hash-table of functions with typename as the key
// use when baseType doesn't need a template argument (eg, is a typedef)
#define ADD_TEMPLATE_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames)                              \
                                                                              \
  /* Add the thisType memberFunction to the table */                          \
  baseType::add##memberFunction##argNames##                                   \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add##thisType##Targ##memberFunction##argNames##MemberFunctionTo##           \
  baseType##Table_

// add to hash-table of functions with 'lookup' as the key
// use when baseType doesn't need a template argument (eg, is a typedef)
#define ADD_NAMED_TEMPLATE_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames,lookup)                       \
                                                                              \
  /* Add the thisType memberFunction to the table, find by lookup name */     \
  baseType::add##memberFunction##argNames##                                   \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add_##lookup##_##thisType##Targ##memberFunction##argNames##                 \
  MemberFunctionTo##baseType##Table_(#lookup)

// use when baseType requires the Targ template argument as well
#define ADD_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames)                              \
                                                                              \
  /* Add the thisType memberFunction to the table */                          \
  baseType<Targ>::add##memberFunction##argNames##                             \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add##thisType##Targ##memberFunction##argNames##MemberFunctionTo##           \
  baseType##Targ##Table_

// use when baseType requires the Targ template argument as well
#define ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames,lookup)                       \
                                                                              \
  /* Add the thisType memberFunction to the table, find by lookup name */     \
  baseType<Targ>::add##memberFunction##argNames##                             \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add_##lookup##_##thisType##Targ##memberFunction##argNames##                 \
  MemberFunctionTo##baseType##Targ##Table_(#lookup)

// add to hash-table of functions with typename as the key
// use when baseType requires the Targ template argument as well
#define ADD_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames)                              \
                                                                              \
  /* Add the thisType memberFunction to the table */                          \
  baseType<Targ>::add##memberFunction##argNames##                             \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add##thisType##Targ##memberFunction##argNames##MemberFunctionTo##           \
  baseType##Targ##Table_

// add to hash-table of functions with 'lookup' as the key
// use when baseType requires the Targ template argument as well
#define ADD_NAMED_TEMPLATED_TO_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,thisType,Targ,memberFunction,argNames,lookup)                       \
                                                                              \
  /* Add the thisType memberFunction to the table, find by lookup name */     \
  baseType<Targ>::add##memberFunction##argNames##                             \
  MemberFunctionToTable<thisType<Targ> >                                      \
  add_##lookup##_##thisType##Targ##memberFunction##argNames##                 \
  MemberFunctionTo##baseType##Targ##Table_(#lookup)

#endif
