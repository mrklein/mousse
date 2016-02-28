// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InertPhaseModel
// Description
//   Class which represents an inert phase, with no reactions. Returns zero
//   reaction rate and heat.
// SourceFiles
//   inert_phase_model.cpp
#ifndef inert_phase_model_hpp_
#define inert_phase_model_hpp_
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class InertPhaseModel
:
  public BasePhaseModel
{
public:
  // Constructors
    InertPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~InertPhaseModel();
  // Member Functions
    // Thermo
      //- Return the fuel consumption rate matrix
      virtual tmp<fvScalarMatrix> R
      (
        volScalarField& Yi
      ) const;
      //- Return the reaction heat source
      virtual tmp<volScalarField> Sh() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "inert_phase_model.cpp"
#endif
#endif
