// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sub_model_base.hpp"
// Protected Member Functions 
bool mousse::subModelBase::subModelBase::inLine() const
{
  return (modelName_ != word::null);
}
// Constructors 
mousse::subModelBase::subModelBase(dictionary& properties)
:
  modelName_(word::null),
  properties_(properties),
  dict_(dictionary::null),
  baseName_(word::null),
  modelType_(word::null),
  coeffDict_(dictionary::null)
{}
mousse::subModelBase::subModelBase
(
  dictionary& properties,
  const dictionary& dict,
  const word& baseName,
  const word& modelType,
  const word& dictExt
)
:
  modelName_(word::null),
  properties_(properties),
  dict_(dict),
  baseName_(baseName),
  modelType_(modelType),
  coeffDict_(dict.subDict(modelType + dictExt))
{}
mousse::subModelBase::subModelBase
(
  const word& modelName,
  dictionary& properties,
  const dictionary& dict,
  const word& baseName,
  const word& modelType
)
:
  modelName_(modelName),
  properties_(properties),
  dict_(dict),
  baseName_(baseName),
  modelType_(modelType),
  coeffDict_(dict)
{}
mousse::subModelBase::subModelBase(const subModelBase& smb)
:
  modelName_(smb.modelName_),
  properties_(smb.properties_),
  dict_(smb.dict_),
  baseName_(smb.baseName_),
  modelType_(smb.modelType_),
  coeffDict_(smb.coeffDict_)
{}
// Destructor 
mousse::subModelBase::~subModelBase()
{}
// Member Functions 
const mousse::word& mousse::subModelBase::modelName() const
{
  return modelName_;
}
const mousse::dictionary& mousse::subModelBase::dict() const
{
  return dict_;
}
const mousse::word& mousse::subModelBase::baseName() const
{
  return baseName_;
}
const mousse::word& mousse::subModelBase::modelType() const
{
  return modelType_;
}
const mousse::dictionary& mousse::subModelBase::coeffDict() const
{
  return coeffDict_;
}
const mousse::dictionary& mousse::subModelBase::properties() const
{
  return properties_;
}
bool mousse::subModelBase::defaultCoeffs(const bool printMsg) const
{
  bool def = coeffDict_.lookupOrDefault<bool>("defaultCoeffs", false);
  if (printMsg && def)
  {
    Info<< incrIndent;
    Info<< indent << "Employing default coefficients" << endl;
    Info<< decrIndent;
  }
  return def;
}
bool mousse::subModelBase::active() const
{
  return true;
}
void mousse::subModelBase::cacheFields(const bool)
{}
bool mousse::subModelBase::outputTime() const
{
  return active();
}
void mousse::subModelBase::write(Ostream& os) const
{
  os  << coeffDict_;
}
