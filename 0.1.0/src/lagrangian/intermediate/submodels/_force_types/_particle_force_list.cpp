// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_force_list.hpp"
#include "entry.hpp"
// Constructors 
template<class CloudType>
mousse::ParticleForceList<CloudType>::ParticleForceList
(
  CloudType& owner,
  const fvMesh& mesh
)
:
  PtrList<ParticleForce<CloudType> >(),
  owner_(owner),
  mesh_(mesh),
  dict_(dictionary::null),
  calcCoupled_(true),
  calcNonCoupled_(true)
{}
template<class CloudType>
mousse::ParticleForceList<CloudType>::ParticleForceList
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict,
  const bool readFields
)
:
  PtrList<ParticleForce<CloudType> >(),
  owner_(owner),
  mesh_(mesh),
  dict_(dict),
  calcCoupled_(true),
  calcNonCoupled_(true)
{
  if (readFields)
  {
    wordList modelNames(dict.toc());
    Info<< "Constructing particle forces" << endl;
    if (modelNames.size() > 0)
    {
      this->setSize(modelNames.size());
      label i = 0;
      forAllConstIter(IDLList<entry>, dict, iter)
      {
        const word& model = iter().keyword();
        if (iter().isDict())
        {
          this->set
          (
            i++,
            ParticleForce<CloudType>::New
            (
              owner,
              mesh,
              iter().dict(),
              model
            )
          );
        }
        else
        {
          this->set
          (
            i++,
            ParticleForce<CloudType>::New
            (
              owner,
              mesh,
              dict,
              model
            )
          );
        }
      }
    }
    else
    {
      Info<< "    none" << endl;
    }
  }
}
template<class CloudType>
mousse::ParticleForceList<CloudType>::ParticleForceList
(
  const ParticleForceList& pf
)
:
  PtrList<ParticleForce<CloudType> >(pf),
  owner_(pf.owner_),
  mesh_(pf.mesh_),
  dict_(pf.dict_)
{}
// Destructor 
template<class CloudType>
mousse::ParticleForceList<CloudType>::~ParticleForceList()
{}
// Member Functions 
template<class CloudType>
void mousse::ParticleForceList<CloudType>::cacheFields(const bool store)
{
  forAll(*this, i)
  {
    this->operator[](i).cacheFields(store);
  }
}
template<class CloudType>
mousse::forceSuSp mousse::ParticleForceList<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  if (calcCoupled_)
  {
    forAll(*this, i)
    {
      value += this->operator[](i).calcCoupled(p, dt, mass, Re, muc);
    }
  }
  return value;
}
template<class CloudType>
mousse::forceSuSp mousse::ParticleForceList<CloudType>::calcNonCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  forceSuSp value(vector::zero, 0.0);
  if (calcNonCoupled_)
  {
    forAll(*this, i)
    {
      value += this->operator[](i).calcNonCoupled(p, dt, mass, Re, muc);
    }
  }
  return value;
}
template<class CloudType>
mousse::scalar mousse::ParticleForceList<CloudType>::massEff
(
  const typename CloudType::parcelType& p,
  const scalar mass
) const
{
  scalar massEff = mass;
  forAll(*this, i)
  {
    massEff += this->operator[](i).massAdd(p, mass);
  }
  return massEff;
}
