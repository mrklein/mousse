// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::coupledFacePointPatch
// Description
//   coupled patch for post-processing.  Used as the base class for processor
//   and cyclic pointPatches
// SourceFiles
//   coupled_face_point_patch.cpp
#ifndef coupled_face_point_patch_hpp_
#define coupled_face_point_patch_hpp_
#include "coupled_point_patch.hpp"
#include "face_point_patch.hpp"
#include "coupled_poly_patch.hpp"
namespace mousse
{
class pointBoundaryMesh;
class coupledFacePointPatch
:
  public facePointPatch,
  public coupledPointPatch
{
  // Private data
    const coupledPolyPatch& coupledPolyPatch_;
  // Private Member Functions
    //- Disallow default construct as copy
    coupledFacePointPatch(const coupledFacePointPatch&);
    //- Disallow default assignment
    void operator=(const coupledFacePointPatch&);
protected:
    // Construction of demand-driven data
      //- Calculate mesh points
      virtual void calcGeometry(PstreamBuffers&) = 0;
public:
  //- Runtime type information
  TYPE_NAME(coupledPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    coupledFacePointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
  //- Destructor
  virtual ~coupledFacePointPatch();
};
}  // namespace mousse
#endif
