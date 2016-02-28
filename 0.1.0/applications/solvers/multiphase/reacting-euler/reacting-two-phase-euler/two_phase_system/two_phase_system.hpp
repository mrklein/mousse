// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::twoPhaseSystem
// Description
//   Class which solves the volume fraction equations for two phases.
// SourceFiles
//   two_phase_system.cpp
#ifndef two_phase_system_hpp_
#define two_phase_system_hpp_
#include "phase_system.hpp"
namespace mousse
{
class dragModel;
class virtualMassModel;
class twoPhaseSystem
:
  public phaseSystem
{
  // Private member functions
    //- Return the drag coefficient for phase pair
    virtual tmp<volScalarField> Kd(const phasePairKey& key) const = 0;
    //- Return the face drag coefficient for phase pair
    virtual tmp<surfaceScalarField> Kdf(const phasePairKey& key) const = 0;
    //- Return the virtual mass coefficient for phase pair
    virtual tmp<volScalarField> Vm(const phasePairKey& key) const = 0;
    //- Return the face virtual mass coefficient for phase pair
    virtual tmp<surfaceScalarField> Vmf(const phasePairKey& key) const = 0;
    //- Return the combined force (lift + wall-lubrication) for phase pair
    virtual tmp<volVectorField> F(const phasePairKey& key) const = 0;
    //- Return the combined face-force (lift + wall-lubrication)
    //  for phase pair
    virtual tmp<surfaceScalarField> Ff(const phasePairKey& key) const = 0;
    //- Return the turbulent diffusivity for phase pair
    //  Multiplies the phase-fraction gradient
    virtual tmp<volScalarField> D(const phasePairKey& key) const = 0;
    //- Return true if there is mass transfer for phase
    virtual bool transfersMass(const phaseModel& phase) const = 0;
    //- Return the interfacial mass flow rate for phase pair
    virtual tmp<volScalarField> dmdt(const phasePairKey& key) const = 0;
protected:
  // Protected data
    //- Phase model 1
    phaseModel& phase1_;
    //- Phase model 2
    phaseModel& phase2_;
public:
  //- Runtime type information
  TYPE_NAME("twoPhaseSystem");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      twoPhaseSystem,
      dictionary,
      (
        const fvMesh& mesh
      ),
      (mesh)
    );
  // Constructors
    //- Construct from fvMesh
    twoPhaseSystem(const fvMesh&);
  //- Destructor
  virtual ~twoPhaseSystem();
  // Selectors
    static autoPtr<twoPhaseSystem> New
    (
      const fvMesh& mesh
    );
  // Member Functions
    //- Constant access phase model 1
    const phaseModel& phase1() const;
    //- Access phase model 1
    phaseModel& phase1();
    //- Constant access phase model 2
    const phaseModel& phase2() const;
    //- Access phase model 2
    phaseModel& phase2();
    //- Constant access the phase not given as an argument
    const phaseModel& otherPhase
    (
      const phaseModel& phase
    ) const;
    //- Return the momentum transfer matrices
    virtual autoPtr<momentumTransferTable> momentumTransfer() const = 0;
    //- Return the heat transfer matrices
    virtual autoPtr<heatTransferTable> heatTransfer() const = 0;
    //- Return the mass transfer matrices
    virtual autoPtr<massTransferTable> massTransfer() const = 0;
    using phaseSystem::sigma;
    //- Return the surface tension coefficient
    tmp<volScalarField> sigma() const;
    //- Return the drag model for the given phase
    const dragModel& drag(const phaseModel& phase) const;
    //- Return the drag coefficient
    tmp<volScalarField> Kd() const;
    //- Return the face drag coefficient
    tmp<surfaceScalarField> Kdf() const;
    //- Return the virtual mass model for the given phase
    const virtualMassModel& virtualMass(const phaseModel& phase) const;
    //- Return the virtual mass coefficient
    tmp<volScalarField> Vm() const;
    //- Return the face virtual mass coefficient
    tmp<surfaceScalarField> Vmf() const;
    //- Return the combined force (lift + wall-lubrication)
    tmp<volVectorField> F() const;
    //- Return the combined face-force (lift + wall-lubrication)
    tmp<surfaceScalarField> Ff() const;
    //- Return the turbulent diffusivity
    //  Multiplies the phase-fraction gradient
    tmp<volScalarField> D() const;
    //- Return true if there is mass transfer
    bool transfersMass() const;
    //- Return the interfacial mass flow rate
    tmp<volScalarField> dmdt() const;
    //- Solve for the phase fractions
    virtual void solve();
};
}  // namespace mousse

// Member Functions 
inline const mousse::phaseModel& mousse::twoPhaseSystem::phase1() const
{
  return phase1_;
}
inline mousse::phaseModel& mousse::twoPhaseSystem::phase1()
{
  return phase1_;
}
inline const mousse::phaseModel& mousse::twoPhaseSystem::phase2() const
{
  return phase2_;
}
inline mousse::phaseModel& mousse::twoPhaseSystem::phase2()
{
  return phase2_;
}
inline const mousse::phaseModel& mousse::twoPhaseSystem::otherPhase
(
  const phaseModel& phase
) const
{
  if (&phase == &phase1_)
  {
    return phase2_;
  }
  else
  {
    return phase1_;
  }
}
#endif
