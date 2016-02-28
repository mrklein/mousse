// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PurePhaseModel
// Description
//   Class which represents pure phases, i.e. without any species. Returns an
//   empty list of mass fractions.
// SourceFiles
//   pure_phase_model.cpp
#ifndef pure_phase_model_hpp_
#define pure_phase_model_hpp_
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class PurePhaseModel
:
  public BasePhaseModel
{
protected:
  // Protected data
    //- Empty mass fraction field list
    PtrList<volScalarField> Y_;
public:
  // Constructors
    PurePhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~PurePhaseModel();
  // Member Functions
    //- Return the species fraction equation
    virtual tmp<fvScalarMatrix> YiEqn(volScalarField& Yi);
    // Thermo
      //- Return the species mass fractions
      virtual const PtrList<volScalarField>& Y() const;
      //- Access the species mass fractions
      virtual PtrList<volScalarField>& Y();
};
}  // namespace mousse
#ifdef NoRepository
#   include "pure_phase_model.cpp"
#endif
#endif
