// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_dispersion_ras_model.hpp"
#include "demand_driven_data.hpp"
#include "turbulence_model.hpp"

// Protected Member Functions 
template<class CloudType>
mousse::tmp<mousse::volScalarField>
mousse::DispersionRASModel<CloudType>::kModel() const
{
  const objectRegistry& obr = this->owner().mesh();
  const word turbName =
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      this->owner().U().group()
    );
  if (obr.foundObject<turbulenceModel>(turbName))
  {
    const turbulenceModel& model =
      obr.lookupObject<turbulenceModel>(turbName);
    return model.k();
  }
  else
  {
    FATAL_ERROR_IN
    (
      "mousse::tmp<mousse::volScalarField>"
      "mousse::DispersionRASModel<CloudType>::kModel() const"
    )
      << "Turbulence model not found in mesh database" << nl
      << "Database objects include: " << obr.sortedToc()
      << abort(FatalError);
    return tmp<volScalarField>(NULL);
  }
}
template<class CloudType>
mousse::tmp<mousse::volScalarField>
mousse::DispersionRASModel<CloudType>::epsilonModel() const
{
  const objectRegistry& obr = this->owner().mesh();
  const word turbName =
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      this->owner().U().group()
    );
  if (obr.foundObject<turbulenceModel>(turbName))
  {
    const turbulenceModel& model =
      obr.lookupObject<turbulenceModel>(turbName);
    return model.epsilon();
  }
  else
  {
    FATAL_ERROR_IN
    (
      "mousse::tmp<mousse::volScalarField>"
      "mousse::DispersionRASModel<CloudType>::epsilonModel() const"
    )
      << "Turbulence model not found in mesh database" << nl
      << "Database objects include: " << obr.sortedToc()
      << abort(FatalError);
    return tmp<volScalarField>(NULL);
  }
}
// Constructors 
template<class CloudType>
mousse::DispersionRASModel<CloudType>::DispersionRASModel
(
  const dictionary&,
  CloudType& owner
)
:
  DispersionModel<CloudType>(owner),
  kPtr_(NULL),
  ownK_(false),
  epsilonPtr_(NULL),
  ownEpsilon_(false)
{}
template<class CloudType>
mousse::DispersionRASModel<CloudType>::DispersionRASModel
(
  const DispersionRASModel<CloudType>& dm
)
:
  DispersionModel<CloudType>(dm),
  kPtr_(dm.kPtr_),
  ownK_(dm.ownK_),
  epsilonPtr_(dm.epsilonPtr_),
  ownEpsilon_(dm.ownEpsilon_)
{
  dm.ownK_ = false;
  dm.ownEpsilon_ = false;
}
// Destructor 
template<class CloudType>
mousse::DispersionRASModel<CloudType>::~DispersionRASModel()
{
  cacheFields(false);
}
// Member Functions 
template<class CloudType>
void mousse::DispersionRASModel<CloudType>::cacheFields(const bool store)
{
  if (store)
  {
    tmp<volScalarField> tk = this->kModel();
    if (tk.isTmp())
    {
      kPtr_ = tk.ptr();
      ownK_ = true;
    }
    else
    {
      kPtr_ = tk.operator->();
      ownK_ = false;
    }
    tmp<volScalarField> tepsilon = this->epsilonModel();
    if (tepsilon.isTmp())
    {
      epsilonPtr_ = tepsilon.ptr();
      ownEpsilon_ = true;
    }
    else
    {
      epsilonPtr_ = tepsilon.operator->();
      ownEpsilon_ = false;
    }
  }
  else
  {
    if (ownK_ && kPtr_)
    {
      deleteDemandDrivenData(kPtr_);
      ownK_ = false;
    }
    if (ownEpsilon_ && epsilonPtr_)
    {
      deleteDemandDrivenData(epsilonPtr_);
      ownEpsilon_ = false;
    }
  }
}
template<class CloudType>
void mousse::DispersionRASModel<CloudType>::write(Ostream& os) const
{
  DispersionModel<CloudType>::write(os);
  os.writeKeyword("ownK") << ownK_ << token::END_STATEMENT << endl;
  os.writeKeyword("ownEpsilon") << ownEpsilon_ << token::END_STATEMENT
    << endl;
}
