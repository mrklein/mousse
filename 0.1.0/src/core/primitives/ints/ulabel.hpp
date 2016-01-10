// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef ulabel_hpp_
#define ulabel_hpp_
#include "uint.hpp"
#define UINT_ADD_SIZE(x,s,y) x ## s ## y
#define UINT_ADD_DEF_SIZE(x,s,y) UINT_ADD_SIZE(x,s,y)
#define UINT_SIZE(x,y) UINT_ADD_DEF_SIZE(x,WM_LABEL_SIZE,y)
#if WM_LABEL_SIZE != 32 && WM_LABEL_SIZE != 64
  #error "ulabel.hpp: WM_LABEL_SIZE must be set to either 32 or 64"
#endif
namespace mousse
{
typedef UINT_SIZE(uint, _t) uLabel;
static const uLabel uLabelMin = UINT_SIZE(UINT, _MIN);
static const uLabel uLabelMax = UINT_SIZE(UINT, _MAX);
inline uLabel readULabel(Istream& is)
{
  return UINT_SIZE(readUint,) (is);
}
//- Raise one uLabel to the power of another
uLabel pow(uLabel a, uLabel b);
//- Evaluate n! : 0 < n <= 12
uLabel factorial(uLabel n);
inline uLabel& setComponent(uLabel& l, const direction)
{
  return l;
}
inline uLabel component(const uLabel l, const direction)
{
  return l;
}
}  // namespace mousse
#include "ulabel_specific.hpp"
#undef UINT_ADD_SIZE
#undef UINT_ADD_DEF_SIZE
#undef UINT_SIZE
#endif
