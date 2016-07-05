// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interface_composition_model.hpp"
#include "phase_pair.hpp"
#include "rho_thermo.hpp"
// Selector 
mousse::autoPtr<mousse::interfaceCompositionModel>
mousse::interfaceCompositionModel::New
(
  const dictionary& dict,
  const phasePair& pair
)
{
  word interfaceCompositionModelType
  {
    word(dict.lookup("type"))
    + "<"
    + pair.phase1().thermo().type()
    + ","
    + pair.phase2().thermo().type()
    + ">"
  };
  Info << "Selecting interfaceCompositionModel for "
    << pair << ": " << interfaceCompositionModelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(interfaceCompositionModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("interfaceCompositionModel::New")
      << "Unknown interfaceCompositionModelType type "
      << interfaceCompositionModelType << endl << endl
      << "Valid interfaceCompositionModel types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, pair);
}
