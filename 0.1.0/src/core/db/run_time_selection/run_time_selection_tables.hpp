// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "token.hpp"
#ifndef run_time_selection_tables_hpp_
#define run_time_selection_tables_hpp_
#include "auto_ptr.hpp"
#include "hash_table.hpp"
#include "error.hpp"

// external use:
// ~~~~~~~~~~~~~
// declare a run-time selection:
#define DECLARE_RUN_TIME_SELECTION_TABLE\
(autoPtr,baseType,argNames,argList,parList)                                   \
                                                                              \
  /* Construct from argList function pointer type */                          \
  typedef autoPtr< baseType > (*argNames##ConstructorPtr)argList;             \
                                                                              \
  /* Construct from argList function table type */                            \
  typedef HashTable< argNames##ConstructorPtr, word, string::hash >           \
    argNames##ConstructorTable;                                               \
                                                                              \
  /* Construct from argList function pointer table pointer */                 \
  static argNames##ConstructorTable* argNames##ConstructorTablePtr_;          \
                                                                              \
  /* Table constructor called from the table add function */                  \
  static void construct##argNames##ConstructorTables();                       \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  static void destroy##argNames##ConstructorTables();                         \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  template< class baseType##Type >                                            \
  class add##argNames##ConstructorToTable                                     \
  {                                                                           \
  public:                                                                     \
                                                                              \
    static autoPtr< baseType > New argList                                    \
    {                                                                         \
      return autoPtr< baseType >(new baseType##Type parList);                 \
    }                                                                         \
                                                                              \
    add##argNames##ConstructorToTable                                         \
    (                                                                         \
      const word& lookup = baseType##Type::typeName                           \
    )                                                                         \
    {                                                                         \
      construct##argNames##ConstructorTables();                               \
      if (!argNames##ConstructorTablePtr_->insert(lookup, New))               \
      {                                                                       \
        std::cerr<< "Duplicate entry " << lookup                              \
          << " in runtime selection table " << #baseType                      \
          << std::endl;                                                       \
        error::safePrintStack(std::cerr);                                     \
      }                                                                       \
    }                                                                         \
                                                                              \
    ~add##argNames##ConstructorToTable()                                      \
    {                                                                         \
      destroy##argNames##ConstructorTables();                                 \
    }                                                                         \
  };                                                                          \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  /* Remove only the entry (not the table) upon destruction */                \
  template< class baseType##Type >                                            \
  class addRemovable##argNames##ConstructorToTable                            \
  {                                                                           \
    /* retain lookup name for later removal */                                \
    const word& lookup_;                                                      \
                                                                              \
  public:                                                                     \
                                                                              \
    static autoPtr< baseType > New argList                                    \
    {                                                                         \
      return autoPtr< baseType >(new baseType##Type parList);                 \
    }                                                                         \
                                                                              \
    addRemovable##argNames##ConstructorToTable                                \
    (                                                                         \
      const word& lookup = baseType##Type::typeName                           \
    )                                                                         \
    :                                                                         \
      lookup_(lookup)                                                         \
    {                                                                         \
      construct##argNames##ConstructorTables();                               \
      argNames##ConstructorTablePtr_->set(lookup, New);                       \
    }                                                                         \
                                                                              \
    ~addRemovable##argNames##ConstructorToTable()                             \
    {                                                                         \
      if (argNames##ConstructorTablePtr_)                                     \
      {                                                                       \
        argNames##ConstructorTablePtr_->erase(lookup_);                       \
      }                                                                       \
    }                                                                         \
  };

// external use:
// ~~~~~~~~~~~~~
// declare a run-time selection for derived classes:
#define DECLARE_RUN_TIME_NEW_SELECTION_TABLE\
(autoPtr,baseType,argNames,argList,parList)                                   \
                                                                              \
  /* Construct from argList function pointer type */                          \
  typedef autoPtr< baseType > (*argNames##ConstructorPtr)argList;             \
                                                                              \
  /* Construct from argList function table type */                            \
  typedef HashTable< argNames##ConstructorPtr, word, string::hash >           \
    argNames##ConstructorTable;                                               \
                                                                              \
  /* Construct from argList function pointer table pointer */                 \
  static argNames##ConstructorTable* argNames##ConstructorTablePtr_;          \
                                                                              \
  /* Table constructor called from the table add function */                  \
  static void construct##argNames##ConstructorTables();                       \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  static void destroy##argNames##ConstructorTables();                         \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  template< class baseType##Type >                                            \
  class add##argNames##ConstructorToTable                                     \
  {                                                                           \
  public:                                                                     \
                                                                              \
    static autoPtr< baseType > New##baseType argList                          \
    {                                                                         \
      return autoPtr< baseType >(baseType##Type::New parList.ptr());          \
    }                                                                         \
                                                                              \
    add##argNames##ConstructorToTable                                         \
    (                                                                         \
      const word& lookup = baseType##Type::typeName                           \
    )                                                                         \
    {                                                                         \
      construct##argNames##ConstructorTables();                               \
      if                                                                      \
      (                                                                       \
       !argNames##ConstructorTablePtr_->insert                                \
        (                                                                     \
          lookup,                                                             \
          New##baseType                                                       \
        )                                                                     \
      )                                                                       \
      {                                                                       \
        std::cerr<< "Duplicate entry " << lookup                              \
          << " in runtime selection table " << #baseType                      \
          << std::endl;                                                       \
        error::safePrintStack(std::cerr);                                     \
      }                                                                       \
    }                                                                         \
                                                                              \
    ~add##argNames##ConstructorToTable()                                      \
    {                                                                         \
      destroy##argNames##ConstructorTables();                                 \
    }                                                                         \
  };                                                                          \
                                                                              \
  /* Class to add constructor from argList to table */                        \
  template< class baseType##Type >                                            \
  class addRemovable##argNames##ConstructorToTable                            \
  {                                                                           \
    /* retain lookup name for later removal */                                \
    const word& lookup_;                                                      \
                                                                              \
  public:                                                                     \
                                                                              \
    static autoPtr< baseType > New##baseType argList                          \
    {                                                                         \
      return autoPtr< baseType >(baseType##Type::New parList.ptr());          \
    }                                                                         \
                                                                              \
    addRemovable##argNames##ConstructorToTable                                \
    (                                                                         \
      const word& lookup = baseType##Type::typeName                           \
    )                                                                         \
    :                                                                         \
      lookup_(lookup)                                                         \
    {                                                                         \
      construct##argNames##ConstructorTables();                               \
      argNames##ConstructorTablePtr_->set                                     \
      (                                                                       \
        lookup,                                                               \
        New##baseType                                                         \
      );                                                                      \
    }                                                                         \
                                                                              \
    ~addRemovable##argNames##ConstructorToTable()                             \
    {                                                                         \
      if (argNames##ConstructorTablePtr_)                                     \
      {                                                                       \
        argNames##ConstructorTablePtr_->erase(lookup_);                       \
      }                                                                       \
    }                                                                         \
  };

// internal use:
// constructor aid
#define DEFINE_RUN_TIME_SELECTION_TABLE_CONSTRUCTOR(baseType,argNames)        \
                                                                              \
  /* Table constructor called from the table add function */                  \
  void baseType::construct##argNames##ConstructorTables()                     \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      baseType::argNames##ConstructorTablePtr_                                \
        = new baseType::argNames##ConstructorTable;                           \
    }                                                                         \
  }

// internal use:
// destructor aid
#define DEFINE_RUN_TIME_SELECTION_TABLE_DESTRUCTOR(baseType,argNames)         \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void baseType::destroy##argNames##ConstructorTables()                       \
  {                                                                           \
    if (baseType::argNames##ConstructorTablePtr_)                             \
    {                                                                         \
      delete baseType::argNames##ConstructorTablePtr_;                        \
      baseType::argNames##ConstructorTablePtr_ = NULL;                        \
    }                                                                         \
  }

// internal use:
// create pointer to hash-table of functions
#define DEFINE_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames)                \
                                                                              \
  /* Define the constructor function table */                                 \
  baseType::argNames##ConstructorTable*                                       \
    baseType::argNames##ConstructorTablePtr_ = NULL

// not much in use:
#define DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames)       \
                                                                              \
  /* Define the constructor function table */                                 \
  typename baseType::argNames##ConstructorTable*                              \
    baseType::argNames##ConstructorTablePtr_ = NULL

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table
#define DEFINE_RUN_TIME_SELECTION_TABLE(baseType,argNames)                    \
                                                                              \
  DEFINE_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames);                     \
  DEFINE_RUN_TIME_SELECTION_TABLE_CONSTRUCTOR(baseType,argNames);             \
  DEFINE_RUN_TIME_SELECTION_TABLE_DESTRUCTOR(baseType,argNames)

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table for template classes
// use when baseType doesn't need a template argument (eg, is a typedef)
#define DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(baseType,argNames)           \
                                                                              \
  template<>                                                                  \
  DEFINE_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames);                     \
  template<>                                                                  \
  DEFINE_RUN_TIME_SELECTION_TABLE_CONSTRUCTOR(baseType,argNames);             \
  template<>                                                                  \
  DEFINE_RUN_TIME_SELECTION_TABLE_DESTRUCTOR(baseType,argNames)

// internal use:
// constructor aid
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_CONSTRUCTOR\
(baseType,argNames,Targ)                                                      \
                                                                              \
  /* Table constructor called from the table add function */                  \
  void baseType< Targ >::construct##argNames##ConstructorTables()             \
  {                                                                           \
    static bool constructed = false;                                          \
    if (!constructed)                                                         \
    {                                                                         \
      constructed = true;                                                     \
      baseType< Targ >::argNames##ConstructorTablePtr_                        \
        = new baseType< Targ >::argNames##ConstructorTable;                   \
    }                                                                         \
  }

// internal use:
// destructor aid
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_DESTRUCTOR\
(baseType,argNames,Targ)                                                      \
                                                                              \
  /* Table destructor called from the table add function destructor */        \
  void baseType< Targ >::destroy##argNames##ConstructorTables()               \
  {                                                                           \
    if (baseType< Targ >::argNames##ConstructorTablePtr_)                     \
    {                                                                         \
      delete baseType< Targ >::argNames##ConstructorTablePtr_;                \
      baseType< Targ >::argNames##ConstructorTablePtr_ = NULL;                \
    }                                                                         \
  }

// internal use:
// create pointer to hash-table of functions
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames,Targ) \
                                                                              \
  /* Define the constructor function table */                                 \
  baseType< Targ >::argNames##ConstructorTable*                               \
    baseType< Targ >::argNames##ConstructorTablePtr_ = NULL

// external use:
// ~~~~~~~~~~~~~
// define run-time selection table for template classes
// use when baseType requires the Targ template argument
#define DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE(baseType,argNames,Targ)     \
                                                                              \
  template<>                                                                  \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_PTR(baseType,argNames,Targ);      \
  template<>                                                                  \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_CONSTRUCTOR                       \
  (                                                                           \
    baseType,                                                                 \
    argNames,                                                                 \
    Targ                                                                      \
  );                                                                          \
  template<>                                                                  \
  DEFINE_TEMPLATED_RUN_TIME_SELECTION_TABLE_DESTRUCTOR(baseType,argNames,Targ)

#endif
