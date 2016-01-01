// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::randomise
// Description
//   Adds a random component to a field, with a given perturbation magnitude.
// SourceFiles
//   randomise.cpp
#ifndef randomise_hpp_
#define randomise_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    randomise(const randomise&);
    //- Disallow default bitwise assignment
    void operator=(const randomise&);
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
  TypeName("randomise");
  // Constructors
    //- Construct null
    randomise();
  //- Destructor
  virtual ~randomise();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_random_field.cpp"
#endif
#endif
