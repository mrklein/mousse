// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_chemistry_model.hpp"
#include "basic_thermo.hpp"
// Selectors
template<class ChemistryModel>
mousse::autoPtr<ChemistryModel> mousse::basicChemistryModel::New
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
  word chemistryTypeName;
  if (chemistryDict.isDict("chemistryType"))
  {
    const dictionary& chemistryTypeDict
    (
      chemistryDict.subDict("chemistryType")
    );
    Info<< "Selecting chemistry type " << chemistryTypeDict << endl;
    const int nCmpt = 7;
    const char* cmptNames[nCmpt] =
    {
      "chemistrySolver",
      "chemistryThermo",
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
        IOobject::groupName(basicThermo::dictName, phaseName),
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    );
    word thermoTypeName;
    if (thermoDict.isDict("thermoType"))
    {
      const dictionary& thermoTypeDict(thermoDict.subDict("thermoType"));
      thermoTypeName =
        word(thermoTypeDict.lookup("transport")) + '<'
       + word(thermoTypeDict.lookup("thermo")) + '<'
       + word(thermoTypeDict.lookup("equationOfState")) + '<'
       + word(thermoTypeDict.lookup("specie")) + ">>,"
       + word(thermoTypeDict.lookup("energy")) + ">";
    }
    else
    {
      FatalIOErrorIn
      (
        (ChemistryModel::typeName + "::New(const mesh&)").c_str(),
        thermoDict
      )   << "thermoType is in the old format and must be upgraded"
        << exit(FatalIOError);
    }
    // Construct the name of the chemistry type from the components
    chemistryTypeName =
      word(chemistryTypeDict.lookup("chemistrySolver")) + '<'
     + word(chemistryTypeDict.lookup("chemistryThermo")) + ','
     + thermoTypeName + ">";
    typename ChemistryModel::fvMeshConstructorTable::iterator cstrIter =
      ChemistryModel::fvMeshConstructorTablePtr_->find(chemistryTypeName);
    if (cstrIter == ChemistryModel::fvMeshConstructorTablePtr_->end())
    {
      FatalErrorIn(ChemistryModel::typeName + "::New(const mesh&)")
        << "Unknown " << ChemistryModel::typeName << " type " << nl
        << "chemistryType" << chemistryTypeDict << nl << nl
        << "Valid " << ChemistryModel ::typeName << " types are:"
        << nl << nl;
      // Get the list of all the suitable chemistry packages available
      wordList validChemistryTypeNames
      (
        ChemistryModel::fvMeshConstructorTablePtr_->sortedToc()
      );
      // Build a table of the thermo packages constituent parts
      // Note: row-0 contains the names of constituent parts
      List<wordList> validChemistryTypeNameCmpts
      (
        validChemistryTypeNames.size() + 1
      );
      validChemistryTypeNameCmpts[0].setSize(nCmpt);
      forAll(validChemistryTypeNameCmpts[0], j)
      {
        validChemistryTypeNameCmpts[0][j] = cmptNames[j];
      }
      // Split the thermo package names into their constituent parts
      forAll(validChemistryTypeNames, i)
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
    return autoPtr<ChemistryModel>(cstrIter()(mesh, phaseName));
  }
  else
  {
    chemistryTypeName =
      word(chemistryDict.lookup("chemistryType"));
    Info<< "Selecting chemistry type " << chemistryTypeName << endl;
    typename ChemistryModel::fvMeshConstructorTable::iterator cstrIter =
      ChemistryModel::fvMeshConstructorTablePtr_->find(chemistryTypeName);
    if (cstrIter == ChemistryModel::fvMeshConstructorTablePtr_->end())
    {
      FatalErrorIn(ChemistryModel::typeName + "::New(const mesh&)")
        << "Unknown " << ChemistryModel::typeName << " type "
        << chemistryTypeName << nl << nl
        << "Valid ChemistryModel types are:" << nl
        << ChemistryModel::fvMeshConstructorTablePtr_->sortedToc() << nl
        << exit(FatalError);
    }
    return autoPtr<ChemistryModel>(cstrIter()(mesh, phaseName));
  }
}
