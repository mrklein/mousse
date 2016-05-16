// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wide_band_absorption_emission.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(wideBandAbsorptionEmission, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  absorptionEmissionModel,
  wideBandAbsorptionEmission,
  dictionary
);

}
}


// Constructors 
mousse::radiation::wideBandAbsorptionEmission::wideBandAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel{dict, mesh},
  coeffsDict_{dict.subDict(typeName + "Coeffs")},
  speciesNames_{0},
  specieIndex_{label{0}},
  lookUpTable_
  {
    fileName(coeffsDict_.lookup("lookUpTableFileName")),
    mesh.time().constant(),
    mesh
  },
  thermo_{mesh.lookupObject<fluidThermo>(basicThermo::dictName)},
  Yj_{nSpecies_},
  totalWaveLength_{0}
{
  label nBand = 0;
  const dictionary& functionDicts = dict.subDict(typeName +"Coeffs");
  FOR_ALL_CONST_ITER(dictionary, functionDicts, iter) {
    // safety:
    if (!iter().isDict()) {
      continue;
    }
    const dictionary& dict = iter().dict();
    dict.lookup("bandLimits") >> iBands_[nBand];
    dict.lookup("EhrrCoeff") >> iEhrrCoeffs_[nBand];
    totalWaveLength_ += iBands_[nBand][1] - iBands_[nBand][0];
    label nSpec = 0;
    const dictionary& specDicts = dict.subDict("species");
    FOR_ALL_CONST_ITER(dictionary, specDicts, iter) {
      const word& key = iter().keyword();
      if (nBand == 0) {
        speciesNames_.insert(key, nSpec);
      } else {
        if (!speciesNames_.found(key)) {
          FATAL_ERROR_IN
          (
            "mousse::radiation::wideBandAbsorptionEmission(const"
            "dictionary& dict, const fvMesh& mesh)"
          )
          << "specie: " << key << "is not in all the bands"
          << nl << exit(FatalError);
        }
      }
      coeffs_[nBand][nSpec].initialise(specDicts.subDict(key));
      nSpec++;
    }
    nBand++;
  }
  nBands_ = nBand;
  // Check that all the species on the dictionary are present in the
  // look-up table  and save the corresponding indices of the look-up table
  label j = 0;
  FOR_ALL_CONST_ITER(HashTable<label>, speciesNames_, iter) {
    if (lookUpTable_.found(iter.key())) {
      label index = lookUpTable_.findFieldIndex(iter.key());
      Info << "specie: " << iter.key() << " found in look-up table "
        << " with index: " << index << endl;
      specieIndex_[iter()] = index;
    } else if (mesh.foundObject<volScalarField>(iter.key())) {
      volScalarField& Y =
        const_cast<volScalarField&>
        (
          mesh.lookupObject<volScalarField>(iter.key())
        );
      Yj_.set(j, &Y);
      specieIndex_[iter()] = 0.0;
      j++;
      Info << "species: " << iter.key() << " is being solved" << endl;
    } else {
      FATAL_ERROR_IN
      (
        "radiation::wideBandAbsorptionEmission(const"
        "dictionary& dict, const fvMesh& mesh)"
      )
      << "specie: " << iter.key()
      << " is neither in look-up table : "
      << lookUpTable_.tableName() << " nor is being solved"
      << exit(FatalError);
    }
  }
}


// Destructor 
mousse::radiation::wideBandAbsorptionEmission::~wideBandAbsorptionEmission()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::wideBandAbsorptionEmission::aCont(const label bandI) const
{
  const volScalarField& T = thermo_.T();
  const volScalarField& p = thermo_.p();
  const volScalarField& ft = mesh_.lookupObject<volScalarField>("ft");
  label nSpecies = speciesNames_.size();
  tmp<volScalarField> ta
  {
    new volScalarField
    {
      {
        "a",
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh(),
      {"a", dimless/dimLength, 0.0}
    }
  };
  scalarField& a = ta().internalField();
  FOR_ALL(a, i) {
    const List<scalar>& species = lookUpTable_.lookUp(ft[i]);
    for (label n=0; n<nSpecies; n++) {
      label l = 0;
      scalar Yipi = 0.0;
      if (specieIndex_[n] != 0) {
        // moles x pressure [atm]
        Yipi = species[specieIndex_[n]]*p[i]*9.869231e-6;
      } else {
        // mass fraction from species being solved
        Yipi = Yj_[l][i];
        l++;
      }
      scalar Ti = T[i];
      const absorptionCoeffs::coeffArray& b = coeffs_[n][bandI].coeffs(T[i]);
      if (coeffs_[n][bandI].invTemp()) {
        Ti = 1.0/T[i];
      }
      a[i]+=
        Yipi*(((((b[5]*Ti + b[4])*Ti + b[3])*Ti + b[2])*Ti + b[1])*Ti + b[0]);
    }
  }
  return ta;
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::wideBandAbsorptionEmission::eCont(const label bandI) const
{
  return aCont(bandI);
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::wideBandAbsorptionEmission::ECont(const label bandI) const
{
  tmp<volScalarField> E
  {
    new volScalarField
    {
      {
        "E",
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh(),
      {"E", dimMass/dimLength/pow3(dimTime), 0.0}
    }
  };
  if (mesh().foundObject<volScalarField>("dQ")) {
    const volScalarField& dQ = mesh().lookupObject<volScalarField>("dQ");
    if (dQ.dimensions() == dimEnergy/dimTime) {
      E().internalField() =
        iEhrrCoeffs_[bandI]*dQ.internalField()
        *(iBands_[bandI][1] - iBands_[bandI][0])/totalWaveLength_/mesh_.V();
    } else if (dQ.dimensions() == dimEnergy/dimTime/dimVolume) {
      E().internalField() = iEhrrCoeffs_[bandI]*dQ.internalField()
        *(iBands_[bandI][1] - iBands_[bandI][0])/totalWaveLength_;
    } else {
      WARNING_IN
      (
        "tmp<volScalarField>"
        "radiation::wideBandAbsorptionEmission::ECont"
        "("
          "const label"
        ") const"
      )
      << "Incompatible dimensions for dQ field" << endl;
    }
  }

  return E;
}


void mousse::radiation::wideBandAbsorptionEmission::correct
(
  volScalarField& a,
  PtrList<volScalarField>& aLambda
) const
{
  a = dimensionedScalar{"zero", dimless/dimLength, 0.0};
  for (label j=0; j<nBands_; j++) {
    aLambda[j].internalField() = this->a(j);
    a.internalField() +=
      aLambda[j].internalField()*(iBands_[j][1] - iBands_[j][0])/totalWaveLength_;
  }
}
