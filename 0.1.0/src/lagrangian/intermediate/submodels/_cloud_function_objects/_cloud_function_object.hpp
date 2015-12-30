// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CloudFunctionObject
// Description
//   Templated cloud function object base class
// SourceFiles
//   _cloud_function_object.cpp
//   _cloud_function_object_new.cpp
#ifndef _cloud_function_object_hpp_
#define _cloud_function_object_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
class polyPatch;
class tetIndices;
template<class CloudType>
class CloudFunctionObject
:
  public CloudSubModelBase<CloudType>
{
  // Private data
    //- Output path
    fileName outputDir_;
  // Private Member Functions
    //- Inherite write from CloudSubModelBase
    using CloudSubModelBase<CloudType>::write;
    //- Write post-processing info
    virtual void write();
public:
  //- Runtime type information
  TypeName("cloudFunctionObject");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    CloudFunctionObject,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    ),
    (dict, owner, modelName)
  );
  // Constructors
    //- Construct null from owner
    CloudFunctionObject(CloudType& owner);
    //- Construct from dictionary
    CloudFunctionObject
    (
      const dictionary& dict,
      CloudType& owner,
      const word& objectType,
      const word& modelName
    );
    //- Construct copy
    CloudFunctionObject(const CloudFunctionObject<CloudType>& ppm);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new CloudFunctionObject<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~CloudFunctionObject();
  //- Selector
  static autoPtr<CloudFunctionObject<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner,
    const word& objectType,
    const word& modelName
  );
  // Member Functions
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-evolve hook
      virtual void postEvolve();
      //- Post-move hook
      virtual void postMove
      (
        typename CloudType::parcelType& p,
        const label cellI,
        const scalar dt,
        const point& position0,
        bool& keepParticle
      );
      //- Post-patch hook
      virtual void postPatch
      (
        const typename CloudType::parcelType& p,
        const polyPatch& pp,
        const scalar trackFraction,
        const tetIndices& testIs,
        bool& keepParticle
      );
      //- Post-face hook
      virtual void postFace
      (
        const typename CloudType::parcelType& p,
        const label faceI,
        bool& keepParticle
      );
    // Input/output
      //- Return the output path
      const fileName& outputDir() const;
      //- Return the output time path
      fileName outputTimeDir() const;
};
}  // namespace mousse

#define makeCloudFunctionObject(CloudType)                                    \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::CloudFunctionObject<kinematicCloudType>,                          \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      CloudFunctionObject<kinematicCloudType>,                                \
      dictionary                                                              \
    );                                                                        \
  }

#define makeCloudFunctionObjectType(SS, CloudType)                            \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::CloudFunctionObject<kinematicCloudType>::                           \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_cloud_function_object.cpp"
#endif
#endif
