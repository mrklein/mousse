// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_solid_chemistry_model.hpp"
// Selectors
mousse::autoPtr<mousse::basicSolidChemistryModel> mousse::basicSolidChemistryModel::
New
(
  const fvMesh& mesh,
  const word& phaseName
)
{
  IOdictionary chemistryDict
  (
    IOobject
    (
      IOobject::groupName("chemistryProperties", phaseName),
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    )
  );
  const dictionary& chemistryTypeDict
  (
    chemistryDict.subDict("chemistryType")
  );
  Info<< "Selecting chemistry type " << chemistryTypeDict << endl;
  const int nCmpt = 13;
  const char* cmptNames[nCmpt] =
  {
    "chemistrySolver",
    "chemistryThermo",
    "baseChemistry",
    "transport",
    "thermo",
    "equationOfState",
    "specie",
    "energy",
    "transport",
    "thermo",
    "equationOfState",
    "specie",
    "energy"
  };
  IOdictionary thermoDict
  (
    IOobject
    (
      basicThermo::dictName,
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    )
  );
  const dictionary& solidThermoTypeDict(thermoDict.subDict("thermoType"));
  word solidThermoTypeName
  (
    word(solidThermoTypeDict.lookup("transport")) + '<'
   + word(solidThermoTypeDict.lookup("thermo")) + '<'
   + word(solidThermoTypeDict.lookup("equationOfState")) + '<'
   + word(solidThermoTypeDict.lookup("specie")) + ">>,"
   + word(solidThermoTypeDict.lookup("energy")) + ">"
  );
  const dictionary& gasThermoTypeDict(thermoDict.subDict("gasThermoType"));
  word gasThermoTypeName
  (
    word(gasThermoTypeDict.lookup("transport")) + '<'
   + word(gasThermoTypeDict.lookup("thermo")) + '<'
   + word(gasThermoTypeDict.lookup("equationOfState")) + '<'
   + word(gasThermoTypeDict.lookup("specie")) + ">>,"
   + word(gasThermoTypeDict.lookup("energy")) + ">"
  );
  // Construct the name of the chemistry type from the components
  word chemistryTypeName
  (
    word(chemistryTypeDict.lookup("chemistrySolver")) + '<'
   + word(chemistryTypeDict.lookup("chemistryThermo")) + '<'
   + typeName + ','
   + solidThermoTypeName + ',' + gasThermoTypeName + ">>"
  );
  Info<< "chemistryTypeName " << chemistryTypeName << endl;
  fvMeshConstructorTable::iterator cstrIter =
    fvMeshConstructorTablePtr_->find(chemistryTypeName);
  if (cstrIter == fvMeshConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN(typeName + "::New(const mesh&)")
      << "Unknown " << typeName << " type " << nl
      << "chemistryType" << chemistryTypeDict << nl << nl
      << "Valid " << typeName << " types are:"
      << nl << nl;
    // Get the list of all the suitable chemistry packages available
    wordList validChemistryTypeNames
    (
      fvMeshConstructorTablePtr_->sortedToc()
    );
    Info<< validChemistryTypeNames << endl;
    // Build a table of the thermo packages constituent parts
    // Note: row-0 contains the names of constituent parts
    List<wordList> validChemistryTypeNameCmpts
    (
      validChemistryTypeNames.size() + 1
    );
    validChemistryTypeNameCmpts[0].setSize(nCmpt);
    FOR_ALL(validChemistryTypeNameCmpts[0], j)
    {
      validChemistryTypeNameCmpts[0][j] = cmptNames[j];
    }
    // Split the thermo package names into their constituent parts
    FOR_ALL(validChemistryTypeNames, i)
    {
      validChemistryTypeNameCmpts[i+1] = basicThermo::splitThermoName
      (
        validChemistryTypeNames[i],
        nCmpt
      );
    }
    // Print the table of available packages
    // in terms of their constituent parts
    printTable(validChemistryTypeNameCmpts, FatalError);
    FatalError<< exit(FatalError);
  }
  return autoPtr<basicSolidChemistryModel>(cstrIter()(mesh, phaseName));
}
