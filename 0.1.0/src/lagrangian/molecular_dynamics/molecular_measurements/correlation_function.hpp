// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::correlationFunction
// Description
// SourceFiles
//   correlation_function.cpp
//   correlation_function_io.cpp
#ifndef correlation_function_hpp_
#define correlation_function_hpp_
#include "buffered_accumulator.hpp"
#include "dictionary.hpp"
namespace mousse
{
template<class Type>
class correlationFunction;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const correlationFunction<Type>&
);
template<class Type>
class correlationFunction
:
  public bufferedAccumulator<scalar>
{
  // Private data
    const polyMesh& mesh_;
    Field< Field<Type> > tZeroBuffers_;
    scalar duration_;
    scalar sampleInterval_;
    scalar averagingInterval_;
    label sampleSteps_;
  // Private Member Functions
    void setTimesAndSizes(const label);
public:
  //- Component type
  typedef typename pTraits<Type>::cmptType cmptType;
  // Static data members
    static const char* const typeName;
  // Constructors
    //- Construct from dictionary
    correlationFunction
    (
      const polyMesh&,
      const dictionary&,
      const label tZeroBufferSize
    );
    //- Construct from components
    correlationFunction
    (
      const polyMesh&,
      const label tZeroBufferSize,
      const scalar duration,
      const scalar sampleInterval,
      const scalar averagingInterval
    );
    //- Disallow default bitwise copy construct
    correlationFunction(const correlationFunction<Type>&) = delete;
    //- Disallow default bitwise assignment
    correlationFunction<Type>& operator=
    (
      const correlationFunction<Type>&
    ) = delete;
  //- Destructor
  ~correlationFunction();
  // Member Functions
    void calculateCorrelationFunction(const Field<Type>&);
    void calculateCorrelationFunction(const Type&);
    scalar integral() const;
    bool writeAveraged(Ostream&) const;
  // Access
    inline const Field< Field<Type> >& tZeroBuffers() const;
    inline scalar duration() const;
    inline scalar sampleInterval() const;
    inline scalar averagingInterval() const;
    inline label sampleSteps() const;
    inline label measurandFieldSize() const;
  // IOstream Operators
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const correlationFunction<Type>&
    );
};
}  // namespace mousse

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

#ifdef NoRepository
#include "correlation_function.cpp"
#endif
#endif
