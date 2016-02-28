// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::AnisothermalPhaseModel
// Description
//   Class which represents a phase for which the temperature (strictly energy)
//   varies. Returns the energy equation and corrects the thermodynamic model.
// SourceFiles
//   anisothermal_phase_model.cpp
#ifndef anisothermal_phase_model_hpp_
#define anisothermal_phase_model_hpp_
#include "phase_model.hpp"
namespace mousse
{
template<class BasePhaseModel>
class AnisothermalPhaseModel
:
  public BasePhaseModel
{
  // Private data
    //- Dilatation rate
    tmp<volScalarField> divU_;
    //- Kinetic energy
    volScalarField K_;
public:
  // Constructors
    AnisothermalPhaseModel
    (
      const phaseSystem& fluid,
      const word& phaseName,
      const label index
    );
  //- Destructor
  virtual ~AnisothermalPhaseModel();
  // Member Functions
    //- Correct the kinematics
    virtual void correctKinematics();
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Return the enthalpy equation
    virtual tmp<fvScalarMatrix> heEqn();
    // Compressibility (variable density)
      //- Return true if the phase is compressible otherwise false
      virtual bool compressible() const;
      //- Return the phase dilatation rate (d(alpha)/dt + div(alpha*phi))
      virtual const tmp<volScalarField>& divU() const;
      //- Set the phase dilatation rate (d(alpha)/dt + div(alpha*phi))
      virtual void divU(const tmp<volScalarField>& divU);
      //- Return the phase kinetic energy
      virtual const volScalarField& K() const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "anisothermal_phase_model.cpp"
#endif
#endif
