// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correlation_function.hpp"


// Static Data Members
template<class Type>
const char* const
  mousse::correlationFunction<Type>::typeName("correlationFunction");


// Private Member Functions 
template<class Type>
void mousse::correlationFunction<Type>::setTimesAndSizes
(
  const label tZeroBufferSize
)
{
  sampleSteps_ = ceil(sampleInterval_/mesh_.time().deltaTValue());
  sampleInterval_ = sampleSteps_*mesh_.time().deltaTValue();
  label bufferLength{ceil(duration_/sampleInterval_)};
  duration_ = bufferLength*sampleInterval_;
  label bufferingInterval(ceil(averagingInterval_/sampleInterval_));
  averagingInterval_ = bufferingInterval*sampleInterval_;
  label nBuffers{ceil(duration_/averagingInterval_)};
  this->setSizes
  (
    nBuffers,
    bufferLength,
    bufferingInterval
  );
  tZeroBuffers_ =
    Field<Field<Type>>
    {
      nBuffers,
      Field<Type>{tZeroBufferSize, pTraits<Type>::zero}
    };
}


// Constructors 
template<class Type>
mousse::correlationFunction<Type>::correlationFunction
(
  const polyMesh& mesh,
  const dictionary& cfDict,
  const label tZeroBufferSize
)
:
  bufferedAccumulator<scalar>{},
  mesh_{mesh}
{
  duration_ = readScalar(cfDict.lookup("duration"));
  sampleInterval_ = readScalar(cfDict.lookup("sampleInterval"));
  averagingInterval_ = readScalar(cfDict.lookup("averagingInterval"));
  setTimesAndSizes(tZeroBufferSize);
}


template<class Type>
mousse::correlationFunction<Type>::correlationFunction
(
  const polyMesh& mesh,
  const label tZeroBufferSize,
  const scalar duration,
  const scalar sampleInterval,
  const scalar averagingInterval
)
:
  bufferedAccumulator<scalar>{},
  mesh_{mesh},
  duration_{duration},
  sampleInterval_{sampleInterval},
  averagingInterval_{averagingInterval}
{
  setTimesAndSizes(tZeroBufferSize);
}


// Destructor 
template<class Type>
mousse::correlationFunction<Type>::~correlationFunction()
{}


// Member Functions 
template<class Type>
void mousse::correlationFunction<Type>::calculateCorrelationFunction
(
  const Field<Type>& currentValues
)
{
  if (measurandFieldSize() != currentValues.size()) {
    FATAL_ERROR_IN("correlationFunction<Type>::calculateCorrelationFunction")
      << "Trying to supply a Field of length"
      << currentValues.size()
      << " to calculate the correlation function. "
      << "Expecting a Field of length "
      << measurandFieldSize() << nl
      << abort(FatalError);
  }
  List<scalar> cFSums{nBuffers(), 0.0};
  FOR_ALL(tZeroBuffers_, tZB) {
    scalar& cFSum = cFSums[tZB];
    const Field<Type>& tZeroBuffer = tZeroBuffers_[tZB];
    FOR_ALL(currentValues, cV) {
      const Type& tZeroBufferValue = tZeroBuffer[cV];
      const Type& currentValue = currentValues[cV];
      FOR_ALL(currentValue, component) {
        cFSum += tZeroBufferValue[component]*currentValue[component];
      }
    }
    cFSum /= (measurandFieldSize()*currentValues[0].size());
  }
  label bufferToRefill = addToBuffers(cFSums);
  if (bufferToRefill != -1) {
    tZeroBuffers_[bufferToRefill] = currentValues;
  }
}


template<class Type>
void mousse::correlationFunction<Type>::calculateCorrelationFunction
(
  const Type& currentValue
)
{
  if (measurandFieldSize() != 1) {
    FATAL_ERROR_IN("correlationFunction<Type>::calculateCorrelationFunction")
      << "Trying to supply a single value to calculate the correlation "
      << "function.  Expecting a Field of length "
      << measurandFieldSize()
      << abort(FatalError);
  }
  calculateCorrelationFunction(Field<Type>{1, currentValue});
}


template<class Type>
mousse::scalar mousse::correlationFunction<Type>::integral() const
{
  Field<scalar> averageCF{averaged()};
  scalar cFIntegral = 0.0;
  for (label v = 0; v < averageCF.size() - 1; v++) {
    cFIntegral += 0.5*sampleInterval_*(averageCF[v+1] + averageCF[v]);
  }
  return cFIntegral;
}

#include "correlation_function_io.ipp"
