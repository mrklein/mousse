// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "buffered_accumulator.hpp"
// Static Data Members
template<class Type>
const char* const
  mousse::bufferedAccumulator<Type>::typeName("bufferedAccumulator");
// Private Member Functions 
template<class Type>
void mousse::bufferedAccumulator<Type>::accumulateAndResetBuffer(const label b)
{
  accumulationBuffer() += (*this)[b];
  averagesTaken_++;
  (*this)[b] = Field<Type>(bufferLength(), pTraits<Type>::zero);
  bufferOffsets_[b] = 0;
}
// Constructors 
template<class Type>
mousse::bufferedAccumulator<Type>::bufferedAccumulator()
:
  List< Field<Type> >(),
  averagesTaken_(),
  bufferOffsets_()
{}
template<class Type>
mousse::bufferedAccumulator<Type>::bufferedAccumulator
(
  const label nBuffers,
  const label bufferLength,
  const label bufferingInterval
)
:
  List< Field<Type> >(),
  averagesTaken_(),
  bufferOffsets_()
{
  setSizes
  (
    nBuffers,
    bufferLength,
    bufferingInterval
  );
}
template<class Type>
mousse::bufferedAccumulator<Type>::bufferedAccumulator
(
  const bufferedAccumulator<Type>& bA
)
:
  List< Field<Type> >(static_cast< List< Field<Type> > >(bA)),
  averagesTaken_(bA.averagesTaken()),
  bufferOffsets_(bA.bufferOffsets())
{}
// Destructor 
template<class Type>
mousse::bufferedAccumulator<Type>::~bufferedAccumulator()
{}
// Member Functions 
template<class Type>
void mousse::bufferedAccumulator<Type>::setSizes
(
  const label nBuffers,
  const label bufferLength,
  const label bufferingInterval
)
{
  (*this).setSize(nBuffers + 1);
  forAll((*this), b)
  {
    (*this)[b] = Field<Type>(bufferLength, pTraits<Type>::zero);
  }
  averagesTaken_ = 0;
  bufferOffsets_.setSize(nBuffers);
  forAll(bufferOffsets_, bO)
  {
    bufferOffsets_[bO] = -bufferingInterval * bO - 1;
  }
}
template<class Type>
mousse::label mousse::bufferedAccumulator<Type>::addToBuffers
(
  const List<Type>& valuesToAdd
)
{
  label bufferToRefill = -1;
  for (label b = 0; b < nBuffers(); b++)
  {
    Field<Type>& buf((*this)[b]);
    label& bO = bufferOffsets_[b];
    if (bO >= 0)
    {
      buf[bO] = valuesToAdd[b];
    }
    bO++;
    if (bO == bufferLength())
    {
      accumulateAndResetBuffer(b);
    }
    if (bO == 0)
    {
      if (bufferToRefill != -1)
      {
        FatalErrorIn("bufferedAccumulator<Type>::addToBuffers ")
          << "More than one bufferedAccumulator accumulation "
          << "buffer filled at once, this is considered an error."
          << abort(FatalError);
      }
      bufferToRefill = b;
    }
  }
  return bufferToRefill;
}
template<class Type>
mousse::Field<Type> mousse::bufferedAccumulator<Type>::averaged() const
{
  if (averagesTaken_)
  {
    Field<Type> bA = accumulationBuffer()/averagesTaken_;
    return bA;
  }
  else
  {
    WarningIn
    (
      "bufferedAccumulator<Type>::averagedbufferedAccumulator() const"
    )   << "Averaged correlation function requested but averagesTaken = "
      << averagesTaken_
      << ". Returning empty field."
      << endl;
    return Field<Type>(bufferLength(), pTraits<Type>::zero);
  }
}
template<class Type>
void mousse::bufferedAccumulator<Type>::resetAveraging()
{
  accumulationBuffer() = Field<Type>(bufferLength(), pTraits<Type>::zero);
  averagesTaken_ = 0;
}
// Member Operators 
template<class Type>
void mousse::bufferedAccumulator<Type>::operator=
(
  const bufferedAccumulator<Type>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FatalErrorIn
    (
      "bufferedAccumulator<Type>::operator=(const bufferedAccumulator&)"
    )   << "Attempted assignment to self"
      << abort(FatalError);
  }
  List< Field<Type> >::operator=(rhs);
  averagesTaken_ = rhs.averagesTaken();
  bufferOffsets_ = rhs.bufferOffsets();
}
#   include "buffered_accumulator_io.cpp"
