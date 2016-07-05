#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_TREACTING_LOOKUP_TABLE_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_TREACTING_LOOKUP_TABLE_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingLookupTableInjection
// Description
//   Particle injection sources read from look-up table. Each row corresponds to
//   an injection site.
//   (
//     (x y z) (u v w) d rho mDot T cp (Y0..YN)  // injector 1
//     (x y z) (u v w) d rho mDot T cp (Y0..YN)  // injector 2
//     ...
//     (x y z) (u v w) d rho mDot T cp (Y0..YN)  // injector N
//   );
//   where:
//     x, y, z = global cartesian co-ordinates [m]
//     u, v, w = global cartesian velocity components [m/s]
//     d       = diameter [m]
//     rho     = density [kg/m3]
//     mDot    = mass flow rate [kg/m3]
//     T       = temperature [K]
//     cp      = specific heat capacity [J/kg/K]
//     Y       = list of mass fractions

#include "_injection_model.hpp"
#include "reacting_parcel_injection_data_io_list.hpp"


namespace mousse {

template<class CloudType>
class ReactingLookupTableInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    //- Name of file containing injector/parcel data
    const word inputFileName_;
    //- Injection duration - common to all injection sources
    scalar duration_;
    //- Number of parcels per injector - common to all injection sources
    const scalar parcelsPerSecond_;
    //- Flag to indicate to randomise injection positions
    bool randomise_;
    //- List of injectors
    reactingParcelInjectionDataIOList injectors_;
    //- List of cell labels corresoponding to injector positions
    labelList injectorCells_;
    //- List of tetFace labels corresoponding to injector positions
    labelList injectorTetFaces_;
    //- List of tetPt labels corresoponding to injector positions
    labelList injectorTetPts_;
public:
  //- Runtime type information
  TYPE_NAME("reactingLookupTableInjection");
  // Constructors
    //- Construct from dictionary
    ReactingLookupTableInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ReactingLookupTableInjection
    (
      const ReactingLookupTableInjection<CloudType>& im
    );
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType>> clone() const
    {
      return
        autoPtr<InjectionModel<CloudType>>
        {
          new ReactingLookupTableInjection<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~ReactingLookupTableInjection();
  // Member Functions
    //- Set injector locations when mesh is updated
    virtual void updateMesh();
    //- Return the end-of-injection time
    scalar timeEnd() const;
    //- Number of parcels to introduce relative to SOI
    virtual label parcelsToInject(const scalar time0, const scalar time1);
    //- Volume of parcels to introduce relative to SOI
    virtual scalar volumeToInject(const scalar time0, const scalar time1);
    // Injection geometry
      //- Set the injection position and owner cell, tetFace and tetPt
      virtual void setPositionAndCell
      (
        const label parcelI,
        const label nParcels,
        const scalar time,
        vector& position,
        label& cellOwner,
        label& tetFaceI,
        label& tetPtI
      );
      //- Set the parcel properties
      virtual void setProperties
      (
        const label parcelI,
        const label nParcels,
        const scalar time,
        typename CloudType::parcelType& parcel
      );
      //- Flag to identify whether model fully describes the parcel
      virtual bool fullyDescribed() const;
      //- Return flag to identify whether or not injection of parcelI is
      //  permitted
      virtual bool validInjection(const label parcelI);
};

}  // namespace mousse

#include "_reacting_lookup_table_injection.ipp"

#endif
