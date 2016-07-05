#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_REACTING_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_REACTING_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMixture
// Description
//   mousse::reactingMixture

#include "species_table.hpp"
#include "chemistry_reader.hpp"
#include "multi_component_mixture.hpp"


namespace mousse {

template<class ThermoType>
class reactingMixture
:
  public speciesTable,
  public autoPtr<chemistryReader<ThermoType>>,
  public multiComponentMixture<ThermoType>,
  public PtrList<Reaction<ThermoType>>
{
public:
  //- The type of thermo package this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    reactingMixture(const dictionary&, const fvMesh&, const word&);
    //- Disallow default bitwise copy construct
    reactingMixture(const reactingMixture&) = delete;
    //- Disallow default bitwise assignment
    reactingMixture& operator=(const reactingMixture&) = delete;
  //- Destructor
  virtual ~reactingMixture()
  {}
  // Member functions
    //- Read dictionary
    void read(const dictionary&);
    label size() const
    {
      return PtrList<Reaction<ThermoType>>::size();
    }
    Reaction<ThermoType>& operator [] (const label i)
    {
      return PtrList<Reaction<ThermoType>>::operator[](i);
    }
    const Reaction<ThermoType>& operator [] (const label i) const
    {
      return PtrList<Reaction<ThermoType>>::operator[](i);
    }
};

}  // namespace mousse

#include "reacting_mixture.ipp"

#endif
