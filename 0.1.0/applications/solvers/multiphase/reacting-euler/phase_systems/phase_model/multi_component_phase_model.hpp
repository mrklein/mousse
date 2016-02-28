// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MultiComponentPhaseModel
// Description
//   Class which represents a phase with multiple species. Returns the species'
//   mass fractions, and their governing equations.
// SourceFiles
//   multi_component_phase_model.cpp
#ifndef multi_component_phase_model_hpp_
#define multi_component_phase_model_hpp_
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class MultiComponentPhaseModel
:
  public BasePhaseModel
{
protected:
  // Protected data
    //- Schmidt number
    dimensionedScalar Sc_;
    //- Residual phase fraction
    dimensionedScalar residualAlpha_;
    //- Inert species index
    label inertIndex_;
public:
  // Constructors
    MultiComponentPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~MultiComponentPhaseModel();
  // Member Functions
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Return the species fraction equation
    virtual tmp<fvScalarMatrix> YiEqn
    (
      volScalarField& Yi
    );
    //- Constant access the species mass fractions
    virtual const PtrList<volScalarField>& Y() const;
    //- Access the species mass fractions
    virtual PtrList<volScalarField>& Y();
};
}  // namespace mousse
#ifdef NoRepository
#   include "multi_component_phase_model.cpp"
#endif
#endif
