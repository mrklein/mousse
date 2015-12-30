// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

namespace mousse
{
// Private Member Functions 
template<class Type>
inline Field<Type>& bufferedAccumulator<Type>::accumulationBuffer()
{
  return (*this)[nBuffers()];
}
template<class Type>
inline const Field<Type>& bufferedAccumulator<Type>::accumulationBuffer() const
{
  return (*this)[nBuffers()];
}
// Member Functions 
template<class Type>
inline label bufferedAccumulator<Type>::averagesTaken() const
{
  return averagesTaken_;
}
template<class Type>
inline label bufferedAccumulator<Type>::nBuffers() const
{
  return bufferOffsets_.size();
}
template<class Type>
inline label bufferedAccumulator<Type>::bufferLength() const
{
  return (*this)[0].size();
}
template<class Type>
inline const List<label>& bufferedAccumulator<Type>::bufferOffsets() const
{
  return bufferOffsets_;
}
}  // namespace mousse
