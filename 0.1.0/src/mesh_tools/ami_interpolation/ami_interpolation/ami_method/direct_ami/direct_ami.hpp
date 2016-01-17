// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directAMI
// Description
//   Direct mapped Arbitrary Mesh Interface (AMI) method
// SourceFiles
//   direct_ami.cpp

#ifndef direct_ami_hpp_
#define direct_ami_hpp_

#include "ami_method.hpp"

namespace mousse
{
template<class SourcePatch, class TargetPatch>
class directAMI
:
  public AMIMethod<SourcePatch, TargetPatch>
{
private:

  // Private Member Functions

    // Marching front

      //- Append to list of src face seed indices
      void appendToDirectSeeds
      (
        labelList& mapFlag,
        labelList& srcTgtSeed,
        DynamicList<label>& srcSeeds,
        DynamicList<label>& nonOverlapFaces,
        label& srcFaceI,
        label& tgtFaceI
      ) const;

      //- Restart the advancing front - typically happens for
      //  disconnected regions
      void restartAdvancingFront
      (
        labelList& mapFlag,
        DynamicList<label>& nonOverlapFaces,
        label& srcFaceI,
        label& tgtFaceI
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
  TYPE_NAME("directAMI");

  // Constructors

    //- Construct from components
    directAMI
    (
      const SourcePatch& srcPatch,
      const TargetPatch& tgtPatch,
      const scalarField& srcMagSf,
      const scalarField& tgtMagSf,
      const faceAreaIntersect::triangulationMode& triMode,
      const bool reverseTarget = false,
      const bool requireMatch = true
    );

    //- Disallow default bitwise copy construct
    directAMI(const directAMI&) = delete;

    //- Disallow default bitwise assignment
    directAMI& operator=(const directAMI&) = delete;

  //- Destructor
  virtual ~directAMI();

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
#   include "direct_ami.cpp"
#endif
#endif
