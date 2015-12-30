// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PrandtlDelta
// Description
//   Apply Prandtl mixing-length based damping function to the specified
//   geometric delta to improve near-wall behavior or LES models.
//   \verbatim
//     delta = min(geometricDelta, (kappa/Cdelta)*y)
//   \endverbatim
//   Example specification in the turbulenceProperties dictionary:
//   \verbatim
//   delta           Prandtl;
//   PrandtlCoeffs
//   {
//     delta   cubeRootVol;
//     cubeRootVolCoeffs
//     {
//       deltaCoeff      1;
//     }
//     // Default coefficients
//     kappa           0.41;
//     Cdelta          0.158;
//   }
//   \endverbatim
// SourceFiles
//   prandtl_delta.cpp
#ifndef prandtl_delta_hpp_
#define prandtl_delta_hpp_
#include "les_delta.hpp"
namespace mousse
{
namespace LESModels
{
class PrandtlDelta
:
  public LESdelta
{
  // Private data
    autoPtr<LESdelta> geometricDelta_;
    scalar kappa_;
    scalar Cdelta_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    PrandtlDelta(const PrandtlDelta&);
    void operator=(const PrandtlDelta&);
    // Calculate the delta values
    void calcDelta();
public:
  //- Runtime type information
  TypeName("Prandtl");
  // Constructors
    //- Construct from name, turbulenceModel and dictionary
    PrandtlDelta
    (
      const word& name,
      const turbulenceModel& turbulence,
      const dictionary&
    );
  //- Destructor
  virtual ~PrandtlDelta()
  {}
  // Member Functions
    //- Read the LESdelta dictionary
    virtual void read(const dictionary&);
    // Correct values
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#endif
