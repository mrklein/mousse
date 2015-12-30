// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
inline const mousse::Field< mousse::Field<Type> >&
mousse::correlationFunction<Type>::tZeroBuffers() const
{
  return tZeroBuffers_;
}
template<class Type>
inline mousse::scalar mousse::correlationFunction<Type>::duration() const
{
  return duration_;
}
template<class Type>
inline mousse::scalar mousse::correlationFunction<Type>::sampleInterval() const
{
  return sampleInterval_;
}
template<class Type>
inline mousse::scalar mousse::correlationFunction<Type>::averagingInterval() const
{
  return averagingInterval_;
}
template<class Type>
inline mousse::label mousse::correlationFunction<Type>::sampleSteps() const
{
  return sampleSteps_;
}
template<class Type>
inline mousse::label mousse::correlationFunction<Type>::measurandFieldSize() const
{
  return tZeroBuffers_[0].size();
}
