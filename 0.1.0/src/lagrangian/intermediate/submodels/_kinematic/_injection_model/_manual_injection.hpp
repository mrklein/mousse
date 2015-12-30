// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ManualInjection
// Description
//   Manual injection
//   - User specifies
//    - Total mass to inject
//    - Parcel positions in file \c positionsFile
//    - Initial parcel velocity
//   - Parcel diameters obtained by distribution model
//   - All parcels introduced at SOI
// SourceFiles
//   _manual_injection.cpp
#ifndef _manual_injection_hpp_
#define _manual_injection_hpp_
#include "_injection_model.hpp"
#include "distribution_model.hpp"
#include "switch.hpp"
namespace mousse
{
template<class CloudType>
class ManualInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    //- Name of file containing positions data
    const word positionsFile_;
    //- Field of parcel positions
    vectorIOField positions_;
    //- Field of parcel diameters
    scalarList diameters_;
    //- List of cell labels corresponding to injector positions
    labelList injectorCells_;
    //- List of tetFace labels corresponding to injector positions
    labelList injectorTetFaces_;
    //- List of tetPt labels corresponding to injector positions
    labelList injectorTetPts_;
    //- Initial parcel velocity
    const vector U0_;
    //- Parcel size distribution model
    const autoPtr<distributionModels::distributionModel> sizeDistribution_;
    //- Flag to suppress errors if particle injection site is out-of-bounds
    Switch ignoreOutOfBounds_;
public:
  //- Runtime type information
  TypeName("manualInjection");
  // Constructors
    //- Construct from dictionary
    ManualInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ManualInjection(const ManualInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new ManualInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ManualInjection();
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
#ifdef NoRepository
#   include "_manual_injection.cpp"
#endif
#endif
