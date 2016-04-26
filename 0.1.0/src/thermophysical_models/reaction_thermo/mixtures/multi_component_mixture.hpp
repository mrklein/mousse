#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_MULTI_COMPONENT_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_MULTI_COMPONENT_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiComponentMixture
// Description
//   mousse::multiComponentMixture

#include "basic_specie_mixture.hpp"
#include "hash_ptr_table.hpp"


namespace mousse {

template<class ThermoType>
class multiComponentMixture
:
  public basicSpecieMixture
{
  // Private data
    //- Species data
    PtrList<ThermoType> speciesData_;
    //- Temporary storage for the cell/face mixture thermo data
    mutable ThermoType mixture_;
    //- Temporary storage for the volume weighted
    //  cell/face mixture thermo data
    mutable ThermoType mixtureVol_;
  // Private Member Functions
    //- Construct the species data from the given dictionary and return the
    //  data for the first specie to initialise the mixture thermo data
    const ThermoType& constructSpeciesData(const dictionary& thermoDict);
    //- Correct the mass fractions to sum to 1
    void correctMassFractions();
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, specie names, thermo database,
    //  mesh and phase name
    multiComponentMixture
    (
      const dictionary&,
      const wordList& specieNames,
      const HashPtrTable<ThermoType>& thermoData,
      const fvMesh&,
      const word&
    );
    //- Construct from dictionary, mesh and phase name
    multiComponentMixture(const dictionary&, const fvMesh&, const word&);
    //- Disable construct as copy
    multiComponentMixture(const multiComponentMixture<ThermoType>&) = delete;
  //- Destructor
  virtual ~multiComponentMixture()
  {}
  // Member functions
    const ThermoType& cellMixture(const label celli) const;
    const ThermoType& patchFaceMixture
    (
      const label patchi,
      const label facei
    ) const;
    const ThermoType& cellVolMixture
    (
      const scalar p,
      const scalar T,
      const label celli
    ) const;
    const ThermoType& patchFaceVolMixture
    (
      const scalar p,
      const scalar T,
      const label patchi,
      const label facei
    ) const;
    //- Return the raw specie thermodynamic data
    const PtrList<ThermoType>& speciesData() const
    {
      return speciesData_;
    }
    //- Read dictionary
    void read(const dictionary&);
    //- Return thermo based on index
    inline const ThermoType& getLocalThermo(const label speciei) const
    {
      return speciesData_[speciei];
    }
};

}  // namespace mousse

#include "multi_component_mixture.ipp"

#endif
