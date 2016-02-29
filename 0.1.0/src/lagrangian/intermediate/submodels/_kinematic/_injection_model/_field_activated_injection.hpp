#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TFIELD_ACTIVATED_INJECTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_TFIELD_ACTIVATED_INJECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FieldActivatedInjection
// Description
//   Injection at specified positions, with the conditions:
//   - for injection to be allowed
//     factor*referenceField[cellI] >= thresholdField[cellI]
//     where:
//      - referenceField is the field used to supply the look-up values
//      - thresholdField supplies the values beyond which the injection is
//       permitted
//   - limited to a user-supllied number of injections per injector location
// SourceFiles
//   _field_activated_injection.cpp
#include "_injection_model.hpp"
#include "distribution_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
template<class CloudType>
class FieldActivatedInjection
:
  public InjectionModel<CloudType>
{
  // Private data
    // Model parameters
      //- Factor to apply to reference field
      const scalar factor_;
      //- Reference field
      const volScalarField& referenceField_;
      //- Threshold field
      const volScalarField& thresholdField_;
    // Injector properties
      //- Name of file containing positions data
      const word positionsFile_;
      //- Field of injector (x,y,z) positions
      vectorIOField positions_;
      //- List of cell labels corresponding to injector positions
      labelList injectorCells_;
      //- List of tetFace labels corresponding to injector positions
      labelList injectorTetFaces_;
      //- List of tetPt labels corresponding to injector positions
      labelList injectorTetPts_;
      //- Number of parcels per injector
      const label nParcelsPerInjector_;
      //- List of number of parcels injected for each injector
      labelList nParcelsInjected_;
    // Parcel properties
      //- Initial parcel velocity
      const vector U0_;
      //- List of parcel diameters
      scalarList diameters_;
      //- Parcel size distribution model
      const autoPtr<distributionModels::distributionModel>
        sizeDistribution_;
public:
  //- Runtime type information
  TYPE_NAME("fieldActivatedInjection");
  // Constructors
    //- Construct from dictionary
    FieldActivatedInjection
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    FieldActivatedInjection(const FieldActivatedInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new FieldActivatedInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~FieldActivatedInjection();
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
#   include "_field_activated_injection.cpp"
#endif
#endif
