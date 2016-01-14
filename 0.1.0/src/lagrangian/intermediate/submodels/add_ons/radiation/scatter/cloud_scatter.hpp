// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::cloudScatter
// Description
//   Cloud radiation scatter model
// SourceFiles
//   scatter_model.cpp
#ifndef radiationCloudScatter_H
#define radiationCloudScatter_H
#include "scatter_model.hpp"
namespace mousse
{
namespace radiation
{
class cloudScatter
:
  public scatterModel
{
  // Private data
    //- Absorption model dictionary
    dictionary coeffsDict_;
    //- Cloud names
    wordList cloudNames_;
public:
  //- Runtime type information
  TYPE_NAME("cloudScatter");
  // Constructors
    //- Construct from components
    cloudScatter
    (
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~cloudScatter();
  // Member Operators
    // Access
      //- Return scatter coefficient
      tmp<volScalarField> sigmaEff() const;
};
}  // namespace radiation
}  // namespace mousse
#endif
