// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "chemistry_reader.hpp"
template<class ThermoType>
mousse::autoPtr<mousse::chemistryReader<ThermoType> >
mousse::chemistryReader<ThermoType>::New
(
  const dictionary& thermoDict,
  speciesTable& species
)
{
  // Let the chemistry reader type default to CHEMKIN
  // for backward compatibility
  word chemistryReaderTypeName("chemkinReader");
  // otherwise use the specified reader
  thermoDict.readIfPresent("chemistryReader", chemistryReaderTypeName);
  Info<< "Selecting chemistryReader " << chemistryReaderTypeName << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(chemistryReaderTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "chemistryReader::New(const dictionary&, speciesTable&)"
    )   << "Unknown chemistryReader type "
      << chemistryReaderTypeName << nl << nl
      << "Valid chemistryReader types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<chemistryReader<ThermoType> >
  (
    cstrIter()(thermoDict, species)
  );
}
