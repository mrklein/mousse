#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_ASPECT_RATIO_MODELS_CONSTANT_ASPECT_RATIO_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_ASPECT_RATIO_MODELS_CONSTANT_ASPECT_RATIO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantAspectRatio
// Description
//   Constant value aspect ratio model.
// SourceFiles
//   constant_aspect_ratio.cpp
#include "aspect_ratio_model.hpp"
namespace mousse
{
namespace aspectRatioModels
{
class constantAspectRatio
:
  public aspectRatioModel
{
  // Private data
    //- Constant aspect ratio value
    const dimensionedScalar E0_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from a dictionary and an ordered phase pair
    constantAspectRatio
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~constantAspectRatio();
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
};
}  // namespace aspectRatioModels
}  // namespace mousse
#endif
