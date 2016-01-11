// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressibilityModels::linear
// Description
//   linear compressibility model.
// SourceFiles
//   linear.cpp
#ifndef linear_hpp_
#define linear_hpp_
#include "barotropic_compressibility_model.hpp"
#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace compressibilityModels
{
class linear
:
  public barotropicCompressibilityModel
{
  // Private data
    dimensionedScalar psiv_;
    dimensionedScalar psil_;
public:
  //- Runtime type information
  TYPE_NAME("linear");
  // Constructors
    //- Construct from components
    linear
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
  //- Destructor
  ~linear()
  {}
  // Member Functions
    //- Correct the linear compressibility
    void correct();
    //- Read transportProperties dictionary
    bool read(const dictionary& compressibilityProperties);
};
}  // namespace compressibilityModels
}  // namespace mousse
#endif
