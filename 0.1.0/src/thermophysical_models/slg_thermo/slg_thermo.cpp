// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "slg_thermo.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(SLGThermo, 0);
}
// Constructors 
mousse::SLGThermo::SLGThermo(const fvMesh& mesh, fluidThermo& thermo)
:
  regIOobject
  (
    IOobject
    (
      SLGThermo::typeName,
      mesh.polyMesh::instance(),
      mesh
    )
  ),
  thermo_(thermo),
  carrier_(NULL),
  liquids_(NULL),
  solids_(NULL)
{
  Info<< "Creating component thermo properties:" << endl;
  if (isA<basicSpecieMixture>(thermo))
  {
    basicSpecieMixture& mcThermo =
      dynamic_cast<basicSpecieMixture&>(thermo);
    carrier_ = &mcThermo;
    Info<< "    multi-component carrier - " << mcThermo.species().size()
      << " species" << endl;
  }
  else
  {
    Info<< "    single component carrier" << endl;
  }
  if (thermo.found("liquids"))
  {
    liquids_ = liquidMixtureProperties::New(thermo.subDict("liquids"));
    Info<< "    liquids - " << liquids_->components().size()
      << " components" << endl;
  }
  else
  {
    Info<< "    no liquid components" << endl;
  }
  if (thermo.found("solids"))
  {
    solids_  = solidMixtureProperties::New(thermo.subDict("solids"));
    Info<< "    solids - " << solids_->components().size()
      << " components" << endl;
  }
  else
  {
    Info<< "    no solid components" << endl;
  }
}
// Destructor 
mousse::SLGThermo::~SLGThermo()
{}
// Member Functions 
const mousse::fluidThermo& mousse::SLGThermo::thermo() const
{
  return thermo_;
}
const mousse::basicSpecieMixture& mousse::SLGThermo::carrier() const
{
  if (carrier_ == NULL)
  {
    FATAL_ERROR_IN
    (
      "const mousse::basicSpecieMixture& "
      "mousse::SLGThermo::carrier() const"
    )   << "carrier requested, but object is not allocated"
      << abort(FatalError);
  }
  return *carrier_;
}
const mousse::liquidMixtureProperties& mousse::SLGThermo::liquids() const
{
  if (!liquids_.valid())
  {
    FATAL_ERROR_IN
    (
      "const mousse::liquidMixtureProperties& "
      "mousse::SLGThermo::liquids() const"
    )   << "liquids requested, but object is not allocated"
      << abort(FatalError);
  }
  return liquids_();
}
const mousse::solidMixtureProperties& mousse::SLGThermo::solids() const
{
  if (!solids_.valid())
  {
    FATAL_ERROR_IN
    (
      "const mousse::solidMixtureProperties& "
      "mousse::SLGThermo::solids() const"
    )   << "solids requested, but object is not allocated"
      << abort(FatalError);
  }
  return solids_();
}
mousse::label mousse::SLGThermo::carrierId
(
  const word& cmptName,
  bool allowNotfound
) const
{
  FOR_ALL(carrier().species(), i)
  {
    if (cmptName == carrier_->species()[i])
    {
      return i;
    }
  }
  if (!allowNotfound)
  {
    FATAL_ERROR_IN
    (
      "mousse::label mousse::SLGThermo::carrierId(const word&, bool) const"
    )   << "Unknown carrier component " << cmptName
      << ". Valid carrier components are:" << nl
      << carrier_->species() << exit(FatalError);
  }
  return -1;
}
mousse::label mousse::SLGThermo::liquidId
(
  const word& cmptName,
  bool allowNotfound
) const
{
  FOR_ALL(liquids().components(), i)
  {
    if (cmptName == liquids_->components()[i])
    {
      return i;
    }
  }
  if (!allowNotfound)
  {
    FATAL_ERROR_IN
    (
      "mousse::label mousse::SLGThermo::liquidId(const word&, bool) const"
    )   << "Unknown liquid component " << cmptName << ". Valid liquids are:"
      << nl << liquids_->components() << exit(FatalError);
  }
  return -1;
}
mousse::label mousse::SLGThermo::solidId
(
  const word& cmptName,
  bool allowNotfound
) const
{
  FOR_ALL(solids().components(), i)
  {
    if (cmptName == solids_->components()[i])
    {
      return i;
    }
  }
  if (!allowNotfound)
  {
    FATAL_ERROR_IN
    (
      "mousse::label mousse::SLGThermo::solidId(const word&, bool) const"
    )   << "Unknown solid component " << cmptName << ". Valid solids are:"
      << nl << solids_->components() << exit(FatalError);
  }
  return -1;
}
bool mousse::SLGThermo::hasMultiComponentCarrier() const
{
  return (carrier_ != NULL);
}
bool mousse::SLGThermo::hasLiquids() const
{
  return liquids_.valid();
}
bool mousse::SLGThermo::hasSolids() const
{
  return solids_.valid();
}
