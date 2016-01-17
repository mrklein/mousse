// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::memberFunctionSelectionTables
// Description
//   Macros to enable the easy declaration of member function selection tables.

#include "token.hpp"

#ifndef member_function_selection_tables_hpp_
#define member_function_selection_tables_hpp_

#include "hash_table.hpp"

// external use:
// ~~~~~~~~~~~~~
// declare a run-time selection:
#define DECLARE_MEMBER_FUNCTION_SELECTION_TABLE\
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
  /* Class to add constructor from argList to table */                        \
  template<class baseType##Type>                                              \
  class add##memberFunction##argNames##MemberFunctionToTable                  \
  {                                                                           \
  public:                                                                     \
                                                                              \
    add##memberFunction##argNames##MemberFunctionToTable                      \
    (                                                                         \
      const word& lookup = baseType##Type::typeName                           \
    )                                                                         \
    {                                                                         \
      construct##memberFunction##argNames##MemberFunctionTables();            \
      memberFunction##argNames##MemberFunctionTablePtr_->insert               \
      (                                                                       \
        lookup,                                                               \
        baseType##Type::memberFunction                                        \
      );                                                                      \
    }                                                                         \
                                                                              \
    ~add##memberFunction##argNames##MemberFunctionToTable()                   \
    {                                                                         \
      destroy##memberFunction##argNames##MemberFunctionTables();              \
    }                                                                         \
  };                                                                          \
                                                                              \
  /* Table memberFunction called from the table add function */               \
  static void construct##memberFunction##argNames##MemberFunctionTables();    \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  static void destroy##memberFunction##argNames##MemberFunctionTables()

// internal use:
// constructor aid
#define DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_MEMBER_FUNCTION\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Table memberFunction called from the table add function */               \
  void baseType::construct##memberFunction##argNames##MemberFunctionTables()  \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      baseType::memberFunction##argNames##MemberFunctionTablePtr_             \
        = new baseType::memberFunction##argNames##MemberFunctionTable;        \
    }                                                                         \
  }

// internal use:
// destructor aid
#define DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_DESTRUCTOR\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void baseType::destroy##memberFunction##argNames##MemberFunctionTables()    \
  {                                                                           \
    if (baseType::memberFunction##argNames##MemberFunctionTablePtr_)          \
    {                                                                         \
      delete baseType::memberFunction##argNames##MemberFunctionTablePtr_;     \
      baseType::memberFunction##argNames##MemberFunctionTablePtr_ = NULL;     \
    }                                                                         \
  }

// internal use:
// create pointer to hash-table of functions
#define DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_PTR\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Define the memberFunction table */                                       \
  baseType::memberFunction##argNames##MemberFunctionTable*                    \
    baseType::memberFunction##argNames##MemberFunctionTablePtr_ = NULL

// not much in use:
#define DEFINE_TEMPLATE_MEMBER_FUNCTION_SELECTION_TABLE_PTR\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  /* Define the memberFunction table */                                       \
  typename baseType::memberFunction##argNames##MemberFunctionTable*           \
    baseType::memberFunction##argNames##MemberFunctionTablePtr_ = NULL

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table
#define DEFINE_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_PTR                                  \
    (baseType,memberFunction,argNames);                                       \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_MEMBER_FUNCTION                      \
    (baseType,memberFunction,argNames)                                        \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_DESTRUCTOR                           \
    (baseType,memberFunction,argNames)

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table for template classes
// use when baseType doesn't need a template argument (eg, is a typedef)
#define DEFINE_TEMPLATE_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,memberFunction,argNames)                                            \
                                                                              \
  template<>                                                                  \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_PTR                                  \
    (baseType,memberFunction,argNames);                                       \
  template<>                                                                  \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_MEMBER_FUNCTION                      \
    (baseType,memberFunction,argNames)                                        \
  template<>                                                                  \
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE_DESTRUCTOR                           \
    (baseType,memberFunction,argNames)

// internal use:
// constructor aid
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_MEMBER_FUNCTION\
(baseType,memberFunction,argNames,Targ)                                       \
                                                                              \
  /* Table memberFunction called from the table add function */               \
  void baseType<Targ>::construct##memberFunction##argNames##                  \
  MemberFunctionTables()                                                      \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      baseType<Targ>::memberFunction##argNames##MemberFunctionTablePtr_       \
        = new baseType<Targ>::memberFunction##argNames##                      \
         MemberFunctionTable;                                                 \
    }                                                                         \
  }

// internal use:
// destructor aid
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_DESTRUCTOR\
(baseType,memberFunction,argNames,Targ)                                       \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void baseType<Targ>::destroy##memberFunction##argNames##                    \
  MemberFunctionTables()                                                      \
  {                                                                           \
    if                                                                        \
    (                                                                         \
      baseType<Targ>::memberFunction##argNames##MemberFunctionTablePtr_       \
    )                                                                         \
    {                                                                         \
      delete baseType<Targ>::memberFunction##argNames##                       \
        MemberFunctionTablePtr_;                                              \
      baseType<Targ>::memberFunction##argNames##                              \
        MemberFunctionTablePtr_ = NULL;                                       \
    }                                                                         \
  }

// internal use:
// create pointer to hash-table of functions
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_PTR\
(baseType,memberFunction,argNames,Targ)                                       \
                                                                              \
  /* Define the memberFunction table */                                       \
  baseType<Targ>::memberFunction##argNames##MemberFunctionTable*              \
    baseType<Targ>::memberFunction##argNames##MemberFunctionTablePtr_ = NULL

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table for template classes
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE\
(baseType,memberFunction,argNames,Targ)                                       \
                                                                              \
  template<>                                                                  \
  DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_PTR                        \
    (baseType,memberFunction,argNames,Targ);                                  \
  template<>                                                                  \
  DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_MEMBER_FUNCTION            \
    (baseType,memberFunction,argNames,Targ)                                   \
  template<>                                                                  \
  DEFINE_TEMPLATED_MEMBER_FUNCTION_SELECTION_TABLE_DESTRUCTOR                 \
    (baseType,memberFunction,argNames,Targ)

#endif
