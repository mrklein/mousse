// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolate_fields.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(surfaceInterpolateFields, 0);
}
// Constructors 
mousse::surfaceInterpolateFields::surfaceInterpolateFields
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  obr_(obr),
  active_(true),
  fieldSet_()
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_))
  {
    read(dict);
  }
  else
  {
    active_ = false;
    WarningIn
    (
      "surfaceInterpolateFields::surfaceInterpolateFields"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_
      << endl;
  }
}
// Destructor 
mousse::surfaceInterpolateFields::~surfaceInterpolateFields()
{}
// Member Functions 
void mousse::surfaceInterpolateFields::read(const dictionary& dict)
{
  if (active_)
  {
    dict.lookup("fields") >> fieldSet_;
  }
}
void mousse::surfaceInterpolateFields::execute()
{
  if (active_)
  {
    Info<< type() << " " << name_ << " output:" << nl;
    // Clear out any previously loaded fields
    ssf_.clear();
    svf_.clear();
    sSpheretf_.clear();
    sSymmtf_.clear();
    stf_.clear();
    interpolateFields<scalar>(ssf_);
    interpolateFields<vector>(svf_);
    interpolateFields<sphericalTensor>(sSpheretf_);
    interpolateFields<symmTensor>(sSymmtf_);
    interpolateFields<tensor>(stf_);
    Info<< endl;
  }
}
void mousse::surfaceInterpolateFields::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::surfaceInterpolateFields::timeSet()
{
  // Do nothing
}
void mousse::surfaceInterpolateFields::write()
{
  if (active_)
  {
    Info<< type() << " " << name_ << " output:" << nl;
    Info<< "    Writing interpolated surface fields to "
      << obr_.time().timeName() << endl;
    forAll(ssf_, i)
    {
      ssf_[i].write();
    }
    forAll(svf_, i)
    {
      svf_[i].write();
    }
    forAll(sSpheretf_, i)
    {
      sSpheretf_[i].write();
    }
    forAll(sSymmtf_, i)
    {
      sSymmtf_[i].write();
    }
    forAll(stf_, i)
    {
      stf_[i].write();
    }
  }
}
