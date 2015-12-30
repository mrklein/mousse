// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoInjection
// Description
//   Place holder for 'none' option
// SourceFiles
//   _no_injection.cpp
#ifndef _no_injection_hpp_
#define _no_injection_hpp_
#include "_injection_model.hpp"
namespace mousse
{
template<class CloudType>
class NoInjection
:
  public InjectionModel<CloudType>
{
public:
  //- Runtime type information
  TypeName("none");
  // Constructors
    //- Construct from components
    NoInjection(const dictionary&, CloudType&, const word&);
    //- Construct copy
    NoInjection(const NoInjection<CloudType>& im);
    //- Construct and return a clone
    virtual autoPtr<InjectionModel<CloudType> > clone() const
    {
      return autoPtr<InjectionModel<CloudType> >
      (
        new NoInjection<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~NoInjection();
  // Member Functions
    //- Flag to indicate whether model activates injection model
    bool active() const;
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
#   include "_no_injection.cpp"
#endif
#endif
