#ifndef CORE_DB_RUN_TIME_SELECTION_STATIC_MEMBER_FUNCTION_SELECTION_TABLES_HPP_
#define CORE_DB_RUN_TIME_SELECTION_STATIC_MEMBER_FUNCTION_SELECTION_TABLES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "member_function_selection_tables.hpp"


// external use:
// ~~~~~~~~~~~~~
// declare a run-time selection:
#define DECLARE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE\
(returnType,baseType,memberFunction,argNames,argList,parList)                 \
                                                                              \
  /* Construct from argList function pointer type */                          \
  typedef returnType (*memberFunction##argNames##MemberFunctionPtr)argList;   \
                                                                              \
  /* Construct from argList function table type */                            \
  typedef HashTable                                                           \
    <memberFunction##argNames##MemberFunctionPtr, word, string::hash>         \
    memberFunction##argNames##MemberFunctionTable;                            \
                                                                              \
  /* Construct from argList function pointer table pointer */                 \
  static memberFunction##argNames##MemberFunctionTable*                       \
    memberFunction##argNames##MemberFunctionTablePtr_;                        \
                                                                              \
  /* Table memberFunction called from the table add function */               \
  static void construct##memberFunction##argNames##MemberFunctionTables();    \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  static void destroy##memberFunction##argNames##MemberFunctionTables();      \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  template<class baseType##Type>                                              \
  class add##memberFunction##argNames##StaticMemberFunctionToTable            \
  {                                                                           \
  public:                                                                     \
                                                                              \
    add##memberFunction##argNames##StaticMemberFunctionToTable                \
    (                                                                         \
      const word& lookup,                                                     \
      memberFunction##argNames##MemberFunctionPtr function                    \
    )                                                                         \
    {                                                                         \
      construct##memberFunction##argNames##MemberFunctionTables();            \
      memberFunction##argNames##MemberFunctionTablePtr_->insert               \
      (                                                                       \
        lookup,                                                               \
        function                                                              \
      );                                                                      \
    }                                                                         \
                                                                              \
    ~add##memberFunction##argNames##StaticMemberFunctionToTable()             \
    {                                                                         \
      destroy##memberFunction##argNames##MemberFunctionTables();              \
    }                                                                         \
  }


// internal use:
// constructor/destructor aid
#define DEFINE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE_CONSTRUCT_DESTRUCT\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Table constructor called from the table add function constructor */      \
  void baseType::construct##memberFunction##argNames##MemberFunctionTables()  \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      baseType::memberFunction##argNames##MemberFunctionTablePtr_             \
        = new baseType::memberFunction##argNames##MemberFunctionTable;        \
    }                                                                         \
  };                                                                          \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void baseType::destroy##memberFunction##argNames##MemberFunctionTables()    \
  {                                                                           \
    if (baseType::memberFunction##argNames##MemberFunctionTablePtr_)          \
    {                                                                         \
      delete baseType::memberFunction##argNames##MemberFunctionTablePtr_;     \
      baseType::memberFunction##argNames##MemberFunctionTablePtr_ = NULL;     \
    }                                                                     \
  }


// internal use:
// create pointer to hash-table of functions
#define DEFINE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE_PTR\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Define the memberFunction table */                                       \
  baseType::memberFunction##argNames##MemberFunctionTable*                    \
    baseType::memberFunction##argNames##MemberFunctionTablePtr_ = NULL


// external use:
// ~~~~~~~~~~~~~
// define run-time selection table
#define DEFINE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  DEFINE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE_PTR                           \
    (baseType,memberFunction,argNames);                                       \
  DEFINE_STATIC_MEMBER_FUNCTION_SELECTION_TABLE_CONSTRUCT_DESTRUCT            \
    (baseType,memberFunction,argNames)                                        \

#endif
