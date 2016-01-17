// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noScatter
// Description
//   Dummy scatter model for 'none'
// SourceFiles
//   scatter_model.cpp
#ifndef radiationConstantScatter_H
#define radiationConstantScatter_H
#include "scatter_model.hpp"
namespace mousse
{
namespace radiation
{
class noScatter
:
  public scatterModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noScatter(const dictionary& dict, const fvMesh& mesh);
  //- Destructor
  virtual ~noScatter();
  // Member Functions
    //- Return scatter coefficient
    tmp<volScalarField> sigmaEff() const;
};
}  // namespace radiation
}  // namespace mousse
#endif
