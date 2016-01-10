// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::x0() const
{
  return x0_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::dx() const
{
  return dx_;
}
template<class Type>
const mousse::Switch& mousse::uniformInterpolationTable<Type>::log10() const
{
  return log10_;
}
template<class Type>
const mousse::Switch& mousse::uniformInterpolationTable<Type>::bound() const
{
  return bound_;
}
template<class Type>
mousse::scalar& mousse::uniformInterpolationTable<Type>::x0()
{
  return x0_;
}
template<class Type>
mousse::scalar& mousse::uniformInterpolationTable<Type>::dx()
{
  return dx_;
}
template<class Type>
mousse::Switch& mousse::uniformInterpolationTable<Type>::log10()
{
  return log10_;
}
template<class Type>
mousse::Switch& mousse::uniformInterpolationTable<Type>::bound()
{
  return bound_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::xMin() const
{
  return x0_;
}
template<class Type>
mousse::scalar mousse::uniformInterpolationTable<Type>::xMax() const
{
  return x0_ + dx_*(size() - 1);
}
