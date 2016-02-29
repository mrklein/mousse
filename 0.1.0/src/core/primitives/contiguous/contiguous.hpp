#ifndef CORE_PRIMITIVES_CONTIGUOUS_CONTIGUOUS_HPP_
#define CORE_PRIMITIVES_CONTIGUOUS_CONTIGUOUS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "int.hpp"

namespace mousse
{
// Forward declaration of friend functions and operators
template<class T, unsigned Size> class FixedList;
template<class T> class Pair;
//- Assume the data associated with type T are not contiguous
template<class T>
inline bool contiguous()                                   {return false;}
// Data associated with primitive types (and simple fixed size containers
//  - only size 2 defined here) are contiguous
template<>
inline bool contiguous<bool>()                              {return true;}
template<>
inline bool contiguous<FixedList<bool, 2> >()               {return true;}
template<>
inline bool contiguous<Pair<bool> >()                       {return true;}
template<>
inline bool contiguous<char>()                              {return true;}
template<>
inline bool contiguous<FixedList<char, 2> >()               {return true;}
template<>
inline bool contiguous<Pair<char> >()                       {return true;}
template<>
inline bool contiguous<int8_t>()                            {return true;}
template<>
inline bool contiguous<FixedList<int8_t, 2> >()             {return true;}
template<>
inline bool contiguous<Pair<int8_t> >()                     {return true;}
template<>
inline bool contiguous<uint8_t>()                           {return true;}
template<>
inline bool contiguous<FixedList<uint8_t, 2> >()            {return true;}
template<>
inline bool contiguous<Pair<uint8_t> >()                    {return true;}
template<>
inline bool contiguous<int16_t>()                           {return true;}
template<>
inline bool contiguous<FixedList<int16_t, 2> >()            {return true;}
template<>
inline bool contiguous<Pair<int16_t> >()                    {return true;}
template<>
inline bool contiguous<uint16_t>()                          {return true;}
template<>
inline bool contiguous<FixedList<uint16_t, 2> >()           {return true;}
template<>
inline bool contiguous<Pair<uint16_t> >()                   {return true;}
template<>
inline bool contiguous<int32_t>()                           {return true;}
template<>
inline bool contiguous<FixedList<int32_t, 2> >()            {return true;}
template<>
inline bool contiguous<Pair<int32_t> >()                    {return true;}
template<>
inline bool contiguous<uint32_t>()                          {return true;}
template<>
inline bool contiguous<FixedList<uint32_t, 2> >()           {return true;}
template<>
inline bool contiguous<Pair<uint32_t> >()                   {return true;}
template<>
inline bool contiguous<int64_t>()                           {return true;}
template<>
inline bool contiguous<FixedList<int64_t, 2> >()            {return true;}
template<>
inline bool contiguous<Pair<int64_t> >()                    {return true;}
template<>
inline bool contiguous<uint64_t>()                          {return true;}
template<>
inline bool contiguous<FixedList<uint64_t, 2> >()           {return true;}
template<>
inline bool contiguous<Pair<uint64_t> >()                   {return true;}
template<>
inline bool contiguous<float>()                             {return true;}
template<>
inline bool contiguous<FixedList<float, 2> >()              {return true;}
template<>
inline bool contiguous<Pair<float> >()                      {return true;}
template<>
inline bool contiguous<double>()                            {return true;}
template<>
inline bool contiguous<FixedList<double, 2> >()             {return true;}
template<>
inline bool contiguous<Pair<double> >()                     {return true;}
template<>
inline bool contiguous<long double>()                       {return true;}
template<>
inline bool contiguous<FixedList<long double, 2> >()        {return true;}
template<>
inline bool contiguous<Pair<long double> >()                {return true;}
}  // namespace mousse
#endif
