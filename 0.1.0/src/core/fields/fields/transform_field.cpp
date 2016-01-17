// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform_field.hpp"
#include "field_m.hpp"
#include "diag_tensor.hpp"
#include "field_reuse_functions.hpp"

// global functions
void mousse::transform
(
  vectorField& rtf,
  const quaternion& q,
  const vectorField& tf
)
{
  tensor t = q.R();
  TFOR_ALL_F_OP_FUNC_S_F(vector, rtf, =, transform, tensor, t, vector, tf)
}
mousse::tmp<mousse::vectorField> mousse::transform
(
  const quaternion& q,
  const vectorField& tf
)
{
  tmp<vectorField > tranf(new vectorField(tf.size()));
  transform(tranf(), q, tf);
  return tranf;
}
mousse::tmp<mousse::vectorField> mousse::transform
(
  const quaternion& q,
  const tmp<vectorField>& ttf
)
{
  tmp<vectorField > tranf = reuseTmp<vector, vector>::New(ttf);
  transform(tranf(), q, ttf());
  reuseTmp<vector, vector>::clear(ttf);
  return tranf;
}
void mousse::transform
(
  vectorField& rtf,
  const septernion& tr,
  const vectorField& tf
)
{
  vector T = tr.t();
  // Check if any rotation
  if (mag(tr.r().R() - I) > SMALL)
  {
    transform(rtf, tr.r(), tf);
    if (mag(T) > VSMALL)
    {
      rtf += T;
    }
  }
  else
  {
    if (mag(T) > VSMALL)
    {
      TFOR_ALL_F_OP_S_OP_F(vector, rtf, =, vector, T, +, vector, tf);
    }
    else
    {
      rtf = tf;
    }
  }
}
mousse::tmp<mousse::vectorField> mousse::transform
(
  const septernion& tr,
  const vectorField& tf
)
{
  tmp<vectorField > tranf(new vectorField(tf.size()));
  transform(tranf(), tr, tf);
  return tranf;
}
mousse::tmp<mousse::vectorField> mousse::transform
(
  const septernion& tr,
  const tmp<vectorField>& ttf
)
{
  tmp<vectorField > tranf = reuseTmp<vector, vector>::New(ttf);
  transform(tranf(), tr, ttf());
  reuseTmp<vector, vector>::clear(ttf);
  return tranf;
}
