// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dsmc_fields.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "dsmc_cloud.hpp"
#include "constants.hpp"
using namespace mousse::constant;
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(dsmcFields, 0);
}
// Constructors 
mousse::dsmcFields::dsmcFields
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  obr_(obr),
  active_(true)
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "dsmcFields::dsmcFields"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_ << nl
      << endl;
  }
  read(dict);
}
// Destructor 
mousse::dsmcFields::~dsmcFields()
{}
// Member Functions 
void mousse::dsmcFields::read(const dictionary& dict)
{
  if (active_)
  {
  }
}
void mousse::dsmcFields::execute()
{
  // Do nothing - only valid on write
}
void mousse::dsmcFields::end()
{
  // Do nothing - only valid on write
}
void mousse::dsmcFields::timeSet()
{
  // Do nothing - only valid on write
}
void mousse::dsmcFields::write()
{
  if (active_)
  {
    word rhoNMeanName = "rhoNMean";
    word rhoMMeanName = "rhoMMean";
    word momentumMeanName = "momentumMean";
    word linearKEMeanName = "linearKEMean";
    word internalEMeanName = "internalEMean";
    word iDofMeanName = "iDofMean";
    word fDMeanName = "fDMean";
    const volScalarField& rhoNMean = obr_.lookupObject<volScalarField>
    (
      rhoNMeanName
    );
    const volScalarField& rhoMMean = obr_.lookupObject<volScalarField>
    (
      rhoMMeanName
    );
    const volVectorField& momentumMean = obr_.lookupObject<volVectorField>
    (
      momentumMeanName
    );
    const volScalarField& linearKEMean = obr_.lookupObject<volScalarField>
    (
      linearKEMeanName
    );
    const volScalarField& internalEMean = obr_.lookupObject<volScalarField>
    (
      internalEMeanName
    );
    const volScalarField& iDofMean = obr_.lookupObject<volScalarField>
    (
      iDofMeanName
    );
    const volVectorField& fDMean = obr_.lookupObject<volVectorField>
    (
      fDMeanName
    );
    if (min(mag(rhoNMean)).value() > VSMALL)
    {
      Info<< "Calculating dsmcFields." << endl;
      Info<< "    Calculating UMean field." << endl;
      volVectorField UMean
      (
        IOobject
        (
          "UMean",
          obr_.time().timeName(),
          obr_,
          IOobject::NO_READ
        ),
        momentumMean/rhoMMean
      );
      Info<< "    Calculating translationalT field." << endl;
      volScalarField translationalT
      (
        IOobject
        (
          "translationalT",
          obr_.time().timeName(),
          obr_,
          IOobject::NO_READ
        ),
        2.0/(3.0*physicoChemical::k.value()*rhoNMean)
       *(linearKEMean - 0.5*rhoMMean*(UMean & UMean))
      );
      Info<< "    Calculating internalT field." << endl;
      volScalarField internalT
      (
        IOobject
        (
          "internalT",
          obr_.time().timeName(),
          obr_,
          IOobject::NO_READ
        ),
        (2.0/physicoChemical::k.value())*(internalEMean/iDofMean)
      );
      Info<< "    Calculating overallT field." << endl;
      volScalarField overallT
      (
        IOobject
        (
          "overallT",
          obr_.time().timeName(),
          obr_,
          IOobject::NO_READ
        ),
        2.0/(physicoChemical::k.value()*(3.0*rhoNMean + iDofMean))
       *(linearKEMean - 0.5*rhoMMean*(UMean & UMean) + internalEMean)
      );
      Info<< "    Calculating pressure field." << endl;
      volScalarField p
      (
        IOobject
        (
          "p",
          obr_.time().timeName(),
          obr_,
          IOobject::NO_READ
        ),
        physicoChemical::k.value()*rhoNMean*translationalT
      );
      const fvMesh& mesh = fDMean.mesh();
      forAll(mesh.boundaryMesh(), i)
      {
        const polyPatch& patch = mesh.boundaryMesh()[i];
        if (isA<wallPolyPatch>(patch))
        {
          p.boundaryField()[i] =
            fDMean.boundaryField()[i]
           & (patch.faceAreas()/mag(patch.faceAreas()));
        }
      }
      Info<< "    mag(UMean) max/min : "
        << max(mag(UMean)).value() << " "
        << min(mag(UMean)).value() << endl;
      Info<< "    translationalT max/min : "
        << max(translationalT).value() << " "
        << min(translationalT).value() << endl;
      Info<< "    internalT max/min : "
        << max(internalT).value() << " "
        << min(internalT).value() << endl;
      Info<< "    overallT max/min : "
        << max(overallT).value() << " "
        << min(overallT).value() << endl;
      Info<< "    p max/min : "
        << max(p).value() << " "
        << min(p).value() << endl;
      UMean.write();
      translationalT.write();
      internalT.write();
      overallT.write();
      p.write();
      Info<< "dsmcFields written." << nl << endl;
    }
    else
    {
      Info<< "Small value (" << min(mag(rhoNMean))
        << ") found in rhoNMean field. "
        << "Not calculating dsmcFields to avoid division by zero."
        << endl;
    }
  }
}
