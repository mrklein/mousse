// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
inline mousse::forceSuSp::forceSuSp()
{}
inline mousse::forceSuSp::forceSuSp
(
  const Tuple2<vector, scalar>& fs
)
:
  Tuple2<vector, scalar>(fs)
{}
inline mousse::forceSuSp::forceSuSp(const vector& Su, const scalar Sp)
{
  first() = Su;
  second() = Sp;
}
inline mousse::forceSuSp::forceSuSp(Istream& is)
:
  Tuple2<vector, scalar>(is)
{}
// Member Functions 
inline const mousse::vector& mousse::forceSuSp::Su() const
{
  return first();
}
inline mousse::scalar mousse::forceSuSp::Sp() const
{
  return second();
}
inline mousse::vector& mousse::forceSuSp::Su()
{
  return first();
}
inline mousse::scalar& mousse::forceSuSp::Sp()
{
  return second();
}
// Operators
inline void mousse::forceSuSp::operator=(const forceSuSp& susp)
{
  first() = susp.first();
  second() = susp.second();
}
inline void mousse::forceSuSp::operator+=(const forceSuSp& susp)
{
  first() += susp.first();
  second() += susp.second();
}
inline void mousse::forceSuSp::operator-=(const forceSuSp& susp)
{
  first() -= susp.first();
  second() -= susp.second();
}
// Friend Operators 
inline mousse::forceSuSp mousse::operator+
(
  const forceSuSp& susp1,
  const forceSuSp& susp2
)
{
  return forceSuSp
  (
    susp1.first() + susp2.first(),
    susp1.second() + susp2.second()
  );
}
inline mousse::forceSuSp mousse::operator*
(
  const scalar s,
  const forceSuSp& susp
)
{
  return forceSuSp(susp.first()*s, susp.second()*s);
}
