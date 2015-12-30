// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicAMIPointPatch
// Description
//   Cyclic AMI point patch - place holder only
// SourceFiles
//   cyclic_ami_point_patch.cpp
#ifndef cyclic_ami_point_patch_hpp_
#define cyclic_ami_point_patch_hpp_
#include "coupled_face_point_patch.hpp"
#include "cyclic_ami_poly_patch.hpp"
#include "point_boundary_mesh.hpp"
namespace mousse
{
class cyclicAMIPointPatch
:
  public coupledFacePointPatch
{
  // Private data
    //- Local reference cast into the cyclic AMI patch
    const cyclicAMIPolyPatch& cyclicAMIPolyPatch_;
  // Private Member Functions
    //- Disallow default construct as copy
    cyclicAMIPointPatch(const cyclicAMIPointPatch&);
    //- Disallow default assignment
    void operator=(const cyclicAMIPointPatch&);
protected:
  // Protected Member Functions
    //- Initialise the calculation of the patch geometry
    virtual void initGeometry(PstreamBuffers&);
    //- Calculate the patch geometry
    virtual void calcGeometry(PstreamBuffers&);
    //- Initialise the patches for moving points
    virtual void initMovePoints(PstreamBuffers&, const pointField&);
    //- Correct patches after moving points
    virtual void movePoints(PstreamBuffers&, const pointField&);
    //- Initialise the update of the patch topology
    virtual void initUpdateMesh(PstreamBuffers&);
    //- Update of the patch topology
    virtual void updateMesh(PstreamBuffers&);
public:
  //- Runtime type information
  TypeName(cyclicAMIPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    cyclicAMIPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
  //- Destructor
  virtual ~cyclicAMIPointPatch();
  // Member Functions
    //- Return the constraint type this pointPatch implements.
    virtual const word& constraintType() const
    {
      return type();
    }
    //- Return the underlying cyclicAMIPolyPatch
    const cyclicAMIPolyPatch& cyclicAMIPatch() const
    {
      return cyclicAMIPolyPatch_;
    }
    //- Return neighbour point patch
    const cyclicAMIPointPatch& neighbPatch() const
    {
      label patchI = cyclicAMIPolyPatch_.neighbPatchID();
      const pointPatch& pp = this->boundaryMesh()[patchI];
      return refCast<const cyclicAMIPointPatch>(pp);
    }
    //- Are the cyclic planes parallel
    bool parallel() const
    {
      return cyclicAMIPolyPatch_.parallel();
    }
    //- Return face transformation tensor
    const tensorField& forwardT() const
    {
      return cyclicAMIPolyPatch_.forwardT();
    }
    //- Return neighbour-cell transformation tensor
    const tensorField& reverseT() const
    {
      return cyclicAMIPolyPatch_.reverseT();
    }
};
}  // namespace mousse
#endif
