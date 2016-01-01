// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::div
// Description
//   Writes scalar fields corresponding to the divergence of the supplied
//   field (name) for each time.
// SourceFiles
//   div.cpp
#ifndef div_hpp_
#define div_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class div
:
  public calcType
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    div(const div&);
    //- Disallow default bitwise assignment
    void operator=(const div&);
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
      //- Write div fields
      template<class Type>
      void writeDivField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TypeName("div");
  // Constructors
    //- Construct null
    div();
  //- Destructor
  virtual ~div();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_div_field.cpp"
#endif
#endif
