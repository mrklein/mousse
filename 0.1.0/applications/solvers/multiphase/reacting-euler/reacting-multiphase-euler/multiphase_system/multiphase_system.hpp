// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiphaseSystem
// Description
//   Class which solves the volume fraction equations for two phases.
// SourceFiles
//   multiphase_system.cpp
#ifndef multiphase_system_hpp_
#define multiphase_system_hpp_
#include "phase_system.hpp"
namespace mousse
{
class dragModel;
class virtualMassModel;
class multiphaseSystem
:
  public phaseSystem
{
  // Private data
    volScalarField alphas_;
    typedef HashTable<scalar, phasePairKey, phasePairKey::hash> cAlphaTable;
    cAlphaTable cAlphas_;
    //- Stabilisation for normalisation of the interface normal
    const dimensionedScalar deltaN_;
    //- Conversion factor for degrees into radians
    static const scalar convertToRad;
  // Private member functions
    void calcAlphas();
    void solveAlphas();
    tmp<surfaceVectorField> nHatfv
    (
      const volScalarField& alpha1,
      const volScalarField& alpha2
    ) const;
    tmp<surfaceScalarField> nHatf
    (
      const volScalarField& alpha1,
      const volScalarField& alpha2
    ) const;
    void correctContactAngle
    (
      const phaseModel& alpha1,
      const phaseModel& alpha2,
      surfaceVectorField::GeometricBoundaryField& nHatb
    ) const;
    tmp<volScalarField> K
    (
      const phaseModel& alpha1,
      const phaseModel& alpha2
    ) const;
    //- Return the drag coefficient for phase pair
    virtual tmp<volScalarField> Kd(const phasePairKey& key) const = 0;
    //- Return the face drag coefficient for phase pair
    virtual tmp<surfaceScalarField> Kdf(const phasePairKey& key) const = 0;
    //- Return the virtual mass coefficient for phase pair
    virtual tmp<volScalarField> Vm(const phasePairKey& key) const = 0;
    //- Return the face virtual mass coefficient for phase pair
    virtual tmp<surfaceScalarField> Vmf(const phasePairKey& key) const = 0;
    //- Return the turbulent diffusivity for phase pair
    //  Multiplies the phase-fraction gradient
    virtual tmp<volScalarField> D(const phasePairKey& key) const = 0;
    //- Return the interfacial mass flow rate for phase pair
    virtual tmp<volScalarField> dmdt(const phasePairKey& key) const = 0;
public:
  //- Runtime type information
  TYPE_NAME("multiphaseSystem");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      multiphaseSystem,
      dictionary,
      (
        const fvMesh& mesh
      ),
      (mesh)
    );
  // Constructors
    //- Construct from fvMesh
    multiphaseSystem(const fvMesh&);
  //- Destructor
  virtual ~multiphaseSystem();
  // Selectors
    static autoPtr<multiphaseSystem> New
    (
      const fvMesh& mesh
    );
  // Member Functions
    //- Return the drag coefficient for all phase-pairs
    virtual const phaseSystem::KdTable& Kds() const = 0;
    //- Return the drag coefficient for phase
    virtual tmp<volScalarField> Kd(const phaseModel& phase) const = 0;
    //- Return the combined force (lift + wall-lubrication) for phase pair
    virtual autoPtr<PtrList<mousse::volVectorField> > Fs() const = 0;
    //- Return the turbulent dispersion force on faces for phase pair
    virtual autoPtr<PtrList<mousse::surfaceScalarField> > phiDs
    (
      const PtrList<volScalarField>& rAUs
    ) const = 0;
    //- Return true if there is mass transfer for phase
    virtual bool transfersMass(const phaseModel& phase) const = 0;
    //- Return the total interfacial mass transfer rate for phase
    virtual tmp<volScalarField> dmdt(const phaseModel& phase) const = 0;
    //- Return the momentum transfer matrices
    virtual autoPtr<momentumTransferTable> momentumTransfer() const = 0;
    //- Return the heat transfer matrices
    virtual autoPtr<heatTransferTable> heatTransfer() const = 0;
    //- Return the mass transfer matrices
    virtual autoPtr<massTransferTable> massTransfer() const = 0;
    tmp<surfaceScalarField> surfaceTension(const phaseModel& phase) const;
    //- Indicator of the proximity of the interface
    //  Field values are 1 near and 0 away for the interface.
    tmp<volScalarField> nearInterface() const;
    //- Solve for the phase fractions
    virtual void solve();
};
}  // namespace mousse
#endif
