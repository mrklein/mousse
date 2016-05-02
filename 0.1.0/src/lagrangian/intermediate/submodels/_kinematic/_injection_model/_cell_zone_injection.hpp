#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCELL_ZONE_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TCELL_ZONE_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CellZoneInjection
// Description
//   Injection positions specified by a particle number density within a cell set
//   - User specifies
//    - Number density of particles in cell set (effective)
//    - Total mass to inject
//    - Initial parcel velocity
//   - Parcel diameters obtained by PDF model
//   - All parcels introduced at SOI

#include "_injection_model.hpp"
#include "distribution_model.hpp"


namespace mousse {

template<class CloudType>
class CellZoneInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    //- Name of cell zone
    const word cellZoneName_;
    //- Number density
    const scalar numberDensity_;
    //- Field of parcel positions
    List<vector> positions_;
    //- List of cell labels corresponding to injector positions
    labelList injectorCells_;
    //- List of tetFace labels corresponding to injector positions
    labelList injectorTetFaces_;
    //- List of tetPt labels corresponding to injector positions
    labelList injectorTetPts_;
    //- Field of parcel diameters
    scalarList diameters_;
    //- Initial parcel velocity
    const vector U0_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
  // Private Member Functions
    //- Set the parcel injection positions
    void setPositions(const labelList& cellZoneCells);
public:
  //- Runtime type information
  TYPE_NAME("cellZoneInjection");
  // Constructors
    //- Construct from dictionary
    CellZoneInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    CellZoneInjection(const CellZoneInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType>> clone() const
    {
      return
        autoPtr<InjectionModel<CloudType>>
        {
          new CellZoneInjection<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~CellZoneInjection();
  // Member Functions
    //- Set injector locations when mesh is updated
    virtual void updateMesh();
    //- Return the end-of-injection time
    scalar timeEnd() const;
    //- Number of parcels to introduce relative to SOI
    label parcelsToInject(const scalar time0, const scalar time1);
    //- Volume of parcels to introduce relative to SOI
    scalar volumeToInject(const scalar time0, const scalar time1);
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

#include "_cell_zone_injection.ipp"

#endif
