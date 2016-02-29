#ifndef CORE_CONTAINERS_LISTS_LIST_LOOP_M_HPP_
#define CORE_CONTAINERS_LISTS_LIST_LOOP_M_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#ifdef vectorMachine
// Element access looping using [] for vector machines
#define LIST_FOR_ALL(f, i) \
    const label _n##i = (f).size();\
    for (label i=0; i<_n##i; i++)  \
    {

#define LIST_END_FOR_ALL  }

// Provide current element
#define LIST_CELEM(f, fp, i)  (fp[i])

// Provide current element
#define LIST_ELEM(f, fp, i)  (fp[i])

#define LIST_ACCESS(type, f, fp) \
  type* const __restrict__ fp = (f).begin()

#define LIST_CONST_ACCESS(type, f, fp) \
  const type* const __restrict__ fp = (f).begin()

#else
// Pointer looping for scalar machines
#define LIST_FOR_ALL(f, i)                  \
    label i = (f).size();                   \
    while (i--)                             \
    {                                       \

#define LIST_END_FOR_ALL  }

// Provide current element without incrementing pointer
#define LIST_CELEM(f, fp, i)  (*fp)

// Provide current element and increment pointer
#define LIST_ELEM(f, fp, i)  (*fp++)

#define LIST_ACCESS(type, f, fp) \
  type* __restrict__ fp = (f).begin()

#define LIST_CONST_ACCESS(type, f, fp) \
  const type* __restrict__ fp = (f).begin()

#endif  // vectorMachine
#endif  // CORE_CONTAINERS_LISTS_LIST_LOOP_M_HPP_
