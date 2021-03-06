#ifndef MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_POINT_PATCH_CYCLIC_ACMI_POINT_PATCH_HPP_
#define MESH_TOOLS_AMI_INTERPOLATION_PATCHES_CYCLIC_ACMI_CYCLIC_ACMI_POINT_PATCH_CYCLIC_ACMI_POINT_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cyclicACMIPointPatch
// Description
//   Cyclic AMI point patch - place holder only

#include "coupled_face_point_patch.hpp"
#include "cyclic_acmi_poly_patch.hpp"
#include "point_boundary_mesh.hpp"


namespace mousse {

class cyclicACMIPointPatch
:
  public coupledFacePointPatch
{
  // Private data
    //- Local reference cast into the cyclic AMI patch
    const cyclicACMIPolyPatch& cyclicACMIPolyPatch_;
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
  TYPE_NAME(cyclicACMIPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    cyclicACMIPointPatch
    (
      const polyPatch& patch,
      const pointBoundaryMesh& bm
    );
    //- Disallow default construct as copy
    cyclicACMIPointPatch(const cyclicACMIPointPatch&) = delete;
    //- Disallow default assignment
    void operator=(const cyclicACMIPointPatch&) = delete;
  //- Destructor
  virtual ~cyclicACMIPointPatch();
  // Member Functions
    //- Is patch 'coupled'. Note that on AMI the geometry is not
    //  coupled but the fields are!
    virtual bool coupled() const
    {
      return false;
    }
    //- Return the constraint type this pointPatch implements.
    virtual const word& constraintType() const
    {
      return type();
    }
    //- Return the underlying cyclicAMIPolyPatch
    const cyclicACMIPolyPatch& cyclicACMIPatch() const
    {
      return cyclicACMIPolyPatch_;
    }
    //- Return neighbour point patch
    const cyclicACMIPointPatch& neighbPatch() const
    {
      label patchI = cyclicACMIPolyPatch_.neighbPatchID();
      const pointPatch& pp = this->boundaryMesh()[patchI];
      return refCast<const cyclicACMIPointPatch>(pp);
    }
    //- Are the cyclic planes parallel
    bool parallel() const
    {
      return cyclicACMIPolyPatch_.parallel();
    }
    //- Return face transformation tensor
    const tensorField& forwardT() const
    {
      return cyclicACMIPolyPatch_.forwardT();
    }
    //- Return neighbour-cell transformation tensor
    const tensorField& reverseT() const
    {
      return cyclicACMIPolyPatch_.reverseT();
    }
};

}  // namespace mousse

#endif
