// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SprayCloud
// Description
//   Templated base class for spray cloud
//   - sub-models:
//    - atomization model
//    - break-up model
#ifndef _spray_cloud_hpp_
#define _spray_cloud_hpp_

#include "spray_cloud.hpp"

namespace mousse
{
// Forward declaration of classes
template<class CloudType>
class AtomizationModel;
template<class CloudType>
class BreakupModel;
template<class CloudType>
class SprayCloud
:
  public CloudType,
  public sprayCloud
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef SprayCloud<CloudType> sprayCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<SprayCloud<CloudType>> cloudCopyPtr_;
    //- Average parcel mass
    scalar averageParcelMass_;
protected:
  // Protected data
    // References to the cloud sub-models
      //- Atomization model
      autoPtr<AtomizationModel<SprayCloud<CloudType>>>
        atomizationModel_;
      //- Break-up model
      autoPtr<BreakupModel<SprayCloud<CloudType>>> breakupModel_;
  // Protected Member Functions
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Reset state of cloud
      void cloudReset(SprayCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    SprayCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const dimensionedVector& g,
      const SLGThermo& thermo,
      bool readFields = true
    );
    //- Copy constructor with new name
    SprayCloud(SprayCloud<CloudType>& c, const word& name);
    //- Copy constructor with new name - creates bare cloud
    SprayCloud
    (
      const fvMesh& mesh,
      const word& name,
      const SprayCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return autoPtr<Cloud<parcelType>>
      (
        new SprayCloud(*this, name)
      );
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return autoPtr<Cloud<parcelType>>
      (
        new SprayCloud(this->mesh(), name, *this)
      );
    }
    //- Disallow default bitwise copy construct
    SprayCloud(const SprayCloud&) = delete;
    //- Disallow default bitwise assignment
    SprayCloud& operator=(const SprayCloud&) = delete;
  //- Destructor
  virtual ~SprayCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const SprayCloud& cloudCopy() const;
      //- Return const-access to the average parcel mass
      inline scalar averageParcelMass() const;
      // Sub-models
        //- Return const-access to the atomization model
        inline const AtomizationModel<SprayCloud<CloudType>>&
          atomization() const;
        //- Return reference to the atomization model
        inline AtomizationModel<SprayCloud<CloudType>>& atomization();
        //- Return const-access to the breakup model
        inline const BreakupModel<SprayCloud<CloudType>>&
          breakup() const;
        //- Return reference to the breakup model
        inline BreakupModel<SprayCloud<CloudType>>& breakup();
    // Cloud evolution functions
      //- Set parcel thermo properties
      void setParcelThermoProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt
      );
      //- Check parcel properties
      void checkParcelProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt,
        const bool fullyDescribed
      );
      //- Store the current cloud state
      void storeState();
      //- Reset the current cloud to the previously stored state
      void restoreState();
      //- Evolve the spray (inject, move)
      void evolve();
    // I-O
      //- Print cloud information
      void info();
};
}  // namespace mousse

// Member Functions 
template<class CloudType>
inline const mousse::SprayCloud<CloudType>&
mousse::SprayCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const mousse::AtomizationModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::atomization() const
{
  return atomizationModel_;
}
template<class CloudType>
inline mousse::AtomizationModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::atomization()
{
  return atomizationModel_();
}
template<class CloudType>
inline const mousse::BreakupModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::breakup() const
{
  return breakupModel_;
}
template<class CloudType>
inline mousse::BreakupModel<mousse::SprayCloud<CloudType> >&
mousse::SprayCloud<CloudType>::breakup()
{
  return breakupModel_();
}
template<class CloudType>
inline mousse::scalar mousse::SprayCloud<CloudType>::averageParcelMass() const
{
  return averageParcelMass_;
}
#ifdef NoRepository
#   include "_spray_cloud.cpp"
#endif
#endif
