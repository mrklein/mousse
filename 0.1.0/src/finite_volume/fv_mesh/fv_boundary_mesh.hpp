#ifndef FINITE_VOLUME_FV_MESH_FV_BOUNDARY_MESH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_BOUNDARY_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvBoundaryMesh
// Description
//   mousse::fvBoundaryMesh

#include "fv_patch_list.hpp"
#include "ldu_interface_ptrs_list.hpp"


namespace mousse {

class fvMesh;
class polyBoundaryMesh;


class fvBoundaryMesh
:
  public fvPatchList
{
  // Private data
    //- Reference to mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Add fvPatches corresponding to the given polyBoundaryMesh
    void addPatches(const polyBoundaryMesh&);
protected:
    //- Update boundary based on new polyBoundaryMesh
    void readUpdate(const polyBoundaryMesh&);
public:
  friend class fvMesh;
  // Constructors
    //- Construct with zero size
    fvBoundaryMesh(const fvMesh&);
    //- Construct from polyBoundaryMesh
    fvBoundaryMesh(const fvMesh&, const polyBoundaryMesh&);
    //- Disable default copy construct
    fvBoundaryMesh(const fvBoundaryMesh&) = delete;
    //- Disallow assignment
    void operator=(const fvBoundaryMesh&) = delete;
  // Member Functions
    // Access
      //- Return the mesh reference
      const fvMesh& mesh() const
      {
        return mesh_;
      }
      //- Return a list of pointers for each patch
      //  with only those pointing to interfaces being set
      lduInterfacePtrsList interfaces() const;
      //- Find patch index given a name
      label findPatchID(const word& patchName) const;
      //- Find patch indices given a name
      labelList findIndices(const keyType&, const bool useGroups) const;
    //- Correct patches after moving points
    void movePoints();
  // Member Operators
    //- Return const and non-const reference to fvPatch by index.
    using fvPatchList::operator[];
    //- Return const reference to fvPatch by name.
    const fvPatch& operator[](const word&) const;
    //- Return reference to fvPatch by name.
    fvPatch& operator[](const word&);
};
}  // namespace mousse
#endif
