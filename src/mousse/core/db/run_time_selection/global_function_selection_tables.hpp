#ifndef CORE_DB_RUN_TIME_SELECTION_GLOBAL_MEMBER_FUNCTION_SELECTION_TABLES_HPP_
#define CORE_DB_RUN_TIME_SELECTION_GLOBAL_MEMBER_FUNCTION_SELECTION_TABLES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "member_function_selection_tables.hpp"


// external use:
// ~~~~~~~~~~~~~
// declare a run-time selection:
#define DECLARE_GLOBAL_FUNCTION_SELECTION_TABLE\
(returnType,memberFunction,argNames,argList,parList)                          \
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
  extern memberFunction##argNames##MemberFunctionTable*                       \
    memberFunction##argNames##MemberFunctionTablePtr_;                        \
                                                                              \
  /* Table memberFunction called from the table add function */               \
  void construct##memberFunction##argNames##MemberFunctionTables();           \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void destroy##memberFunction##argNames##MemberFunctionTables();             \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  class add##memberFunction##argNames##GlobalMemberFunctionToTable            \
  {                                                                           \
  public:                                                                     \
                                                                              \
    add##memberFunction##argNames##GlobalMemberFunctionToTable                \
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
    ~add##memberFunction##argNames##GlobalMemberFunctionToTable()             \
    {                                                                         \
      destroy##memberFunction##argNames##MemberFunctionTables();              \
    }                                                                         \
  }


// internal use:
// constructor/destructor aid
#define DEFINE_GLOBAL_FUNCTION_SELECTION_TABLE_CONSTRUCT_DESTRUCT\
(memberFunction,argNames)                                                     \
                                                                              \
  /* Table constructor called from the table add function */                  \
  void construct##memberFunction##argNames##MemberFunctionTables()            \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      memberFunction##argNames##MemberFunctionTablePtr_                       \
        = new memberFunction##argNames##MemberFunctionTable;                  \
    }                                                                         \
  }                                                                           \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void destroy##memberFunction##argNames##MemberFunctionTables()              \
  {                                                                           \
    if (memberFunction##argNames##MemberFunctionTablePtr_)                    \
    {                                                                         \
      delete memberFunction##argNames##MemberFunctionTablePtr_;               \
      memberFunction##argNames##MemberFunctionTablePtr_ = nullptr;            \
    }                                                                         \
  }


// internal use:
// create pointer to hash-table of functions
#define DEFINE_GLOBAL_FUNCTION_SELECTION_TABLE_PTR\
(memberFunction,argNames)                                                     \
                                                                              \
  /* Define the memberFunction table */                                       \
  memberFunction##argNames##MemberFunctionTable*                              \
    memberFunction##argNames##MemberFunctionTablePtr_ = nullptr


// external use:
// ~~~~~~~~~~~~~
// define run-time selection table
#define DEFINE_GLOBAL_FUNCTION_SELECTION_TABLE\
(memberFunction,argNames)                                                     \
                                                                              \
  defineGlobalFunctionSelectionTablePtr                                       \
    (memberFunction,argNames);                                                \
  defineGlobalFunctionSelectionTableConstructDestruct                         \
    (memberFunction,argNames)

#endif
