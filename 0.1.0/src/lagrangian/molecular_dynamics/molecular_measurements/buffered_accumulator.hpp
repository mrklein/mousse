// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::bufferedAccumulator
// Description
// SourceFiles
//   buffered_accumulator_i.hpp
//   buffered_accumulator.cpp
//   buffered_accumulator_io.cpp
#ifndef buffered_accumulator_hpp_
#define buffered_accumulator_hpp_
#include "field.hpp"
namespace mousse
{
template<class Type>
class bufferedAccumulator;
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const bufferedAccumulator<Type>&
);
template<class Type>
class bufferedAccumulator
:
  public List< Field<Type> >
{
  // Private data
    label averagesTaken_;
    List<label> bufferOffsets_;
  // Private Member Functions
    inline Field<Type>& accumulationBuffer();
    inline const Field<Type>& accumulationBuffer() const;
    void accumulateAndResetBuffer(const label b);
public:
  //- Component type
  typedef typename pTraits<Type>::cmptType cmptType;
  // Static data members
    static const char* const typeName;
  // Constructors
    //- Construct null
    bufferedAccumulator();
    //- Construct from components
    bufferedAccumulator
    (
      const label nBuffers,
      const label bufferLength,
      const label bufferingInterval
    );
    //- Construct as copy
    bufferedAccumulator(const bufferedAccumulator<Type>&);
  //- Destructor
  ~bufferedAccumulator();
  // Member Functions
    label addToBuffers(const List<Type>& valuesToAdd);
    Field<Type> averaged() const;
    void resetAveraging();
    // Access
      inline label averagesTaken() const;
      inline label nBuffers() const;
      inline label bufferLength() const;
      inline const List<label>& bufferOffsets() const;
    // Edit
      void setSizes
      (
        const label nBuffers,
        const label bufferLength,
        const label bufferingInterval
      );
  // Member Operators
    void operator=(const bufferedAccumulator<Type>&);
  // IOstream Operators
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const bufferedAccumulator<Type>&
    );
};
}  // namespace mousse
#include "buffered_accumulator_i.hpp"
#ifdef NoRepository
#   include "buffered_accumulator.cpp"
#endif
#endif
