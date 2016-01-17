// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackingModels::Explicit
// Description
//   Explicit model for applying an inter-particle stress to the particles.
//   The inter-particle stress is calculated using current particle locations.
//   This force is then applied only to the particles that are moving towards
//   regions of close pack. The resulting velocity change is limited using an
//   abtracted correction velocity limiter.
//   Reference:
//   \verbatim
//     "An Incompressible Three-Dimensional Multiphase Particle-in-Cell Model
//     for Dense Particle Flows"
//     D Snider
//     Journal of Computational Physics
//     Volume 170, Issue 2, Pages 523-549, July 2001
//   \endverbatim
// SourceFiles
//   explicit.cpp
#ifndef _explicit_hpp_
#define _explicit_hpp_
#include "_packing_model.hpp"
namespace mousse
{
namespace PackingModels
{
template<class CloudType>
class Explicit
:
  public PackingModel<CloudType>
{
private:
  //- Private data
    //- Volume fraction average
    const AveragingMethod<scalar>* volumeAverage_;
    //- Velocity average
    const AveragingMethod<vector>* uAverage_;
    //- Stress average field
    autoPtr<AveragingMethod<scalar> > stressAverage_;
    //- Correction limiter
    autoPtr<CorrectionLimitingMethod> correctionLimiting_;
public:
  //- Runtime type information
  TYPE_NAME("explicit");
  // Constructors
    //- Construct from components
    Explicit(const dictionary& dict, CloudType& owner);
    //- Construct copy
    Explicit(const Explicit<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<PackingModel<CloudType> > clone() const
    {
      return autoPtr<PackingModel<CloudType> >
      (
        new Explicit<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~Explicit();
  // Member Functions
    //- Calculate the inter particles stresses
    virtual void cacheFields(const bool store);
    //- Calculate the velocity correction
    virtual vector velocityCorrection
    (
      typename CloudType::parcelType& p,
      const scalar deltaT
    ) const;
};
}  // namespace PackingModels
}  // namespace mousse
#ifdef NoRepository
#   include "_explicit.cpp"
#endif
#endif
