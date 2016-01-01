// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::interpolate
// Description
//   Interpolates volume fields to surface fields for each time.
// SourceFiles
//   interpolate.cpp
#ifndef interpolate_hpp_
#define interpolate_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class interpolate
:
  public calcType
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    interpolate(const interpolate&);
    //- Disallow default bitwise assignment
    void operator=(const interpolate&);
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
      //- Write interpolate fields
      template<class Type>
      void writeInterpolateField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TypeName("interpolate");
  // Constructors
    //- Construct null
    interpolate();
  //- Destructor
  virtual ~interpolate();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_interpolate_field.cpp"
#endif
#endif
