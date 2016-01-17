// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::constantScatter
// Description
//   Constant radiation scatter coefficient
// SourceFiles
//   scatter_model.cpp
#ifndef radiationConstantScatter_H
#define radiationConstantScatter_H
#include "scatter_model.hpp"
namespace mousse
{
namespace radiation
{
class constantScatter
:
  public scatterModel
{
  // Private data
    //- Coefficients dictionary
    dictionary coeffsDict_;
    //- Scattering coefficient / [1/m]
    dimensionedScalar sigma_;
    //- Linear-anisotropic phase function coefficient / []
    //  -1 < C < 1
    //  - = backward scattering
    //  0 = isotropic scattering (reasonable default value)
    //  + = forward scattering
    dimensionedScalar C_;
public:
  //- Runtime type information
  TYPE_NAME("constantScatter");
  // Constructors
    //- Construct from components
    constantScatter(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~constantScatter();
  // Member Functions
    //- Return scatter coefficient
    tmp<volScalarField> sigmaEff() const;
};
}  // namespace radiation
}  // namespace mousse
#endif
