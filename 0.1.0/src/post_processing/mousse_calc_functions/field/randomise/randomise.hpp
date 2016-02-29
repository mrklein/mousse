#ifndef POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_RANDOMISE_RANDOMISE_HPP_
#define POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_RANDOMISE_RANDOMISE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::randomise
// Description
//   Adds a random component to a field, with a given perturbation magnitude.
// SourceFiles
//   randomise.cpp
#include "calc_type.hpp"
#include "random.hpp"
namespace mousse
{
namespace calcTypes
{
class randomise
:
  public calcType
{
protected:
  // Member Functions
    // Calculation routines
      //- Initialise - typically setting static variables,
      //  e.g. command line arguments
      virtual void init();
      //- Pre-time loop calculations
      virtual void preCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
      //- Time loop calculations
      virtual void calc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
    // I-O
      //- Write component fields
      template<class Type>
      void writeRandomField
      (
        const IOobject& header,
        const scalar pertMag,
        Random& rand,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("randomise");
  // Constructors
    //- Construct null
    randomise();
    //- Disallow default bitwise copy construct
    randomise(const randomise&) = delete;
    //- Disallow default bitwise assignment
    randomise& operator=(const randomise&) = delete;
  //- Destructor
  virtual ~randomise();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_random_field.cpp"
#endif
#endif
