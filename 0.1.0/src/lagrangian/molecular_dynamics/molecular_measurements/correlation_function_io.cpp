// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correlation_function.hpp"
#include "iostreams.hpp"
template<class Type>
bool mousse::correlationFunction<Type>::writeAveraged(Ostream& os) const
{
  Field<scalar> averageCF(averaged());
  forAll(averageCF, v)
  {
    os  << v*sampleInterval()
      << token::SPACE
      << averageCF[v]
      << nl;
  }
  return os.good();
}
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const correlationFunction<Type>& cF
)
{
  os  << cF.duration()
    << nl << cF.sampleInterval()
    << nl << cF.averagingInterval()
    << nl << cF.sampleSteps()
    << nl << cF.tZeroBuffers()
    << nl << static_cast<const bufferedAccumulator<scalar>&>(cF);
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<"
    "(Ostream&, const correlationFunction<Type>&)"
  );
  return os;
}
