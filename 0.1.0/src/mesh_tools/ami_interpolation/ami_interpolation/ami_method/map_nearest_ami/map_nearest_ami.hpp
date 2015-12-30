// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapNearestAMI
// Description
//   Nearest-mapping Arbitrary Mesh Interface (AMI) method
// SourceFiles
//   map_nearest_ami.cpp
#ifndef map_nearest_ami_hpp_
#define map_nearest_ami_hpp_
#include "ami_method.hpp"
namespace mousse
{
template<class SourcePatch, class TargetPatch>
class mapNearestAMI
:
  public AMIMethod<SourcePatch, TargetPatch>
{
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    mapNearestAMI(const mapNearestAMI&);
    //- Disallow default bitwise assignment
    void operator=(const mapNearestAMI&);
    // Marching front
      //- Find nearest target face for source face srcFaceI
      void findNearestFace
      (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const label& srcFaceI,
        label& tgtFaceI
      ) const;
      //- Determine next source-target face pair
      void setNextNearestFaces
      (
        boolList& mapFlag,
        label& startSeedI,
        label& srcFaceI,
        label& tgtFaceI
      ) const;
      //- Find mapped source face
      label findMappedSrcFace
      (
        const label tgtFaceI,
        const List<DynamicList<label> >& tgtToSrc
      ) const;
    // Evaluation
      //- Area of intersection between source and target faces
      scalar interArea
      (
        const label srcFaceI,
        const label tgtFaceI
      ) const;
public:
  //- Runtime type information
  TypeName("mapNearestAMI");
  // Constructors
    //- Construct from components
    mapNearestAMI
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const scalarField& srcMagSf,
      const scalarField& tgtMagSf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool reverseTarget = false,
      const bool requireMatch = true
    );
  //- Destructor
  virtual ~mapNearestAMI();
  // Member Functions
    // Manipulation
      //- Update addressing and weights
      virtual void calculate
      (
        labelListList& srcAddress,
        scalarListList& srcWeights,
        labelListList& tgtAddress,
        scalarListList& tgtWeights,
        label srcFaceI = -1,
        label tgtFaceI = -1
      );
};
}  // namespace mousse
#ifdef NoRepository
#   include "map_nearest_ami.cpp"
#endif
#endif
