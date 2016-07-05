#ifndef CORE_PRIMITIVES_INTS_LABEL_HPP_
#define CORE_PRIMITIVES_INTS_LABEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "int.hpp"
#define INT_ADD_SIZE(x,s,y) x ## s ## y
#define INT_ADD_DEF_SIZE(x,s,y) INT_ADD_SIZE(x,s,y)
#define INT_SIZE(x,y) INT_ADD_DEF_SIZE(x,WM_LABEL_SIZE,y)
#if WM_LABEL_SIZE != 32 && WM_LABEL_SIZE != 64
  #error "label.hpp: WM_LABEL_SIZE must be set to either 32 or 64"
#endif


namespace mousse {

typedef INT_SIZE(int, _t) label;

static const label labelMin = INT_SIZE(INT, _MIN);

static const label labelMax = INT_SIZE(INT, _MAX);

inline label readLabel(Istream& is)
{
  return INT_SIZE(readInt,) (is);
}

//- Raise one label to the power of another
label pow(label a, label b);

//- Evaluate n! : 0 < n <= 12
label factorial(label n);

inline label& setComponent(label& l, const direction)
{
  return l;
}


inline label component(const label l, const direction)
{
  return l;
}


inline label sign(const label s)
{
  return (s >= 0)? 1: -1;
}


inline label pos(const label s)
{
  return (s >= 0)? 1: 0;
}


inline label neg(const label s)
{
  return (s < 0)? 1: 0;
}


inline label posPart(const label s)
{
  return (s > 0)? s: 0;
}


inline label negPart(const label s)
{
  return (s < 0)? s: 0;
}


}  // namespace mousse

#undef INT_ADD_SIZE
#undef INT_ADD_DEF_SIZE
#undef INT_SIZE

#endif
