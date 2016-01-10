// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::vectorTensorTransform::vectorTensorTransform()
:
  t_(vector::zero),
  R_(sphericalTensor::I),
  hasR_(false)
{}
inline mousse::vectorTensorTransform::vectorTensorTransform
(
  const vector& t,
  const tensor& R,
  bool hasR
)
:
  t_(t),
  R_(R),
  hasR_(hasR)
{}
inline mousse::vectorTensorTransform::vectorTensorTransform(const vector& t)
:
  t_(t),
  R_(sphericalTensor::I),
  hasR_(false)
{}
inline mousse::vectorTensorTransform::vectorTensorTransform(const tensor& R)
:
  t_(vector::zero),
  R_(R),
  hasR_(true)
{}
// Member Functions 
inline const mousse::vector& mousse::vectorTensorTransform::t() const
{
  return t_;
}
inline const mousse::tensor& mousse::vectorTensorTransform::R() const
{
  return R_;
}
inline bool mousse::vectorTensorTransform::hasR() const
{
  return hasR_;
}
inline mousse::vector& mousse::vectorTensorTransform::t()
{
  return t_;
}
inline mousse::tensor& mousse::vectorTensorTransform::R()
{
  // Assume that non-const access to R changes it from I, so set
  // hasR to true
  hasR_ = true;
  return R_;
}
inline mousse::vector mousse::vectorTensorTransform::transformPosition
(
  const vector& v
) const
{
  if (hasR_)
  {
    return t() + (R() & v);
  }
  else
  {
    return t() + v;
  }
}
inline mousse::pointField mousse::vectorTensorTransform::transformPosition
(
  const pointField& pts
) const
{
  tmp<pointField> tfld;
  if (hasR_)
  {
    tfld = t() + (R() & pts);
  }
  else
  {
    tfld = t() + pts;
  }
  return tfld();
}
inline mousse::vector mousse::vectorTensorTransform::invTransformPosition
(
  const vector& v
) const
{
  if (hasR_)
  {
    return (R().T() & (v - t()));
  }
  else
  {
    return v - t();
  }
}
inline mousse::pointField mousse::vectorTensorTransform::invTransformPosition
(
  const pointField& pts
) const
{
  tmp<pointField> tfld;
  if (hasR_)
  {
    tfld = (R().T() & (pts - t()));
  }
  else
  {
    tfld = pts - t();
  }
  return tfld();
}
// Member Operators 
inline void mousse::vectorTensorTransform::operator=
(
  const vectorTensorTransform& tr
)
{
  t_ = tr.t_;
  R_ = tr.R_;
  hasR_ = tr.hasR_;
}
inline void mousse::vectorTensorTransform::operator&=
(
  const vectorTensorTransform& tr
)
{
  t_ += tr.t_;
  R_ = tr.R_ & R_;
  // If either of the two objects has hasR_ as true, then inherit
  // it, otherwise, these should both be I tensors.
  hasR_ = tr.hasR_ || hasR_;
}
inline void mousse::vectorTensorTransform::operator=(const vector& t)
{
  t_ = t;
}
inline void mousse::vectorTensorTransform::operator+=(const vector& t)
{
  t_ += t;
}
inline void mousse::vectorTensorTransform::operator-=(const vector& t)
{
  t_ -= t;
}
inline void mousse::vectorTensorTransform::operator=(const tensor& R)
{
  hasR_ = true;
  R_ = R;
}
inline void mousse::vectorTensorTransform::operator&=(const tensor& R)
{
  hasR_ = true;
  R_ = R & R_;
}
// Global Functions 
inline mousse::vectorTensorTransform mousse::inv(const vectorTensorTransform& tr)
{
  return vectorTensorTransform(-tr.t(), tr.R().T(), tr.hasR());
}
// Global Operators 
inline bool mousse::operator==
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
)
{
  return (tr1.t() == tr2.t() && tr1.R() == tr2.R());
}
inline bool mousse::operator!=
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
)
{
  return !operator==(tr1, tr2);
}
inline mousse::vectorTensorTransform mousse::operator+
(
  const vectorTensorTransform& tr,
  const vector& t
)
{
  return vectorTensorTransform(tr.t() + t, tr.R(), tr.hasR());
}
inline mousse::vectorTensorTransform mousse::operator+
(
  const vector& t,
  const vectorTensorTransform& tr
)
{
  return vectorTensorTransform(t + tr.t(), tr.R(), tr.hasR());
}
inline mousse::vectorTensorTransform mousse::operator-
(
  const vectorTensorTransform& tr,
  const vector& t
)
{
  return vectorTensorTransform(tr.t() - t, tr.R(), tr.hasR());
}
inline mousse::vectorTensorTransform mousse::operator&
(
  const vectorTensorTransform& tr1,
  const vectorTensorTransform& tr2
)
{
  return vectorTensorTransform
  (
    tr1.t() + tr2.t(),
    tr1.R() & tr2.R(),
    (tr1.hasR() || tr2.hasR())
  );
}
