#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SURFACE_TENSION_MODELS_CONSTANT_SURFACE_TENSION_COEFFICIENT_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SURFACE_TENSION_MODELS_CONSTANT_SURFACE_TENSION_COEFFICIENT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantSurfaceTensionCoefficient
// Description
//   Constant value surface tension model.
// SourceFiles
//   constant_surface_tension_coefficient.cpp
#include "surface_tension_model.hpp"
namespace mousse
{
namespace surfaceTensionModels
{
class constantSurfaceTensionCoefficient
:
  public surfaceTensionModel
{
  // Private data
    //- Constant surface tension value
    const dimensionedScalar sigma_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from a dictionary and a phase pair
    constantSurfaceTensionCoefficient
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~constantSurfaceTensionCoefficient();
  // Member Functions
    //- Aspect ratio
    virtual tmp<volScalarField> sigma() const;
};
}  // namespace surfaceTensionModels
}  // namespace mousse
#endif
