// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::twoPhaseSystem
// Description
// SourceFiles
//   two_phase_system.cpp
#ifndef two_phase_system_hpp_
#define two_phase_system_hpp_
#include "iodictionary.hpp"
#include "phase_model.hpp"
#include "phase_pair.hpp"
#include "ordered_phase_pair.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "drag_model.hpp"
namespace mousse
{
class virtualMassModel;
class heatTransferModel;
class liftModel;
class wallLubricationModel;
class turbulentDispersionModel;
class blendingMethod;
template <class modelType> class BlendedInterfacialModel;
class twoPhaseSystem
:
  public IOdictionary
{
  // Private data
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Phase model 1
    phaseModel phase1_;
    //- Phase model 2
    phaseModel phase2_;
    //- Total volumetric flux
    surfaceScalarField phi_;
    //- Dilatation term
    volScalarField dgdt_;
    //- Optional dispersion diffusivity
    tmp<surfaceScalarField> pPrimeByA_;
    //- Unordered phase pair
    autoPtr<phasePair> pair_;
    //- Phase pair for phase 1 dispersed in phase 2
    autoPtr<orderedPhasePair> pair1In2_;
    //- Phase pair for phase 2 dispersed in phase 1
    autoPtr<orderedPhasePair> pair2In1_;
    //- Blending methods
    HashTable<autoPtr<blendingMethod>, word, word::hash> blendingMethods_;
    //- Drag model
    autoPtr<BlendedInterfacialModel<dragModel> > drag_;
    //- Virtual mass model
    autoPtr<BlendedInterfacialModel<virtualMassModel> > virtualMass_;
    //- Heat transfer model
    autoPtr<BlendedInterfacialModel<heatTransferModel> > heatTransfer_;
    //- Lift model
    autoPtr<BlendedInterfacialModel<liftModel> > lift_;
    //- Wall lubrication model
    autoPtr<BlendedInterfacialModel<wallLubricationModel> >
      wallLubrication_;
    //- Wall lubrication model
    autoPtr<BlendedInterfacialModel<turbulentDispersionModel> >
      turbulentDispersion_;
  // Private member functions
    //- Return the mixture flux
    tmp<surfaceScalarField> calcPhi() const;
public:
  // Constructors
    //- Construct from fvMesh
    twoPhaseSystem(const fvMesh&, const dimensionedVector& g);
  //- Destructor
  virtual ~twoPhaseSystem();
  // Member Functions
    //- Return the mixture density
    tmp<volScalarField> rho() const;
    //- Return the mixture velocity
    tmp<volVectorField> U() const;
    //- Return the drag coefficient
    tmp<volScalarField> Kd() const;
    //- Return the face drag coefficient
    tmp<surfaceScalarField> Kdf() const;
    //- Return the virtual mass coefficient
    tmp<volScalarField> Vm() const;
    //- Return the face virtual mass coefficient
    tmp<surfaceScalarField> Vmf() const;
    //- Return the heat transfer coefficient
    tmp<volScalarField> Kh() const;
    //- Return the combined force (lift + wall-lubrication)
    tmp<volVectorField> F() const;
    //- Return the combined face-force (lift + wall-lubrication)
    tmp<surfaceScalarField> Ff() const;
    //- Return the turbulent diffusivity
    //  Multiplies the phase-fraction gradient
    tmp<volScalarField> D() const;
    //- Solve for the two-phase-fractions
    void solve();
    //- Correct two-phase properties other than turbulence
    void correct();
    //- Correct two-phase turbulence
    void correctTurbulence();
    //- Read base phaseProperties dictionary
    bool read();
    // Access
      //- Return the drag model for the given phase
      const dragModel& drag(const phaseModel& phase) const;
      //- Return the virtual mass model for the given phase
      const virtualMassModel& virtualMass(const phaseModel& phase) const;
      //- Return the surface tension coefficient
      const dimensionedScalar& sigma() const;
      //- Return the mesh
      inline const fvMesh& mesh() const;
      //- Return phase model 1
      inline const phaseModel& phase1() const;
      //- Return non-const access to phase model 1
      inline phaseModel& phase1();
      //- Return phase model 2
      inline const phaseModel& phase2() const;
      //- Return non-const access to phase model 2
      inline phaseModel& phase2();
      //- Return the phase not given as an argument
      inline const phaseModel& otherPhase(const phaseModel& phase) const;
      //- Return the mixture flux
      inline const surfaceScalarField& phi() const;
      //- Return non-const access to the the mixture flux
      inline surfaceScalarField& phi();
      //- Return the dilatation term
      inline const volScalarField& dgdt() const;
      //- Return non-const access to the dilatation parameter
      inline volScalarField& dgdt();
      //- Return non-const access to the dispersion diffusivity
      inline tmp<surfaceScalarField>& pPrimeByA();
};
}  // namespace mousse

// Member Functions 
inline const mousse::fvMesh& mousse::twoPhaseSystem::mesh() const
{
  return mesh_;
}
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
inline const mousse::surfaceScalarField& mousse::twoPhaseSystem::phi() const
{
  return phi_;
}
inline mousse::surfaceScalarField& mousse::twoPhaseSystem::phi()
{
  return phi_;
}
inline const mousse::volScalarField& mousse::twoPhaseSystem::dgdt() const
{
  return dgdt_;
}
inline mousse::volScalarField& mousse::twoPhaseSystem::dgdt()
{
  return dgdt_;
}
inline mousse::tmp<mousse::surfaceScalarField>& mousse::twoPhaseSystem::pPrimeByA()
{
  return pPrimeByA_;
}
#endif
