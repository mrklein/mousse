// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseSystem
// Description
//   Class to represent a system of phases and model interfacial transfers
//   between them.
// SourceFiles
//   phase_system.cpp
#ifndef PHASE_SYSTEM_HPP_
#define PHASE_SYSTEM_HPP_
#include "iodictionary.hpp"
#include "phase_model.hpp"
#include "phase_pair.hpp"
#include "ordered_phase_pair.hpp"
#include "hash_ptr_table.hpp"
#include "ptr_list_dictionary.hpp"
#include "io_mrf_zone_list.hpp"
#include "fv_io_option_list.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrices_fwd.hpp"
namespace mousse
{
class blendingMethod;
template <class modelType> class BlendedInterfacialModel;
class surfaceTensionModel;
class aspectRatioModel;
class phaseSystem
:
  public IOdictionary
{
public:
  // Public typedefs
    typedef
      HashPtrTable
      <
        volScalarField,
        phasePairKey,
        phasePairKey::hash
      >
      KdTable;
    typedef
      HashPtrTable
      <
        volScalarField,
        phasePairKey,
        phasePairKey::hash
      >
      VmTable;
    typedef
      HashPtrTable
      <
        fvVectorMatrix,
        word,
        string::hash
      >
      momentumTransferTable;
    typedef
      HashPtrTable
      <
        fvScalarMatrix,
        word,
        string::hash
      >
      heatTransferTable;
    typedef
      HashPtrTable
      <
        fvScalarMatrix,
        word,
        string::hash
      >
      massTransferTable;
    typedef PtrListDictionary<phaseModel> phaseModelList;
protected:
  // Protected typedefs
    typedef
      HashTable<dictionary, phasePairKey, phasePairKey::hash>
      dictTable;
    typedef
      HashTable<autoPtr<phasePair>, phasePairKey, phasePairKey::hash>
      phasePairTable;
    typedef
      HashTable<autoPtr<blendingMethod>, word, word::hash>
      blendingMethodTable;
    typedef
      HashTable
      <
        autoPtr<surfaceTensionModel>,
        phasePairKey,
        phasePairKey::hash
      >
      surfaceTensionModelTable;
    typedef
      HashTable
      <
        autoPtr<aspectRatioModel>,
        phasePairKey,
        phasePairKey::hash
      >
      aspectRatioModelTable;
  // Protected data
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Phase models
    phaseModelList phaseModels_;
    //- Phase pairs
    phasePairTable phasePairs_;
    //- Total volumetric flux
    surfaceScalarField phi_;
    //- Rate of change of pressure
    volScalarField dpdt_;
    //- Optional MRF zones
    IOMRFZoneList MRF_;
    //- Optional FV-options
    mutable fv::IOoptionList fvOptions_;
    //- Blending methods
    blendingMethodTable blendingMethods_;
    // Sub Models
      //- Surface tension models
      surfaceTensionModelTable surfaceTensionModels_;
      //- Aspect ratio models
      aspectRatioModelTable aspectRatioModels_;
  // Protected member functions
    //- Calculate and return the mixture flux
    tmp<surfaceScalarField> calcPhi
    (
      const phaseModelList& phaseModels
    ) const;
    //- Generate pairs
    void generatePairs
    (
      const dictTable& modelDicts
    );
    //- Generate pairs and sub-model tables
    template<class modelType>
    void createSubModels
    (
      const dictTable& modelDicts,
      HashTable
      <
        autoPtr<modelType>,
        phasePairKey,
        phasePairKey::hash
      >& models
    );
    //- Generate pairs and sub-model tables
    template<class modelType>
    void generatePairsAndSubModels
    (
      const word& modelName,
      HashTable
      <
        autoPtr<modelType>,
        phasePairKey,
        phasePairKey::hash
      >& models
    );
    //- Generate pairs and blended sub-model tables
    template<class modelType>
    void generatePairsAndSubModels
    (
      const word& modelName,
      HashTable
      <
        autoPtr<BlendedInterfacialModel<modelType> >,
        phasePairKey,
        phasePairKey::hash
      >& models
    );
    //- Generate pairs and per-phase sub-model tables
    template<class modelType>
    void generatePairsAndSubModels
    (
      const word& modelName,
      HashTable
      <
        HashTable<autoPtr<modelType> >,
        phasePairKey,
        phasePairKey::hash
      >& models
    );
public:
  //- Runtime type information
  TYPE_NAME("phaseSystem");
  //- Default name of the phase properties dictionary
  static const word propertiesName;
  // Constructors
    //- Construct from fvMesh
    phaseSystem(const fvMesh& mesh);
  //- Destructor
  virtual ~phaseSystem();
  // Member Functions
    //- Constant access the mesh
    inline const fvMesh& mesh() const;
    //- Constant access the phase models
    inline const phaseModelList& phases() const;
    //- Access the phase models
    inline phaseModelList& phases();
    //- Constant access the phase pairs
    inline const phasePairTable& phasePairs() const;
    //- Return the mixture density
    tmp<volScalarField> rho() const;
    //- Return the mixture velocity
    tmp<volVectorField> U() const;
    //- Constant access the mixture flux
    inline const surfaceScalarField& phi() const;
    //- Access the mixture flux
    inline surfaceScalarField& phi();
    //- Constant access the rate of change of the pressure
    inline const volScalarField& dpdt() const;
    //- Access the rate of change of the pressure
    inline volScalarField& dpdt();
    //- Return the surface tension coefficient
    tmp<volScalarField> sigma(const phasePairKey& key) const;
    //- Return MRF zones
    inline const IOMRFZoneList& MRF() const;
    //- Optional FV-options
    inline fv::IOoptionList& fvOptions() const;
    //- Access a sub model between a phase pair
    template <class modelType>
    const modelType& lookupSubModel(const phasePair& key) const;
    //- Access a sub model between two phases
    template <class modelType>
    const modelType& lookupSubModel
    (
      const phaseModel& dispersed,
      const phaseModel& continuous
    ) const;
    //- Solve for the phase fractions
    virtual void solve();
    //- Correct the fluid properties other than the thermo and turbulence
    virtual void correct();
    //- Correct the kinematics
    virtual void correctKinematics();
    //- Correct the thermodynamics
    virtual void correctThermo();
    //- Correct the turbulence
    virtual void correctTurbulence();
    //- Correct the energy transport e.g. alphat
    virtual void correctEnergyTransport();
    //- Read base phaseProperties dictionary
    virtual bool read();
};
}  // namespace mousse

// Member Functions 
inline const mousse::fvMesh& mousse::phaseSystem::mesh() const
{
  return mesh_;
}
inline const mousse::phaseSystem::phaseModelList&
mousse::phaseSystem::phases() const
{
  return phaseModels_;
}
inline mousse::phaseSystem::phaseModelList&
mousse::phaseSystem::phases()
{
  return phaseModels_;
}
inline const mousse::phaseSystem::phasePairTable&
mousse::phaseSystem::phasePairs() const
{
  return phasePairs_;
}
inline const mousse::surfaceScalarField& mousse::phaseSystem::phi() const
{
  return phi_;
}
inline mousse::surfaceScalarField& mousse::phaseSystem::phi()
{
  return phi_;
}
inline const mousse::volScalarField& mousse::phaseSystem::dpdt() const
{
  return dpdt_;
}
inline mousse::volScalarField& mousse::phaseSystem::dpdt()
{
  return dpdt_;
}
inline const mousse::IOMRFZoneList& mousse::phaseSystem::MRF() const
{
  return MRF_;
}
inline mousse::fv::IOoptionList& mousse::phaseSystem::fvOptions() const
{
  return fvOptions_;
}
#ifdef NoRepository
#   include "phase_system_templates.cpp"
#endif
#endif
