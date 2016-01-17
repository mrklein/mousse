// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::magSqr
// Description
//   Calculates and writes the magnitude-sqaured of a field for each time
// SourceFiles
//   mag_sqr.cpp
#ifndef mag_sqr_hpp_
#define mag_sqr_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class magSqr
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
      void writeMagSqrField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("magSqr");
  // Constructors
    //- Construct null
    magSqr();
    //- Disallow default bitwise copy construct
    magSqr(const magSqr&) = delete;
    //- Disallow default bitwise assignment
    magSqr& operator=(const magSqr&) = delete;
  //- Destructor
  virtual ~magSqr();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_mag_sqr_field.cpp"
#endif
#endif
