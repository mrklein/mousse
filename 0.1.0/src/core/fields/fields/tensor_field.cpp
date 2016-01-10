// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tensor_field.hpp"
#include "transform_field.hpp"
#define TEMPLATE
#include "field_functions_m.cpp"
namespace mousse
{
// global functions 
UNARY_FUNCTION(scalar, tensor, tr)
UNARY_FUNCTION(sphericalTensor, tensor, sph)
UNARY_FUNCTION(symmTensor, tensor, symm)
UNARY_FUNCTION(symmTensor, tensor, twoSymm)
UNARY_FUNCTION(tensor, tensor, skew)
UNARY_FUNCTION(tensor, tensor, dev)
UNARY_FUNCTION(tensor, tensor, dev2)
UNARY_FUNCTION(scalar, tensor, det)
UNARY_FUNCTION(tensor, tensor, cof)
void inv(Field<tensor>& tf, const UList<tensor>& tf1)
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
    tensorField tf1Plus(tf1);
    if (removeCmpts.x())
    {
      tf1Plus += tensor(1,0,0,0,0,0,0,0,0);
    }
    if (removeCmpts.y())
    {
      tf1Plus += tensor(0,0,0,0,1,0,0,0,0);
    }
    if (removeCmpts.z())
    {
      tf1Plus += tensor(0,0,0,0,0,0,0,0,1);
    }
    TFOR_ALL_F_OP_FUNC_F(tensor, tf, =, inv, tensor, tf1Plus)
    if (removeCmpts.x())
    {
      tf -= tensor(1,0,0,0,0,0,0,0,0);
    }
    if (removeCmpts.y())
    {
      tf -= tensor(0,0,0,0,1,0,0,0,0);
    }
    if (removeCmpts.z())
    {
      tf -= tensor(0,0,0,0,0,0,0,0,1);
    }
  }
  else
  {
    TFOR_ALL_F_OP_FUNC_F(tensor, tf, =, inv, tensor, tf1)
  }
}
tmp<tensorField> inv(const UList<tensor>& tf)
{
  tmp<tensorField> result(new tensorField(tf.size()));
  inv(result(), tf);
  return result;
}
tmp<tensorField> inv(const tmp<tensorField>& tf)
{
  tmp<tensorField> tRes = reuseTmp<tensor, tensor>::New(tf);
  inv(tRes(), tf());
  reuseTmp<tensor, tensor>::clear(tf);
  return tRes;
}
UNARY_FUNCTION(vector, tensor, eigenValues)
UNARY_FUNCTION(tensor, tensor, eigenVectors)
UNARY_FUNCTION(vector, symmTensor, eigenValues)
UNARY_FUNCTION(tensor, symmTensor, eigenVectors)
template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
  const symmTensorField& stf
)
{
  tmp<tensorField> tRes(new tensorField(stf.size()));
  tensorField& res = tRes();
  TFOR_ALL_F_OP_F(tensor, res, =, symmTensor, stf)
  return tRes;
}
template<>
tmp<Field<tensor> > transformFieldMask<tensor>
(
  const tmp<symmTensorField>& tstf
)
{
  tmp<Field<tensor> > ret = transformFieldMask<tensor>(tstf());
  tstf.clear();
  return ret;
}
// global operators 
UNARY_OPERATOR(vector, tensor, *, hdual)
UNARY_OPERATOR(tensor, vector, *, hdual)
BINARY_OPERATOR(vector, vector, tensor, /, divide)
BINARY_TYPE_OPERATOR(vector, vector, tensor, /, divide)
}  // namespace mousse
#include "undef_field_functions_m.hpp"
