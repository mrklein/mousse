// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::magGrad
// Description
//   Writes scalar fields corresponding to the magnitude ot the gradient
//   of the supplied field (name) for each time.
// SourceFiles
//   mag_grad.cpp
#ifndef mag_grad_hpp_
#define mag_grad_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class magGrad
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
      //- Write magGrad fields
      template<class Type>
      void writeMagGradField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("magGrad");
  // Constructors
    //- Construct null
    magGrad();
    //- Disallow default bitwise copy construct
    magGrad(const magGrad&) = delete;
    //- Disallow default bitwise assignment
    magGrad& operator=(const magGrad&) = delete;
  //- Destructor
  virtual ~magGrad();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_mag_grad_field.cpp"
#endif
#endif
