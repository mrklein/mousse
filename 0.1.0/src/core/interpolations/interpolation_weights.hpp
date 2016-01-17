// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationWeights
// Description
//   Abstract base class for interpolating in 1D
// SourceFiles
//   interpolation_weights.cpp
//   interpolation_weights_templates.cpp

#ifndef interpolation_weights_hpp_
#define interpolation_weights_hpp_

#include "scalar_field.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "point_field.hpp"

namespace mousse
{

class fvMesh;
class objectRegistry;

class interpolationWeights
{
protected:
    const scalarField& samples_;

public:
  //- Runtime type information
  TYPE_NAME("interpolationWeights");

  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      interpolationWeights,
      word,
      (
        const scalarField& samples
      ),
      (samples)
    );

  // Constructors

    //- Construct from components
    interpolationWeights(const scalarField& samples);

    //- Disallow default bitwise copy construct
    interpolationWeights(const interpolationWeights&) = delete;

    //- Disallow default bitwise assignment
    interpolationWeights& operator=(const interpolationWeights&) = delete;

  // Selectors

    //- Return a reference to the selected interpolationWeights
    static autoPtr<interpolationWeights> New
    (
      const word& type,
      const scalarField& samples
    );

  //- Destructor
  virtual ~interpolationWeights();

  // Member Functions

    //- Calculate weights and indices to calculate t from samples.
    //  Returns true if indices changed.
    virtual bool valueWeights
    (
      const scalar t,
      labelList& indices,
      scalarField& weights
    ) const = 0;

    //- Calculate weights and indices to calculate integrand of t1..t2
    //  from samples. Returns true if indices changed.
    virtual bool integrationWeights
    (
      const scalar t1,
      const scalar t2,
      labelList& indices,
      scalarField& weights
    ) const = 0;

    //- Helper: weighted sum
    template<class ListType1, class ListType2>
    static typename outerProduct
    <
      typename ListType1::value_type,
      typename ListType2::value_type
    >::type
    weightedSum(const ListType1& f1, const ListType2& f2);

};

}  // namespace mousse

#ifdef NoRepository
#   include "interpolation_weights_templates.cpp"
#endif
#endif
