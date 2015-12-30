// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::correlationFunction
// Description
// SourceFiles
//   correlation_function_i.hpp
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
    //- Disallow default bitwise copy construct
    correlationFunction(const correlationFunction<Type>&);
    //- Disallow default bitwise assignment
    void operator=(const correlationFunction<Type>&);
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
#include "correlation_function_i.hpp"
#ifdef NoRepository
#   include "correlation_function.cpp"
#endif
#endif
