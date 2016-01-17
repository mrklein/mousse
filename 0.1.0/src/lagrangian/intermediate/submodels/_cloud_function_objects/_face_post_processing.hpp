// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FacePostProcessing
// Description
//   Records particle face quantities on used-specified face zone
//   Currently supports:
//     accummulated mass
//     average mass flux
// SourceFiles
//   _face_post_processing.cpp
#ifndef _face_post_processing_hpp_
#define _face_post_processing_hpp_
#include "_cloud_function_object.hpp"
#include "face_zone.hpp"
namespace mousse
{
template<class CloudType>
class FacePostProcessing
:
  public CloudFunctionObject<CloudType>
{
  // Private Data
    // Typedefs
      //- Convenience typedef for parcel type
      typedef typename CloudType::parcelType parcelType;
    //- Face zone IDs
    labelList faceZoneIDs_;
    //- Surface output format
    const word surfaceFormat_;
    //- Flag to indicate whether data should be reset/cleared on writing
    Switch resetOnWrite_;
    //- Total time
    scalar totalTime_;
    //- Mass storage
    List<scalarField> mass_;
    //- Mass total storage
    List<scalarField> massTotal_;
    //- Mass flow rate storage
    List<scalarField> massFlowRate_;
    //- Flag to indicate whether data should be written to file
    Switch log_;
    //- Output file pointer per zone
    PtrList<OFstream> outputFilePtr_;
    //- Last calculation time
    scalar timeOld_;
  // Private Member Functions
    //- Helper function to create log files
    void makeLogFile
    (
      const word& zoneName,
      const label zoneI,
      const label nFaces,
      const scalar totArea
    );
protected:
  // Protected Member Functions
    //- Write post-processing info
    void write();
public:
  //- Runtime type information
  TYPE_NAME("facePostProcessing");
  // Constructors
    //- Construct from dictionary
    FacePostProcessing
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    FacePostProcessing(const FacePostProcessing<CloudType>& ppm);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new FacePostProcessing<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~FacePostProcessing();
  // Member Functions
    // Access
      //- Return const access to the reset on write flag
      inline const Switch& resetOnWrite() const;
    // Evaluation
      //- Post-face hook
      virtual void postFace
      (
        const parcelType& p,
        const label faceI,
        bool& keepParticle
      );
};
}  // namespace mousse

template<class CloudType>
inline const mousse::Switch&
mousse::FacePostProcessing<CloudType>::resetOnWrite() const
{
  return resetOnWrite_;
}
#ifdef NoRepository
#   include "_face_post_processing.cpp"
#endif
#endif
