// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symm_tensor_field.hpp"
#include "transform_field.hpp"
#define TEMPLATE
#include "field_functions_m.cpp"
namespace mousse
{
// global functions 
UNARY_FUNCTION(symmTensor, vector, sqr)
UNARY_FUNCTION(symmTensor, symmTensor, innerSqr)
UNARY_FUNCTION(scalar, symmTensor, tr)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph)
UNARY_FUNCTION(symmTensor, symmTensor, symm)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm)
UNARY_FUNCTION(symmTensor, symmTensor, dev)
UNARY_FUNCTION(symmTensor, symmTensor, dev2)
UNARY_FUNCTION(scalar, symmTensor, det)
UNARY_FUNCTION(symmTensor, symmTensor, cof)
void inv(Field<symmTensor>& tf, const UList<symmTensor>& tf1)
{
  if (tf.empty())
  {
    return;
  }
  scalar scale = magSqr(tf1[0]);
  Vector<bool> removeCmpts
  (
    magSqr(tf1[0].xx())/scale < SMALL,
    magSqr(tf1[0].yy())/scale < SMALL,
    magSqr(tf1[0].zz())/scale < SMALL
  );
  if (removeCmpts.x() || removeCmpts.y() || removeCmpts.z())
  {
    symmTensorField tf1Plus(tf1);
    if (removeCmpts.x())
    {
      tf1Plus += symmTensor(1,0,0,0,0,0);
    }
    if (removeCmpts.y())
    {
      tf1Plus += symmTensor(0,0,0,1,0,0);
    }
    if (removeCmpts.z())
    {
      tf1Plus += symmTensor(0,0,0,0,0,1);
    }
    TFOR_ALL_F_OP_FUNC_F(symmTensor, tf, =, inv, symmTensor, tf1Plus)
    if (removeCmpts.x())
    {
      tf -= symmTensor(1,0,0,0,0,0);
    }
    if (removeCmpts.y())
    {
      tf -= symmTensor(0,0,0,1,0,0);
    }
    if (removeCmpts.z())
    {
      tf -= symmTensor(0,0,0,0,0,1);
    }
  }
  else
  {
    TFOR_ALL_F_OP_FUNC_F(symmTensor, tf, =, inv, symmTensor, tf1)
  }
}
tmp<symmTensorField> inv(const UList<symmTensor>& tf)
{
  tmp<symmTensorField> result(new symmTensorField(tf.size()));
  inv(result(), tf);
  return result;
}
tmp<symmTensorField> inv(const tmp<symmTensorField>& tf)
{
  tmp<symmTensorField> tRes = reuseTmp<symmTensor, symmTensor>::New(tf);
  inv(tRes(), tf());
  reuseTmp<symmTensor, symmTensor>::clear(tf);
  return tRes;
}
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tensorField& tf
)
{
  return symm(tf);
}
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tmp<tensorField>& ttf
)
{
  tmp<Field<symmTensor> > ret = transformFieldMask<symmTensor>(ttf());
  ttf.clear();
  return ret;
}
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const symmTensorField& stf
)
{
  return stf;
}
template<>
tmp<Field<symmTensor> > transformFieldMask<symmTensor>
(
  const tmp<symmTensorField>& tstf
)
{
  return tstf;
}
// global operators 
UNARY_OPERATOR(vector, symmTensor, *, hdual)
BINARY_OPERATOR(tensor, symmTensor, symmTensor, &, dot)
BINARY_TYPE_OPERATOR(tensor, symmTensor, symmTensor, &, dot)
}  // namespace mousse
#include "undef_field_functions_m.hpp"
