// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cachedRandom
// Description
//   Random number generator.
//   Pre-computes and caches samples on construction, so that when sample01()
//   is called, the function simply returns the next (pre-computed) sample. On
//   reaching the last sample, the sample sequence is repeated.
//   Constructed using a seed and sample count. If the supplied count is
//   negative, no caching is performed, and a new sample is generated on each
//   call to sample01().
//   Note: the copy constructor cannot be used if count = -1.
// SourceFiles
//   cached_random.cpp
//   cached_random_templates.cpp
#ifndef cached_random_hpp_
#define cached_random_hpp_
#include "scalar_list.hpp"
namespace mousse
{
// Forward declaration of classes
class cachedRandom;
class cachedRandom
{
  // Private data
    //- Initial random number seed
    label seed_;
    //- List of scalar samples
    scalarList samples_;
    //- Current sample marker
    label sampleI_;
    //- Indicator, which tells if there is a stored gaussian sample
    bool hasGaussSample_;
    //- Stored sample value
    scalar gaussSample_;
  // Private Member Functions
    //- Returns the current sample
    scalar scalar01();
public:
  // Constructors
    //- Construct given seed and sample count
    cachedRandom(const label seed, const label count);
    //- Copy constructor with optional reset of sampleI
    cachedRandom(const cachedRandom& cr, const bool reset = false);
  // Destructor
  ~cachedRandom();
  // Member functions
    // Access
      //- Return const access to the initial random number seed
      inline label seed() const;
      //- Return const access to the list of samples
      inline const scalarList& samples() const;
      //- Return the current sample marker
      inline label sampleI() const;
    // Manipulation
      //- Return non-const access to the sample marker
      inline label& sampleI();
    // Evaluation
      // Random numbers
        //- Return a sample whose components lie in the range 0-1
        template<class Type>
        Type sample01();
        //- Return a sample whose components are normally distributed
        //  with zero mean and unity variance N(0, 1)
        template<class Type>
        Type GaussNormal();
        //- Return a sample between start and end
        template<class Type>
        Type position(const Type& start, const Type& end);
        //- Randomise value in the range 0-1
        template<class Type>
        void randomise01(Type& value);
      // Global random numbers - consistent across all processors
        //- Return a sample whose components lie in the range 0-1
        template<class Type>
        Type globalSample01();
        //- Return a sample whose components are normally distributed
        //  with zero mean and unity variance N(0, 1)
        template<class Type>
        Type globalGaussNormal();
        //- Return a sample between start and end
        template<class Type>
        Type globalPosition(const Type& start, const Type& end);
        //- Randomise value in the range 0-1
        template<class Type>
        void globalRandomise01(Type& value);
};
// Template specialisations
template<>
scalar cachedRandom::sample01<scalar>();
template<>
label cachedRandom::sample01<label>();
template<>
scalar cachedRandom::GaussNormal<scalar>();
template<>
label cachedRandom::GaussNormal<label>();
template<>
scalar cachedRandom::position<scalar>
(
  const scalar& start,
  const scalar& end
);
template<>
label cachedRandom::position<label>(const label& start, const label& end);
template<>
scalar cachedRandom::globalSample01<scalar>();
template<>
label cachedRandom::globalSample01<label>();
template<>
scalar cachedRandom::globalGaussNormal<scalar>();
template<>
label cachedRandom::globalGaussNormal<label>();
template<>
scalar cachedRandom::globalPosition<scalar>
(
  const scalar& start,
  const scalar& end
);
template<>
label cachedRandom::globalPosition<label>(const label& start, const label& end);
}  // namespace mousse

inline mousse::label mousse::cachedRandom::seed() const
{
  return seed_;
}
inline const mousse::scalarList& mousse::cachedRandom::samples() const
{
  return samples_;
}
inline mousse::label mousse::cachedRandom::sampleI() const
{
  return sampleI_;
}
inline mousse::label& mousse::cachedRandom::sampleI()
{
  return sampleI_;
}

#ifdef NoRepository
#   include "cached_random_templates.cpp"
#endif
#endif
