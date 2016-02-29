#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TDAMPING_MODELS_TRELAXATION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TMPPIC_TDAMPING_MODELS_TRELAXATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DampingModels::Relaxation
// Description
//   Relaxation collisional damping model.
//   Particle velocities are relaxed towards the local mean over a time-scale.
//   Reference:
//   \verbatim
//     "An improved collision damping time for MP-PIC calculations of dense
//     particle flows with applications to polydisperse sedimenting beds and
//     colliding particle jets"
//     P O'Rourke and D Snider
//     Chemical Engineering Science
//     Volume 65, Issue 22, Pages 6014-6028, November 2010
//   \endverbatim
// SourceFiles
//   _relaxation.cpp
#include "_damping_model.hpp"
namespace mousse
{
namespace DampingModels
{
template<class CloudType>
class Relaxation
:
  public DampingModel<CloudType>
{
private:
  // Private data
    //- Velocity average
    const AveragingMethod<vector>* uAverage_;
    //- Reciprocal of the time scale average
    autoPtr<AveragingMethod<scalar> > oneByTimeScaleAverage_;
public:
  //- Runtime type information
  TYPE_NAME("relaxation");
  // Constructors
    //- Construct from components
    Relaxation(const dictionary& dict, CloudType& owner);
    //- Construct copy
    Relaxation(const Relaxation<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<DampingModel<CloudType> > clone() const
    {
      return autoPtr<DampingModel<CloudType> >
      (
        new Relaxation<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~Relaxation();
  //- Member Functions
    //- Calculate the damping time scales
    virtual void cacheFields(const bool store);
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
};
}  // namespace DampingModels
}  // namespace mousse
#ifdef NoRepository
#   include "_relaxation.cpp"
#endif
#endif
