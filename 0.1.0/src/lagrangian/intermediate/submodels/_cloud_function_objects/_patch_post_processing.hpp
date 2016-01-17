// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchPostProcessing
// Description
//   Standard post-processing
// SourceFiles
//   _patch_post_processing.cpp
#ifndef _patch_post_processing_hpp_
#define _patch_post_processing_hpp_
#include "_cloud_function_object.hpp"
namespace mousse
{
template<class CloudType>
class PatchPostProcessing
:
  public CloudFunctionObject<CloudType>
{
  // Private data
    typedef typename CloudType::particleType parcelType;
    //- Maximum number of parcels to store - set as a scalar for I/O
    scalar maxStoredParcels_;
    //- List of patch indices to post-process
    labelList patchIDs_;
    //- List of time for each data record
    List<DynamicList<scalar> > times_;
    //- List of output data per patch
    List<DynamicList<string> > patchData_;
  // Private Member Functions
    //- Returns local patchI if patch is in patchIds_ list
    label applyToPatch(const label globalPatchI) const;
protected:
  // Protected Member Functions
    //- Write post-processing info
    void write();
public:
  //- Runtime type information
  TYPE_NAME("patchPostProcessing");
  // Constructors
    //- Construct from dictionary
    PatchPostProcessing
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    PatchPostProcessing(const PatchPostProcessing<CloudType>& ppm);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new PatchPostProcessing<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~PatchPostProcessing();
  // Member Functions
    // Access
      //- Return maximum number of parcels to store per patch
      inline label maxStoredParcels() const;
      //- Return const mapping from local to global patch ids
      inline const labelList& patchIDs() const;
    // Evaluation
      //- Post-patch hook
      virtual void postPatch
      (
        const parcelType& p,
        const polyPatch& pp,
        const scalar trackFraction,
        const tetIndices& tetIs,
        bool& keepParticle
      );
};
}  // namespace mousse

template<class CloudType>
mousse::label mousse::PatchPostProcessing<CloudType>::maxStoredParcels() const
{
  return maxStoredParcels_;
}
template<class CloudType>
const mousse::labelList& mousse::PatchPostProcessing<CloudType>::patchIDs() const
{
  return patchIDs_;
}
#ifdef NoRepository
  #include "_patch_post_processing.cpp"
#endif
#endif
