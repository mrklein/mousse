#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_NO_SOOT_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_SOOT_MODEL_NO_SOOT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::noSoot
// Description
//   noSoot
// SourceFiles
//   no_soot.cpp
#include "soot_model.hpp"
namespace mousse
{
namespace radiation
{
class noSoot
:
  public sootModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from components
    noSoot(const dictionary& dict, const fvMesh& mesh, const word&);
  //- Destructor
  virtual ~noSoot();
  // Member Functions
    // Edit
      //- Main update/correction routine
      void correct();
    // Access
      //- Return Ysoot
      const volScalarField& soot() const;
};
}  // namespace mousse
}  // namespace radiation
#endif
