// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InterfaceCompositionModel
// Description
//   Base class for interface composition models, templated on the two
//   thermodynamic models either side of the interface.
// SourceFiles
//   _interface_composition_model.cpp
#ifndef TINTERFACE_COMPOSITION_MODEL_HPP_
#define TINTERFACE_COMPOSITION_MODEL_HPP_
#include "interface_composition_model.hpp"
namespace mousse
{
class phaseModel;
class phasePair;
template <class ThermoType> class pureMixture;
template <class ThermoType> class multiComponentMixture;
template<class Thermo, class OtherThermo>
class InterfaceCompositionModel
:
  public interfaceCompositionModel
{
protected:
  // Private data
    //- Thermo
    const Thermo& thermo_;
    //- Other Thermo
    const OtherThermo& otherThermo_;
    //- Lewis number
    const dimensionedScalar Le_;
  // Private member functions
    //- Get a reference to the local thermo for a pure mixture
    template<class ThermoType>
    const typename pureMixture<ThermoType>::thermoType&
    getLocalThermo
    (
      const word& speciesName,
      const pureMixture<ThermoType>& globalThermo
    ) const;
    //- Get a reference to the local thermo for a multi component mixture
    template<class ThermoType>
    const typename multiComponentMixture<ThermoType>::thermoType&
    getLocalThermo
    (
      const word& speciesName,
      const multiComponentMixture<ThermoType>& globalThermo
    ) const;
public:
  // Constructors
    //- Construct from components
    InterfaceCompositionModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  ~InterfaceCompositionModel();
  // Member Functions
    //- Mass fraction difference between the interface and the field
    virtual tmp<volScalarField> dY
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
    //- Mass diffusivity
    virtual tmp<volScalarField> D
    (
      const word& speciesName
    ) const;
    //- Latent heat
    virtual tmp<volScalarField> L
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const;
    //- Add latent heat flow rate to total
    virtual void addMDotL
    (
      const volScalarField& K,
      const volScalarField& Tf,
      volScalarField& mDotL,
      volScalarField& mDotLPrime
    ) const;
};
}  // namespace mousse
// Instantiation for multi-component-multi-component pairs
#define MAKE_INTERFACE_COMPOSITION_TYPE\
(Type, Thermo, Comp, Mix, Phys, OtherThermo, OtherComp, OtherMix, OtherPhys)  \
                                                                              \
  typedef Thermo<Comp, SpecieMixture<Mix<Phys>>>                              \
    Type##Thermo##Comp##Mix##Phys;                                            \
                                                                              \
  typedef OtherThermo<OtherComp, OtherMix<OtherPhys>>                         \
    Type##Other##OtherThermo##OtherComp##OtherMix##OtherPhys;                 \
                                                                              \
  ADD_INTERFACE_COMPOSITION_TO_RUN_TIME_SELECTION_TABLE                       \
  (                                                                           \
    Type,                                                                     \
    Type##Thermo##Comp##Mix##Phys,                                            \
    Type##Other##OtherThermo##OtherComp##OtherMix##OtherPhys                  \
  )

// Instantiation for multi-component-single-component pairs
#define MAKE_SPECIE_INTERFACE_COMPOSITION_TYPE\
(Type, Thermo, Comp, Mix, Phys, OtherThermo, OtherComp, OtherMix, OtherPhys)  \
                                                                              \
  typedef Thermo<Comp, SpecieMixture<Mix<Phys>>>                              \
    Type##Thermo##Comp##Mix##Phys;                                            \
                                                                              \
  typedef OtherThermo<OtherComp, SpecieMixture<OtherMix<OtherPhys>>>          \
    Type##Other##OtherThermo##OtherComp##OtherMix##OtherPhys;                 \
                                                                              \
  ADD_INTERFACE_COMPOSITION_TO_RUN_TIME_SELECTION_TABLE                       \
  (                                                                           \
    Type,                                                                     \
    Type##Thermo##Comp##Mix##Phys,                                            \
    Type##Other##OtherThermo##OtherComp##OtherMix##OtherPhys                  \
  )

// Addition to the run-time selection table
#define ADD_INTERFACE_COMPOSITION_TO_RUN_TIME_SELECTION_TABLE\
(Type, Thermo, OtherThermo)                                                   \
                                                                              \
  typedef Type<Thermo, OtherThermo>                                           \
    Type##Thermo##OtherThermo;                                                \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
  (                                                                           \
    Type##Thermo##OtherThermo,                                                \
    (                                                                         \
      word(Type##Thermo##OtherThermo::typeName_()) + "<"                      \
     + word(Thermo::typeName) + ","                                           \
     + word(OtherThermo::typeName) + ">"                                      \
    ).c_str(),                                                                \
    0                                                                         \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    interfaceCompositionModel,                                                \
    Type##Thermo##OtherThermo,                                                \
    dictionary                                                                \
  )

#ifdef NoRepository
#   include "_interface_composition_model.cpp"
#endif
#endif
