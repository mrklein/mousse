#ifndef CORE_MESHES_LDU_PRIMITIVE_MESH_HPP_
#define CORE_MESHES_LDU_PRIMITIVE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lduPrimitiveMesh
// Description
//   Simplest contrete lduMesh which stores the addressing needed by lduMatrix.

#include "ldu_mesh.hpp"
#include "label_list.hpp"
#include "ptr_list.hpp"


namespace mousse {

class lduPrimitiveMesh
:
  public lduMesh,
  public lduAddressing
{
  // Private data

    //- Lower addressing
    labelList lowerAddr_;

    //- Upper addressing
    labelList upperAddr_;

    //- List of pointers for each patch
    //  with only those pointing to interfaces being set
    lduInterfacePtrsList interfaces_;

    //- Concrete interfaces
    PtrList<const lduInterface> primitiveInterfaces_;

    //- Patch field evaluation schedule
    lduSchedule patchSchedule_;

    //- Communicator to use for any parallel communication
    const label comm_;

  // Private Member Functions

    //- Get size of all meshes
    static label totalSize(const PtrList<lduPrimitiveMesh>&);

    static labelList upperTriOrder
    (
      const label nCells,
      const labelUList& lower,
      const labelUList& upper
    );

    //- Check if in upper-triangular ordering
    static void checkUpperTriangular
    (
      const label size,
      const labelUList& l,
      const labelUList& u
    );

public:

  // Static data

    // Declare name of the class and its debug switch
    CLASS_NAME("lduPrimitiveMesh");

  // Constructors

    //- Construct from components but without interfaces. Add interfaces
    //  separately using addInterfaces
    lduPrimitiveMesh
    (
      const label nCells,
      labelList& l,
      labelList& u,
      const label comm,
      bool reUse
    );

    //- Add interfaces to a mesh constructed without
    void addInterfaces
    (
      lduInterfacePtrsList& interfaces,
      const lduSchedule& ps
    );

    //- Construct from components and re-use storage.
    lduPrimitiveMesh
    (
      const label nCells,
      labelList& l,
      labelList& u,
      PtrList<const lduInterface>& primitiveInterfaces,
      const lduSchedule& ps,
      const label comm
    );

    //- Construct by combining multiple meshes. The meshes come from
    //  processors procIDs:
    //  procIDs[0] : local processor (myMesh)
    //  procIDs[i] : processor where otherMeshes[i-1] comes from
    //  procAgglomMap : for every processor which processor it agglomerates
    //                  onto. The new processor numbers are in compact
    //                  numbering (so ranks in communicator comm), i.e.
    //                  similar to cell-restrict-addressing.
    //                  We need this information to be able to map
    //                  inter-processor interfaces
    // cellOffsets : for every processor the offset it gets in the mesh
    // faceMap     : for every processor, for every face, the destination
    //               face. Negative for flipped faces.
    // boundaryMap : for every processor, for every patch, -1 or the new
    //               patch index in the mesh.
    // boundaryFaceMap : for every processor, for every patch, for every
    //                   patch face:
    //                   - the new internal face (if boundaryMap=-1)
    //                   - the new patch face (if boundaryMap>=0)
    //                   Faces becoming internal are negative for flipped
    //                   faces.
    lduPrimitiveMesh
    (
      const label comm,
      const labelList& procAgglomMap,
      const labelList& procIDs,
      const lduMesh& myMesh,
      const PtrList<lduPrimitiveMesh>& otherMeshes,
      labelList& cellOffsets,
      labelList& faceOffsets,
      labelListList& faceMap,
      labelListList& boundaryMap,
      labelListListList& boundaryFaceMap
    );

    //- Disallow default bitwise copy construct
    lduPrimitiveMesh(const lduPrimitiveMesh&) = delete;

    //- Disallow default bitwise assignment
    lduPrimitiveMesh& operator=(const lduPrimitiveMesh&) = delete;

  //- Destructor
  virtual ~lduPrimitiveMesh()
  {}

  // Member Functions

    // Access

      //- Return ldu addressing
      virtual const lduAddressing& lduAddr() const
      {
        return *this;
      }

      //- Return a list of pointers for each patch
      //  with only those pointing to interfaces being set
      virtual lduInterfacePtrsList interfaces() const
      {
        return interfaces_;
      }

      //- Return a list of pointers for each patch
      //  with only those pointing to interfaces being set
      //  (reference to cached interfaces)
      const lduInterfacePtrsList& rawInterfaces() const
      {
        return interfaces_;
      }

      //- Return communicator used for parallel communication
      virtual label comm() const
      {
        return comm_;
      }

      //- Return Lower addressing
      virtual const labelUList& lowerAddr() const
      {
        return lowerAddr_;
      }

      //- Return Upper addressing
      virtual const labelUList& upperAddr() const
      {
        return upperAddr_;
      }

      //- Return patch addressing
      virtual const labelUList& patchAddr(const label i) const
      {
        return interfaces_[i].faceCells();
      }

      //- Return patch evaluation schedule
      virtual const lduSchedule& patchSchedule() const
      {
        return patchSchedule_;
      }

    // Helper

      //- Select either mesh0 (meshI is 0) or otherMeshes[meshI-1]
      static const lduMesh& mesh
      (
        const lduMesh& mesh0,
        const PtrList<lduPrimitiveMesh>& otherMeshes,
        const label meshI
      );

      //- Gather meshes from other processors onto procIDs[0].
      //  Received meshes get GAMGInterface and communicator comm
      static void gather
      (
        const label comm,
        const lduMesh& mesh,
        const labelList& procIDs,
        PtrList<lduPrimitiveMesh>& otherMeshes
      );

      //- Get non-scheduled send/receive schedule
      template<class ProcPatch>
      static lduSchedule nonBlockingSchedule(const lduInterfacePtrsList&);
};
}  // namespace mousse

#include "ldu_primitive_mesh.ipp"

#endif
