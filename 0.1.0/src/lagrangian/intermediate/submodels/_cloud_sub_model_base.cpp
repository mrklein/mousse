// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud_sub_model_base.hpp"
// Constructors 
template<class CloudType>
mousse::CloudSubModelBase<CloudType>::CloudSubModelBase(CloudType& owner)
:
  subModelBase(owner.outputProperties()),
  owner_(owner)
{}
template<class CloudType>
mousse::CloudSubModelBase<CloudType>::CloudSubModelBase
(
  CloudType& owner,
  const dictionary& dict,
  const word& baseName,
  const word& modelType,
  const word& dictExt
)
:
  subModelBase
  (
    owner.outputProperties(),
    dict,
    baseName,
    modelType,
    dictExt
  ),
  owner_(owner)
{}
template<class CloudType>
mousse::CloudSubModelBase<CloudType>::CloudSubModelBase
(
  const word& modelName,
  CloudType& owner,
  const dictionary& dict,
  const word& baseName,
  const word& modelType
)
:
  subModelBase
  (
    modelName,
    owner.outputProperties(),
    dict,
    baseName,
    modelType
  ),
  owner_(owner)
{}
template<class CloudType>
mousse::CloudSubModelBase<CloudType>::CloudSubModelBase
(
  const CloudSubModelBase<CloudType>& smb
)
:
  subModelBase(smb),
  owner_(smb.owner_)
{}
// Destructor 
template<class CloudType>
mousse::CloudSubModelBase<CloudType>::~CloudSubModelBase()
{}
// Member Functions 
template<class CloudType>
const CloudType& mousse::CloudSubModelBase<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
CloudType& mousse::CloudSubModelBase<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
bool mousse::CloudSubModelBase<CloudType>::outputTime() const
{
  return
    active()
  && owner_.solution().transient()
  && owner_.db().time().outputTime();
}
template<class CloudType>
void mousse::CloudSubModelBase<CloudType>::write(Ostream& os) const
{
  os.writeKeyword("owner") << owner_.name() << token::END_STATEMENT
    << nl;
  subModelBase::write(os);
}
