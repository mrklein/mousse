// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PatchInjectionBase
// Description
//   Base class for patch-based injection models.
//   Class handles injecting at a random point adjacent to the patch faces to
//   provide a more stochsatic view of the injection process.  Patch faces are
//   triangulated, and area fractions accumulated.  The fractional areas are
//   then applied to determine across which face a parcel is to be injected.
// SourceFiles
//   patch_injection_base.cpp
#ifndef patch_injection_base_hpp_
#define patch_injection_base_hpp_
#include "word.hpp"
#include "label_list.hpp"
#include "scalar_list.hpp"
#include "vector_list.hpp"
#include "face_list.hpp"
namespace mousse
{
// Forward class declarations
class polyMesh;
class cachedRandom;
class patchInjectionBase
{
protected:
  // Protected data
    //- Patch name
    const word patchName_;
    //- Patch ID
    const label patchId_;
    //- Patch area - total across all processors
    scalar patchArea_;
    //- Patch face normal directions
    vectorList patchNormal_;
    //- List of cell labels corresponding to injector positions
    labelList cellOwners_;
    //- Decomposed patch faces as a list of triangles
    faceList triFace_;
    //- Addressing from per triangle to patch face
    labelList triToFace_;
    //- Cumulative triangle area per triangle face
    scalarList triCumulativeMagSf_;
    //- Cumulative area fractions per processor
    scalarList sumTriMagSf_;
public:
  // Constructors
    //- Construct from mesh and patch name
    patchInjectionBase(const polyMesh& mesh, const word& patchName);
    //- Copy constructor
    patchInjectionBase(const patchInjectionBase& pib);
  //- Destructor
  virtual ~patchInjectionBase();
  // Member Functions
    //- Update patch geometry and derived info for injection locations
    virtual void updateMesh(const polyMesh& mesh);
    //- Set the injection position and owner cell, tetFace and tetPt
    virtual void setPositionAndCell
    (
      const polyMesh& mesh,
      cachedRandom& rnd,
      vector& position,
      label& cellOwner,
      label& tetFaceI,
      label& tetPtI
    );
};
} // end namespace mousse
#endif
