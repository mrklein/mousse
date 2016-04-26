// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grey_mean_solid_absorption_emission.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "unit_conversion.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(greyMeanSolidAbsorptionEmission, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  absorptionEmissionModel,
  greyMeanSolidAbsorptionEmission,
  dictionary
);

}
}


// Private Member Functions
mousse::tmp<mousse::scalarField> mousse::radiation::
greyMeanSolidAbsorptionEmission::X(const word specie) const
{
  const volScalarField& T = thermo_.T();
  const volScalarField& p = thermo_.p();
  tmp<scalarField> tXj{new scalarField(T.internalField().size(), 0.0)};
  scalarField& Xj = tXj();
  tmp<scalarField> tRhoInv{new scalarField(T.internalField().size(), 0.0)};
  scalarField& rhoInv = tRhoInv();
  FOR_ALL(mixture_.Y(), specieI) {
    const scalarField& Yi = mixture_.Y()[specieI];
    FOR_ALL(rhoInv, iCell) {
      rhoInv[iCell] += Yi[iCell]/mixture_.rho(specieI, p[iCell], T[iCell]);
    }
  }
  const scalarField& Yj = mixture_.Y(specie);
  const label mySpecieI = mixture_.species()[specie];
  FOR_ALL(Xj, iCell) {
    Xj[iCell] = Yj[iCell]/mixture_.rho(mySpecieI, p[iCell], T[iCell]);
  }
  return (Xj/rhoInv);
}


// Constructors 

mousse::radiation::greyMeanSolidAbsorptionEmission::
greyMeanSolidAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel{dict, mesh},
  coeffsDict_{(dict.subDict(typeName + "Coeffs"))},
  thermo_{mesh.lookupObject<solidThermo>(basicThermo::dictName)},
  speciesNames_{0},
  mixture_{dynamic_cast<const basicSpecieMixture&>(thermo_)},
  solidData_{mixture_.Y().size()}
{
  if (!isA<basicSpecieMixture>(thermo_)) {
    FATAL_ERROR_IN
    (
      "radiation::greyMeanSolidAbsorptionEmission::"
      "greyMeanSolidAbsorptionEmission"
      "("
        "const dictionary&, "
        "const fvMesh&"
      ")"
    )
    << "Model requires a multi-component thermo package"
    << abort(FatalError);
  }
  label nFunc = 0;
  const dictionary& functionDicts = dict.subDict(typeName + "Coeffs");
  FOR_ALL_CONST_ITER(dictionary, functionDicts, iter) {
    // safety:
    if (!iter().isDict()) {
      continue;
    }
    const word& key = iter().keyword();
    if (!mixture_.contains(key)) {
      WARNING_IN
      (
        "greyMeanSolidAbsorptionEmission::"
        "greyMeanSolidAbsorptionEmission "
        "("
        "   const dictionary& dict,"
        "   const fvMesh& mesh"
        ")"
      )
      << " specie: " << key << " is not found in the solid mixture"
      << nl
      << " specie is the mixture are:" << mixture_.species() << nl
      << nl << endl;
    }
    speciesNames_.insert(key, nFunc);
    const dictionary& dict = iter().dict();
    dict.lookup("absorptivity") >> solidData_[nFunc][absorptivity];
    dict.lookup("emissivity") >> solidData_[nFunc][emissivity];
    nFunc++;
  }
}


// Destructor 
mousse::radiation::greyMeanSolidAbsorptionEmission::
~greyMeanSolidAbsorptionEmission()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::greyMeanSolidAbsorptionEmission::
calc(const label propertyId) const
{
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
      {"a", dimless/dimLength, 0.0},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  scalarField& a = ta().internalField();
  FOR_ALL_CONST_ITER(HashTable<label>, speciesNames_, iter) {
    if (mixture_.contains(iter.key())) {
      a += solidData_[iter()][propertyId]*X(iter.key());
    }
  }
  ta().correctBoundaryConditions();
  return ta;
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::greyMeanSolidAbsorptionEmission::eCont
(
  const label /*bandI*/
) const
{
 return calc(emissivity);
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::greyMeanSolidAbsorptionEmission::aCont
(
  const label /*bandI*/
) const
{
 return calc(absorptivity);
}

