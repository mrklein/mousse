// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "absorption_emission_model.hpp"


// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(absorptionEmissionModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(absorptionEmissionModel, dictionary);

}
}


// Constructors 
mousse::radiation::absorptionEmissionModel::absorptionEmissionModel
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  dict_{dict},
  mesh_{mesh}
{}


// Destructor   
mousse::radiation::absorptionEmissionModel::~absorptionEmissionModel()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::a(const label bandI) const
{
  return aDisp(bandI) + aCont(bandI);
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::aCont(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "aCont",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimless/dimLength, 0.0}
      }
    };
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::aDisp(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "aDisp",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimless/dimLength, 0.0}
      }
    };
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::e(const label bandI) const
{
  return eDisp(bandI) + eCont(bandI);
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::eCont(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "eCont",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimless/dimLength, 0.0}
      }
    };
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::eDisp(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "eDisp",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimless/dimLength, 0.0}
      }
    };
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::E(const label bandI) const
{
  return EDisp(bandI) + ECont(bandI);
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::ECont(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "ECont",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimMass/dimLength/pow3(dimTime), 0.0}
      }
    };
}


mousse::tmp<mousse::volScalarField>
mousse::radiation::absorptionEmissionModel::EDisp(const label /*bandI*/) const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        // IOobject
        {
          "EDisp",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh_,
        {"zero", dimMass/dimLength/pow3(dimTime), 0.0}
      }
    };
}


mousse::label mousse::radiation::absorptionEmissionModel::nBands() const
{
  return pTraits<label>::one;
}


const mousse::Vector2D<mousse::scalar>&
mousse::radiation::absorptionEmissionModel::bands(const label /*n*/) const
{
  return Vector2D<scalar>::one;
}


bool mousse::radiation::absorptionEmissionModel::isGrey() const
{
  return false;
}


void mousse::radiation::absorptionEmissionModel::correct
(
  volScalarField& a,
  PtrList<volScalarField>& aj
) const
{
  a = this->a();
  aj[0] =  a;
}

