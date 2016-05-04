#ifndef LAGRANGIAN_DSMC_PARCELS_TEMPLATES_TDSMC_PARCEL_HPP_
#define LAGRANGIAN_DSMC_PARCELS_TEMPLATES_TDSMC_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DSMCParcel
// Description
//   DSMC parcel class

#include "particle.hpp"
#include "iostream.hpp"
#include "auto_ptr.hpp"
#include "contiguous.hpp"
#include "_dsmc_cloud.hpp"
#include "mathematical_constants.hpp"


namespace mousse {

template<class ParcelType> class DSMCParcel;

// Forward declaration of friend functions
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const DSMCParcel<ParcelType>&
);


template<class ParcelType>
class DSMCParcel
:
  public ParcelType
{
  // Private member data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  //- Class to hold DSMC particle constant properties
  class constantProperties
  {
    // Private data
      //- Particle mass [kg] (constant)
      scalar mass_;
      //- Particle hard sphere diameter [m] (constant)
      scalar d_;
      //- Internal degrees of freedom
      direction internalDegreesOfFreedom_;
      //- Viscosity index
      scalar omega_;
  public:
    // Constructors
      //- Null constructor, allows List of constantProperties to be
      //  created before the contents is initialised
      inline constantProperties();
      //- Constructor from dictionary
      inline constantProperties(const dictionary& dict);
    // Member functions
      //- Return const access to the particle mass [kg]
      inline scalar mass() const;
      //- Return const access to the hard sphere diameter [m]
      inline scalar d() const;
      //- Return the reference total collision cross section
      inline scalar sigmaT() const;
      //- Return the internalDegreesOfFreedom
      inline direction internalDegreesOfFreedom() const;
      //- Return the viscosity index
      inline scalar omega() const;
  };
  //- Class used to pass kinematic tracking data to the trackToFace function
  class trackingData
  :
    public particle::TrackingData<DSMCCloud<DSMCParcel<ParcelType>>>
  {
  public:
    // Constructors
      //- Construct from components
      trackingData(DSMCCloud<DSMCParcel<ParcelType>>& cloud)
      :
        particle::TrackingData<DSMCCloud<DSMCParcel<ParcelType>>>
        {
          cloud
        }
      {}
  };
protected:
  // Protected member data
    // Parcel properties
      //- Velocity of Parcel [m/s]
      vector U_;
      //- Internal energy of the Parcel, covering all non-translational
      //  degrees of freedom [J]
      scalar Ei_;
      //- Parcel type id
      label typeId_;
public:
  //- Runtime type information
  TYPE_NAME("DSMCParcel");
  friend class Cloud<ParcelType>;
  // Constructors
    //- Construct from components
    inline DSMCParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const vector& U,
      const scalar Ei,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label typeId
    );
    //- Construct from Istream
    DSMCParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct and return a clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>(new DSMCParcel<ParcelType>(*this));
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_{mesh}
      {}
      autoPtr<DSMCParcel<ParcelType>> operator()(Istream& is) const
      {
        return
          autoPtr<DSMCParcel<ParcelType>>
          {
            new DSMCParcel<ParcelType>{mesh_, is, true}
          };
      }
    };
  // Member Functions
    // Access
      //- Return type id
      inline label typeId() const;
      //- Return const access to velocity
      inline const vector& U() const;
      //- Return const access to internal energy
      inline scalar Ei() const;
    // Edit
      //- Return access to velocity
      inline vector& U();
      //- Return access to internal energy
      inline scalar& Ei();
    // Main calculation loop
      // Tracking
        //- Move the parcel
        template<class TrackData>
        bool move(TrackData& td, const scalar trackTime);
    // Patch interactions
      //- Overridable function to handle the particle hitting a patch
      //  Executed before other patch-hitting functions
      template<class TrackData>
      bool hitPatch
      (
        const polyPatch&,
        TrackData& td,
        const label patchI,
        const scalar trackFraction,
        const tetIndices& tetIs
      );
      //- Overridable function to handle the particle hitting a
      //  processorPatch
      template<class TrackData>
      void hitProcessorPatch
      (
        const processorPolyPatch&,
        TrackData& td
      );
      //- Overridable function to handle the particle hitting a wallPatch
      template<class TrackData>
      void hitWallPatch
      (
        const wallPolyPatch&,
        TrackData& td,
        const tetIndices&
      );
      //- Overridable function to handle the particle hitting a polyPatch
      template<class TrackData>
      void hitPatch
      (
        const polyPatch&,
        TrackData& td
      );
      //- Transform the physical properties of the particle
      //  according to the given transformation tensor
      virtual void transformProperties(const tensor& T);
      //- Transform the physical properties of the particle
      //  according to the given separation vector
      virtual void transformProperties(const vector& separation);
    // I-O
      static void readFields(Cloud<DSMCParcel<ParcelType>>& c);
      static void writeFields(const Cloud<DSMCParcel<ParcelType>>& c);
  // Ostream Operator
    friend Ostream& operator<< <ParcelType>
    (
      Ostream&,
      const DSMCParcel<ParcelType>&
    );
};

}  // namespace mousse


// Constructors 
template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::constantProperties::constantProperties()
:
  mass_{0},
  d_{0}
{}


template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& dict
)
:
  mass_{readScalar(dict.lookup("mass"))},
  d_{readScalar(dict.lookup("diameter"))},
  internalDegreesOfFreedom_
  {
    static_cast<unsigned char>(readInt(dict.lookup("internalDegreesOfFreedom")))
  },
  omega_{readScalar(dict.lookup("omega"))}
{}


template<class ParcelType>
inline mousse::DSMCParcel<ParcelType>::DSMCParcel
(
  const polyMesh& mesh,
  const vector& position,
  const vector& U,
  const scalar Ei,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId
)
:
  ParcelType{mesh, position, cellI, tetFaceI, tetPtI},
  U_{U},
  Ei_{Ei},
  typeId_{typeId}
{}


// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::mass() const
{
  return mass_;
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCParcel<ParcelType>::constantProperties::d() const
{
  return d_;
}


template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::sigmaT() const
{
  return constant::mathematical::pi*d_*d_;
}


template<class ParcelType>
inline mousse::direction
mousse::DSMCParcel<ParcelType>::constantProperties::internalDegreesOfFreedom()
const
{
  return internalDegreesOfFreedom_;
}


template<class ParcelType>
inline mousse::scalar
mousse::DSMCParcel<ParcelType>::constantProperties::omega() const
{
  return omega_;
}


// DSMCParcel Member Functions 
template<class ParcelType>
inline mousse::label mousse::DSMCParcel<ParcelType>::typeId() const
{
  return typeId_;
}


template<class ParcelType>
inline const mousse::vector& mousse::DSMCParcel<ParcelType>::U() const
{
  return U_;
}


template<class ParcelType>
inline mousse::scalar mousse::DSMCParcel<ParcelType>::Ei() const
{
  return Ei_;
}


template<class ParcelType>
inline mousse::vector& mousse::DSMCParcel<ParcelType>::U()
{
  return U_;
}


template<class ParcelType>
inline mousse::scalar& mousse::DSMCParcel<ParcelType>::Ei()
{
  return Ei_;
}


#include "_dsmc_parcel.ipp"

#endif
