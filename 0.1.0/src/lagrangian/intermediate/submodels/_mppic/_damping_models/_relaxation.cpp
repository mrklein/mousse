// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_relaxation.hpp"
// Constructors 
template<class CloudType>
mousse::DampingModels::Relaxation<CloudType>::Relaxation
(
  const dictionary& dict,
  CloudType& owner
)
:
  DampingModel<CloudType>(dict, owner, typeName),
  uAverage_(NULL),
  oneByTimeScaleAverage_(NULL)
{}
template<class CloudType>
mousse::DampingModels::Relaxation<CloudType>::Relaxation
(
  const Relaxation<CloudType>& cm
)
:
  DampingModel<CloudType>(cm),
  uAverage_(NULL),
  oneByTimeScaleAverage_(cm.oneByTimeScaleAverage_->clone())
{}
// Destructor 
template<class CloudType>
mousse::DampingModels::Relaxation<CloudType>::
~Relaxation()
{}
// Member Functions 
template<class CloudType>
void mousse::DampingModels::Relaxation<CloudType>::cacheFields(const bool store)
{
  if (store)
  {
    const fvMesh& mesh = this->owner().mesh();
    const word& cloudName = this->owner().name();
    const AveragingMethod<scalar>& volumeAverage =
      mesh.lookupObject<AveragingMethod<scalar> >
      (
        cloudName + ":volumeAverage"
      );
    const AveragingMethod<scalar>& radiusAverage =
      mesh.lookupObject<AveragingMethod<scalar> >
      (
        cloudName + ":radiusAverage"
      );
    const AveragingMethod<vector>& uAverage =
      mesh.lookupObject<AveragingMethod<vector> >
      (
        cloudName + ":uAverage"
      );
    const AveragingMethod<scalar>& uSqrAverage =
      mesh.lookupObject<AveragingMethod<scalar> >
      (
        cloudName + ":uSqrAverage"
      );
    const AveragingMethod<scalar>& frequencyAverage =
      mesh.lookupObject<AveragingMethod<scalar> >
      (
        cloudName + ":frequencyAverage"
      );
    uAverage_ = &uAverage;
    oneByTimeScaleAverage_.reset
    (
      AveragingMethod<scalar>::New
      (
        IOobject
        (
          cloudName + ":oneByTimeScaleAverage",
          this->owner().db().time().timeName(),
          mesh
        ),
        this->owner().solution().dict(),
        mesh
      ).ptr()
    );
    oneByTimeScaleAverage_() =
    (
      this->timeScaleModel_->oneByTau
      (
        volumeAverage,
        radiusAverage,
        uSqrAverage,
        frequencyAverage
      )
    )();
  }
  else
  {
    uAverage_ = NULL;
    oneByTimeScaleAverage_.clear();
  }
}
template<class CloudType>
mousse::vector mousse::DampingModels::Relaxation<CloudType>::velocityCorrection
(
  typename CloudType::parcelType& p,
  const scalar deltaT
) const
{
  const tetIndices
    tetIs(p.cell(), p.tetFace(), p.tetPt(), this->owner().mesh());
  const scalar x =
    deltaT*oneByTimeScaleAverage_->interpolate(p.position(), tetIs);
  const vector u = uAverage_->interpolate(p.position(), tetIs);
  return (u - p.U())*x/(x + 2.0);
}
