// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::components
// Description
//   Writes scalar fields corresponding to each component of the supplied
//   field (name) for each time.
// SourceFiles
//   components.cpp
#ifndef components_hpp_
#define components_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class components
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
      void writeComponentFields
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("components");
  // Constructors
    //- Construct null
    components();
    //- Disallow default bitwise copy construct
    components(const components&) = delete;
    //- Disallow default bitwise assignment
    components& operator=(const components&) = delete;
  //- Destructor
  virtual ~components();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_component_fields.cpp"
#endif
#endif
