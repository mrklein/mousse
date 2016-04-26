// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "foam_chemistry_reader.hpp"
#include "ifstream.hpp"
#include "add_to_run_time_selection_table.hpp"


// Private Member Functions 
template<class ThermoType>
mousse::speciesTable& mousse::foamChemistryReader<ThermoType>::setSpecies
(
  const dictionary& dict,
  speciesTable& species
)
{
  wordList s{dict.lookup("species")};
  species.transfer(s);
  return species;
}


// Constructor
template<class ThermoType>
mousse::foamChemistryReader<ThermoType>::foamChemistryReader
(
  const fileName& reactionsFileName,
  speciesTable& species,
  const fileName& thermoFileName
)
:
  chemistryReader<ThermoType>{},
  chemDict_
  {
    IFstream{fileName(reactionsFileName).expand()}()
  },
  thermoDict_
  {
    IFstream{fileName{thermoFileName}.expand()}()
  },
  speciesTable_{setSpecies(chemDict_, species)},
  speciesThermo_{thermoDict_},
  reactions_{speciesTable_, speciesThermo_, chemDict_}
{}


template<class ThermoType>
mousse::foamChemistryReader<ThermoType>::foamChemistryReader
(
  const dictionary& thermoDict,
  speciesTable& species
)
:
  chemistryReader<ThermoType>{},
  chemDict_
  {
    IFstream{fileName{thermoDict.lookup("foamChemistryFile")}.expand()}()
  },
  thermoDict_
  {
    IFstream{fileName{thermoDict.lookup("foamChemistryThermoFile")}.expand()}()
  },
  speciesThermo_{thermoDict_},
  speciesTable_{setSpecies(chemDict_, species)},
  reactions_{speciesTable_, speciesThermo_, chemDict_}
{}

