#ifndef CORE_DB_RUN_TIME_SELECTION_ADD_TO_RUN_TIME_SELECTION_TABLE_HPP_
#define CORE_DB_RUN_TIME_SELECTION_ADD_TO_RUN_TIME_SELECTION_TABLE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


// add to hash-table of functions with typename as the key
#define ADD_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,argNames)                                                  \
                                                                              \
  /* Add the thisType constructor function to the table */                    \
  baseType::add##argNames##ConstructorToTable< thisType >                     \
    add##thisType##argNames##ConstructorTo##baseType##Table_

// add to hash-table of functions with 'lookup' as the key
#define ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,argNames,lookup)                                           \
                                                                              \
  /* Add the thisType constructor function to the table, find by lookup */    \
  baseType::add##argNames##ConstructorToTable< thisType >                     \
    add_##lookup##_##thisType##argNames##ConstructorTo##baseType##Table_      \
    (#lookup)

// add to hash-table of functions with typename as the key
#define ADD_REMOVABLE_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,argNames)                                                  \
                                                                              \
  /* Add the thisType constructor function to the table */                    \
  baseType::addRemovable##argNames##ConstructorToTable< thisType >            \
    addRemovable##thisType##argNames##ConstructorTo##baseType##Table_

// add to hash-table of functions with 'lookup' as the key
#define ADD_REMOVABLE_NAMED_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,argNames,lookup)                                           \
                                                                              \
  /* Add the thisType constructor function to the table, find by lookup */    \
  baseType::addRemovable##argNames##ConstructorToTable< thisType >            \
    addRemovable_##lookup##_##thisType##argNames##ConstructorTo               \
    ##baseType##Table_(#lookup)

// add to hash-table of functions with typename as the key
// use when baseType doesn't need a template argument (eg, is a typedef)
#define ADD_TEMPLATE_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,Targ,argNames)                                             \
                                                                              \
  /* Add the thisType constructor function to the table */                    \
  baseType::add##argNames##ConstructorToTable< thisType< Targ > >             \
    add##thisType##Targ##argNames##ConstructorTo##baseType##Table_

// add to hash-table of functions with 'lookup' as the key
// use when baseType doesn't need a template argument (eg, is a typedef)
#define ADD_NAMED_TEMPLATE_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,Targ,argNames,lookup)                                      \
                                                                              \
  /* Add the thisType constructor function to the table, find by lookup */    \
  baseType::add##argNames##ConstructorToTable< thisType< Targ > >             \
    add_##lookup##_##thisType##Targ##argNames##ConstructorTo##baseType        \
    ##Table_(#lookup)

// add to hash-table of functions with typename as the key
// use when baseType requires the Targ template argument as well
#define ADD_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,Targ,argNames)                                             \
                                                                              \
  /* Add the thisType constructor function to the table */                    \
  baseType< Targ >::add##argNames##ConstructorToTable< thisType< Targ > >     \
    add##thisType##Targ##argNames##ConstructorTo##baseType##Targ##Table_

// add to hash-table of functions with 'lookup' as the key
// use when baseType requires the Targ template argument as well
#define ADD_NAMED_TEMPLATED_TO_RUN_TIME_SELECTION_TABLE\
(baseType,thisType,Targ,argNames,lookup)                                      \
                                                                              \
  /* Add the thisType constructor function to the table, find by lookup */    \
  baseType< Targ >::add##argNames##ConstructorToTable< thisType< Targ > >     \
    add_##lookup##_##thisType##Targ##argNames##ConstructorTo##baseType##      \
    Targ##Table_(#lookup)

#endif
