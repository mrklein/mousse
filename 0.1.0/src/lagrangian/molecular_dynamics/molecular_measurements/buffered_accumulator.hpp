#ifndef LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULAR_MEASUREMENTS_BUFFERED_ACCUMULATOR_HPP_
#define LAGRANGIAN_MOLECULAR_DYNAMICS_MOLECULAR_MEASUREMENTS_BUFFERED_ACCUMULATOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::bufferedAccumulator

#include "field.hpp"


namespace mousse {

template<class Type> class bufferedAccumulator;

template<class Type>
Ostream& operator<<
(
  Ostream&,
  const bufferedAccumulator<Type>&
);


template<class Type>
class bufferedAccumulator
:
  public List<Field<Type>>
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

#include "buffered_accumulator.ipp"

#endif
