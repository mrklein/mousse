#ifndef CORE_PRIMITIVES_OPS_OPS_HPP_
#define CORE_PRIMITIVES_OPS_OPS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse {

#define EQ_OP(opName, op)                                                     \
                                                                              \
template<class T1, class T2>                                                  \
class opName##Op2                                                             \
{                                                                             \
public:                                                                       \
                                                                              \
  void operator()(T1& x, const T2& y) const                                   \
  {                                                                           \
    op;                                                                       \
  }                                                                           \
};                                                                            \
                                                                              \
template<class T>                                                             \
class opName##Op                                                              \
{                                                                             \
public:                                                                       \
                                                                              \
  void operator()(T& x, const T& y) const                                     \
  {                                                                           \
    op;                                                                       \
  }                                                                           \
}


EQ_OP(eq, x = y);
EQ_OP(plusEq, x += y);
EQ_OP(minusEq, x -= y);
EQ_OP(multiplyEq, x *= y);
EQ_OP(divideEq, x /= y);
EQ_OP(eqMag, x = mag(y));
EQ_OP(plusEqMagSqr, x += magSqr(y));
EQ_OP(maxEq, x = max(x, y));
EQ_OP(minEq, x = min(x, y));
EQ_OP(minMagSqrEq, x = (magSqr(x)<=magSqr(y) ? x : y));
EQ_OP(maxMagSqrEq, x = (magSqr(x)>=magSqr(y) ? x : y));
EQ_OP(andEq, x = (x && y));
EQ_OP(orEq, x  = (x || y));
EQ_OP(eqMinus, x = -y);
EQ_OP(nopEq, (void)(x + y));

#undef EQ_OP

#if __GNUC__
#define WARNRETURN __attribute__((warn_unused_result))
#else
#define WARNRETURN
#endif


#define OP(opName, op)                                                        \
                                                                              \
  template<class T, class T1, class T2>                                       \
  class opName##Op3                                                           \
  {                                                                           \
  public:                                                                     \
                                                                              \
    T operator()(const T1& x, const T2& y) const WARNRETURN                   \
    {                                                                         \
      return op;                                                              \
    }                                                                         \
  };                                                                          \
                                                                              \
  template<class T1, class T2>                                                \
  class opName##Op2                                                           \
  {                                                                           \
  public:                                                                     \
                                                                              \
    T1 operator()(const T1& x, const T2& y) const WARNRETURN                  \
    {                                                                         \
      return op;                                                              \
    }                                                                         \
  };                                                                          \
                                                                              \
  template<class T>                                                           \
  class opName##Op                                                            \
  {                                                                           \
  public:                                                                     \
                                                                              \
    T operator()(const T& x, const T& y) const WARNRETURN                     \
    {                                                                         \
      return op;                                                              \
    }                                                                         \
  };

#define WEIGHTED_OP(opName, op)                                               \
                                                                              \
  template<class Type, class CombineOp>                                       \
  class opName##WeightedOp                                                    \
  {                                                                           \
    const CombineOp& cop_;                                                    \
                                                                              \
    public:                                                                   \
                                                                              \
      opName##WeightedOp(const CombineOp& cop)                                \
      :                                                                       \
        cop_(cop)                                                             \
      {}                                                                      \
                                                                              \
      void operator()                                                         \
      (                                                                       \
        Type& x,                                                              \
        const label index,                                                    \
        const Type& y,                                                        \
        const scalar weight                                                   \
      ) const                                                                 \
      {                                                                       \
        (void)index;                                                          \
        cop_(x, op);                                                          \
      }                                                                       \
  };                                                                          \


OP(sum, x + y)
OP(plus, x + y)
OP(minus, x - y)
OP(multiply, x * y)
OP(divide, x / y)
OP(cmptMultiply, cmptMultiply(x, y))
OP(cmptPow, cmptPow(x, y))
OP(cmptDivide, cmptDivide(x, y))
OP(stabilise, stabilise(x, y))
OP(max, max(x, y))
OP(min, min(x, y))
OP(minMagSqr, (magSqr(x)<=magSqr(y) ? x : y))
OP(maxMagSqr, (magSqr(x)>=magSqr(y) ? x : y))
OP(minMod, minMod(x, y))
OP(and, x && y)
OP(or, x || y)
OP(eqEq, x == y)
OP(less, x < y)
OP(lessEq, x <= y)
OP(greater, x > y)
OP(greaterEq, x >= y)
WEIGHTED_OP(multiply, (weight*y))

#undef OP
#undef WEIGHTED_OP
#undef WARNRETURN

}  // namespace mousse

#endif
