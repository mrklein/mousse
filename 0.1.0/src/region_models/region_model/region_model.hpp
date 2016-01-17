// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModel
// Description
//   Base class for region models
// SourceFiles
//   region_model.cpp
#ifndef region_model_hpp_
#define region_model_hpp_
#include "iodictionary.hpp"
#include "switch.hpp"
#include "label_list.hpp"
#include "vol_fields.hpp"
#include "mapped_patch_base.hpp"
#include "region_model_function_object_list.hpp"
namespace mousse
{
namespace regionModels
{
class regionModel
:
  public IOdictionary
{
private:
  // Private Member Functions
    //- Construct region mesh and fields
    void constructMeshObjects();
    //- Construct region mesh and dictionary
    void constructMeshObjects(const dictionary& dict);
    //- Initialise the region
    void initialise();
protected:
  // Protected data
    //- Reference to the primary mesh database
    const fvMesh& primaryMesh_;
    //- Reference to the time database
    const Time& time_;
    //- Active flag
    Switch active_;
    //- Active information output
    Switch infoOutput_;
    //- Model name
    const word modelName_;
    //- Pointer to the region mesh database
    autoPtr<fvMesh> regionMeshPtr_;
    //- Model coefficients dictionary
    dictionary coeffs_;
    //- Dictionary of output properties
    autoPtr<IOdictionary> outputPropertiesPtr_;
    // Addressing
      //- List of patch IDs on the primary region coupled to this region
      labelList primaryPatchIDs_;
      //- List of patch IDs internally coupled with the primary region
      labelList intCoupledPatchIDs_;
    //- Region name
    word regionName_;
    //- Region model function objects
    regionModelFunctionObjectList functions_;
    // Inter-region AMI interpolation caching
      //- List of region names this region is coupled to
      mutable wordList interRegionAMINames_;
      //- List of AMI objects per coupled region
      mutable PtrList<PtrList<AMIPatchToPatchInterpolation> >
        interRegionAMI_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary& dict);
    //- Create or return a new inter-region AMI object
    virtual const AMIPatchToPatchInterpolation& interRegionAMI
    (
      const regionModel& nbrRegion,
      const label regionPatchI,
      const label nbrPatchI,
      const bool flip
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("regionModel");
  // Constructors
    //- Construct null
    regionModel(const fvMesh& mesh, const word& regionType);
    //- Construct from mesh, region type and name
    regionModel
    (
      const fvMesh& mesh,
      const word& regionType,
      const word& modelName,
      bool readFields = true
    );
    //- Construct from mesh and name and dict
    regionModel
    (
      const fvMesh& mesh,
      const word& regionType,
      const word& modelName,
      const dictionary& dict,
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    regionModel(const regionModel&) = delete;
    //- Disallow default bitwise assignment
    regionModel& operator=(const regionModel&) = delete;
  //- Destructor
  virtual ~regionModel();
  // Member Functions
    // Access
      //- Return the reference to the primary mesh database
      inline const fvMesh& primaryMesh() const;
      //- Return the reference to the time database
      inline const Time& time() const;
      //- Return the active flag
      inline const Switch& active() const;
      //- Return the information flag
      inline const Switch& infoOutput() const;
      //- Return the model name
      inline const word& modelName() const;
      //- Return the region mesh database
      inline const fvMesh& regionMesh() const;
      //- Return the region mesh database for manipulation
      inline fvMesh& regionMesh();
      //- Return the model coefficients dictionary
      inline const dictionary& coeffs() const;
      //- Return the solution dictionary
      inline const dictionary& solution() const;
      //- Return const access to the output properties dictionary
      inline const IOdictionary& outputProperties() const;
      //- Return output properties dictionary
      inline IOdictionary& outputProperties();
      // Addressing
        //- Return true if patchI on the local region is a coupled
        //  patch to the primary region
        inline bool isCoupledPatch(const label regionPatchI) const;
        //- Return true if patchI on the primary region is a coupled
        //  patch to the local region
        inline bool isRegionPatch(const label primaryPatchI) const;
        //- Return the list of patch IDs on the primary region coupled
        //  to this region
        inline const labelList& primaryPatchIDs() const;
        //- Return the list of patch IDs internally coupled with the
        //  primary region
        inline const labelList& intCoupledPatchIDs() const;
        //- Return region ID corresponding to primaryPatchID
        inline label regionPatchID(const label primaryPatchID) const;
    // Helper
      //- Return the coupled patch ID paired with coupled patch
      //  regionPatchI
      label nbrCoupledPatchID
      (
        const regionModel& nbrRegion,
        const label regionPatchI
      ) const;
      //- Map patch field from another region model to local patch
      template<class Type>
      tmp<mousse::Field<Type> > mapRegionPatchField
      (
        const regionModel& nbrRegion,
        const label regionPatchI,
        const label nbrPatchI,
        const Field<Type>& nbrField,
        const bool flip = false
      ) const;
      //- Map patch field from another region model to local patch
      template<class Type>
      tmp<Field<Type> > mapRegionPatchField
      (
        const regionModel& nbrRegion,
        const word& fieldName,
        const label regionPatchI,
        const bool flip = false
      ) const;
      //- Map patch internal field from another region model to local
      //  patch
      template<class Type>
      tmp<Field<Type> > mapRegionPatchInternalField
      (
        const regionModel& nbrRegion,
        const word& fieldName,
        const label regionPatchI,
        const bool flip = false
      ) const;
      //- Convert a local region field to the primary region
      template<class Type>
      void toPrimary
      (
        const label regionPatchI,
        List<Type>& regionField
      ) const;
      //- Convert a primary region field to the local region
      template<class Type>
      void toRegion
      (
        const label regionPatchI,
        List<Type>& primaryFieldField
      ) const;
      //- Convert a local region field to the primary region with op
      template<class Type, class CombineOp>
      void toPrimary
      (
        const label regionPatchI,
        List<Type>& regionField,
        const CombineOp& cop
      ) const;
      //- Convert a primary region field to the local region with op
      template<class Type, class CombineOp>
      void toRegion
      (
        const label regionPatchI,
        List<Type>& primaryFieldField,
        const CombineOp& cop
      ) const;
    // Evolution
      //- Main driver routing to evolve the region - calls other evolves
      virtual void evolve();
      //- Pre-evolve region
      virtual void preEvolveRegion();
      //- Evolve the region
      virtual void evolveRegion();
      //- Post-evolve region
      virtual void postEvolveRegion();
    // I-O
      //- Provide some feedback
      virtual void info();
};
}  // namespace regionModels
}  // namespace mousse

inline const mousse::fvMesh&
mousse::regionModels::regionModel::primaryMesh() const
{
  return primaryMesh_;
}
inline const mousse::Time& mousse::regionModels::regionModel::time() const
{
  return time_;
}
inline const mousse::Switch& mousse::regionModels::regionModel::active() const
{
  return active_;
}
inline const mousse::Switch& mousse::regionModels::regionModel::infoOutput() const
{
  return infoOutput_;
}
inline const mousse::word& mousse::regionModels::regionModel::modelName() const
{
  return modelName_;
}
inline const mousse::fvMesh& mousse::regionModels::regionModel::regionMesh() const
{
  if (time_.foundObject<fvMesh>(regionName_))
  {
    return time_.lookupObject<fvMesh>(regionName_);
  }
  else if (!regionMeshPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::fvMesh&"
      "mousse::regionModels::regionModel::regionMesh() const"
    )
    << "Region mesh not available" << abort(FatalError);
  }
  return regionMeshPtr_();
}
inline mousse::fvMesh& mousse::regionModels::regionModel::regionMesh()
{
  if (time_.foundObject<fvMesh>(regionName_))
  {
    return const_cast<fvMesh&>
    (
      time_.lookupObject<fvMesh>(regionName_)
    );
  }
  else if (!regionMeshPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline mousse::fvMesh&"
      "mousse::regionModels::regionModel::regionMesh()"
    )
    << "Region mesh not available" << abort(FatalError);
  }
  return regionMeshPtr_();
}
inline const mousse::dictionary& mousse::regionModels::regionModel::coeffs() const
{
  return coeffs_;
}
inline const mousse::dictionary&
mousse::regionModels::regionModel::solution() const
{
  return regionMesh().solutionDict();
}
inline const mousse::IOdictionary&
mousse::regionModels::regionModel::outputProperties() const
{
  if (!outputPropertiesPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::IOdictionary& "
      "mousse::regionModels::regionModel::outputProperties() const"
    )
    << "outputProperties dictionary not available"
    << abort(FatalError);
  }
  return outputPropertiesPtr_();
}
inline mousse::IOdictionary&
mousse::regionModels::regionModel::outputProperties()
{
  if (!outputPropertiesPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline mousse::IOdictionary& "
      "mousse::regionModels::regionModel::outputProperties()"
    )
    << "outputProperties dictionary not available"
    << abort(FatalError);
  }
  return outputPropertiesPtr_();
}
inline bool mousse::regionModels::regionModel::isCoupledPatch
(
  const label regionPatchI
) const
{
  FOR_ALL(intCoupledPatchIDs_, i)
  {
    if (intCoupledPatchIDs_[i] == regionPatchI)
    {
      return true;
    }
  }
  return false;
}
inline bool mousse::regionModels::regionModel::isRegionPatch
(
  const label primaryPatchI
) const
{
  FOR_ALL(primaryPatchIDs_, i)
  {
    if (primaryPatchIDs_[i] == primaryPatchI)
    {
      return true;
    }
  }
  return false;
}
inline const mousse::labelList&
mousse::regionModels::regionModel::primaryPatchIDs() const
{
  return primaryPatchIDs_;
}
inline const mousse::labelList&
mousse::regionModels::regionModel::intCoupledPatchIDs() const
{
  return intCoupledPatchIDs_;
}
inline mousse::label mousse::regionModels::regionModel::regionPatchID
(
  const label primaryPatchID
) const
{
  FOR_ALL(primaryPatchIDs_, i)
  {
    if (primaryPatchIDs_[i] == primaryPatchID)
    {
      return intCoupledPatchIDs_[i];
    }
  }
  return -1;
}
#ifdef NoRepository
  #include "region_model_templates.cpp"
#endif
#endif
