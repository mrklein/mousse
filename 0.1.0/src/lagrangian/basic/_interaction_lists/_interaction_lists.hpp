#ifndef LAGRANGIAN_BASIC_TINTERACTION_LISTS_TINTERACTION_LISTS_HPP_
#define LAGRANGIAN_BASIC_TINTERACTION_LISTS_TINTERACTION_LISTS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InteractionLists
// Description
//   Builds direct interaction list, specifying which local (real)
//   cells are potentially in range of each other.
//   Builds referred interaction list, specifying which cells are
//   required to provide interactions across coupled patches (cyclic or
//   processor).  Generates referred cells, and refers particles to the
//   correct processor, applying the appropriate transform.
//   Simultaneous communication and computation is possible using:
//   \verbatim
//   PstreamBuffers pBufs(Pstream::nonBlocking);
//   label startOfRequests = Pstream::nRequests();
//   il_.sendReferredData(cellOccupancy_, pBufs);
//   // Do other things
//   il_.receiveReferredData(pBufs, startOfRequests);
//   \endverbatim
//   Requiring data:
//   \verbatim
//   List<DynamicList<typename CloudType::parcelType*>> cellOccupancy_;
//   \endverbatim

#include "poly_mesh.hpp"
#include "referred_wall_face.hpp"


namespace mousse {

class globalIndexAndTransform;
class mapDistribute;


template<class ParticleType>
class InteractionLists
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Dummy cloud to give to particles
    Cloud<ParticleType> cloud_;
    //- Switch controlling whether or not the cloud gets populated
    //  with the referred particles, hence gets written out
    const Switch writeCloud_;
    //- mapDistribute to exchange referred particles into referred cells
    autoPtr<mapDistribute> cellMapPtr_;
    //- mapDistribute to exchange wall face data
    autoPtr<mapDistribute> wallFaceMapPtr_;
    //- Maximum distance over which interactions will be detected
    scalar maxDistance_;
    //- Direct interaction list
    labelListList dil_;
    //- Wall faces on this processor that are in interaction range
    //  of each each cell (direct wall face interaction list)
    labelListList dwfil_;
    //- Referred interaction list - which real cells are to be
    //  supplied with particle interactions from the referred
    //  particle container with the same index.
    labelListList ril_;
    //- Inverse addressing for referred cells, specifies which
    //  referred cells (indices of entries in the ril_ and
    //  referredParticles_ lists) interact with the real cell
    //  indexed in this container.
    labelListList rilInverse_;
    //- Which real cells on this on this processor are in
    //  interaction range of each referred wall face (referred
    //  wall face interaction list)
    labelListList rwfil_;
    //- Inverse addressing for referred wall faces, specifies
    //  which referred wall faces interact with the real cells
    //  indexed in this container.
    labelListList rwfilInverse_;
    //- Which cells are to be sent via the cellMap, and an index
    //  specifying how they should be transformed.
    List<labelPair> cellIndexAndTransformToDistribute_;
    //- Which wallFaces are to be sent via the wallFaceMap, and an index
    //  specifying how they should be transformed.
    List<labelPair> wallFaceIndexAndTransformToDistribute_;
    //- Referred wall faces
    List<referredWallFace> referredWallFaces_;
    //- Velocity field name, default to "U"
    const word UName_;
    //- Referred wall face velocity field values;
    List<vector> referredWallData_;
    //- Referred particle container
    List<IDLList<ParticleType>> referredParticles_;
  // Private Member Functions
    //- Construct all interaction lists
    void buildInteractionLists();
    //- Find the other processors which have interaction range
    //  extended bound boxes in range
    void findExtendedProcBbsInRange
    (
      const treeBoundBox& procBb,
      const List<treeBoundBox>& allExtendedProcBbs,
      const globalIndexAndTransform& globalTransforms,
      List<treeBoundBox>& extendedProcBbsInRange,
      List<label>& extendedProcBbsTransformIndex,
      List<label>& extendedProcBbsOrigProc
    );
    //- Build the mapDistribute from information about which entry
    //  is to be sent to which processor
    void buildMap
    (
      autoPtr<mapDistribute>& mapPtr,
      const List<label>& toProc
    );
    //- Fill the referredParticles container with particles that
    //  will be referred
    void prepareParticlesToRefer
    (
      const List<DynamicList<ParticleType*>>& cellOccupancy
    );
    //- Prepare particle to be referred
    void prepareParticleToBeReferred
    (
      ParticleType* particle,
      labelPair iat
    );
    //- Fill the referredParticles so that it will be written out
    void fillReferredParticleCloud();
    //- Populate the referredWallData container with data that
    //  will be referred.
    void prepareWallDataToRefer();
    //- Write the referred wall faces out for debug
    void writeReferredWallFaces() const;
public:
  // Constructors
    //- Construct null from mesh
    InteractionLists(const polyMesh& mesh);
    //- Construct and call function to create all information from
    //  the mesh
    InteractionLists
    (
      const polyMesh& mesh,
      scalar maxDistance,
      Switch writeCloud = false,
      const word& UName = "U"
    );
    //- Disallow default bitwise copy construct
    InteractionLists(const InteractionLists&) = delete;
    //- Disallow default bitwise assignment
    InteractionLists& operator=(const InteractionLists&) = delete;
  // Destructor
    ~InteractionLists();
  // Member Functions
    //- Prepare and send referred particles and wall data,
    //  nonBlocking communication
    void sendReferredData
    (
      const List<DynamicList<ParticleType*>>& cellOccupancy,
      PstreamBuffers& pBufs
    );
    //- Receive referred data
    void receiveReferredData
    (
      PstreamBuffers& pBufs,
      const label startReq = 0
    );
    // Access
      //- Return access to the mesh
      inline const polyMesh& mesh() const;
      //- Return access to the cellMap
      inline const mapDistribute& cellMap() const;
      //- Return access to the wallFaceMap
      inline const mapDistribute& wallFaceMap() const;
      //- Return access to the direct interaction list
      inline const labelListList& dil() const;
      //- Return access to the direct wall face interaction list
      inline const labelListList& dwfil() const;
      //- Return access to the referred interaction list
      inline const labelListList& ril() const;
      //- Return access to the inverse referred interaction list
      inline const labelListList& rilInverse() const;
      //- Return access to the referred wall face interaction list
      inline const labelListList& rwfil() const;
      //- Return access to the inverse referred wall face
      //  interaction list
      inline const labelListList& rwfilInverse() const;
      //- Return access to the cellIndexAndTransformToDistribute list
      inline const List<labelPair>&
      cellIndexAndTransformToDistribute() const;
      //- Return access to the wallFaceIndexAndTransformToDistribute list
      inline const List<labelPair>&
      wallFaceIndexAndTransformToDistribute() const;
      //- Return access to the referred wall faces
      inline const List<referredWallFace>& referredWallFaces() const;
      //- Return the name of the velocity field
      inline const word& UName() const;
      //- Return access to the referred wall data
      inline const List<vector>& referredWallData() const;
      //- Return access to the referred particle container
      inline const List<IDLList<ParticleType>>&
      referredParticles() const;
      //- Return non-const access to the referred particle container
      inline List<IDLList<ParticleType>>& referredParticles();
};

}  // namespace mousse


// Member Functions 
template<class ParticleType>
const mousse::polyMesh& mousse::InteractionLists<ParticleType>::mesh() const
{
  return mesh_;
}


template<class ParticleType>
const mousse::mapDistribute&
mousse::InteractionLists<ParticleType>::cellMap() const
{
  return cellMapPtr_();
}


template<class ParticleType>
const mousse::mapDistribute&
mousse::InteractionLists<ParticleType>::wallFaceMap() const
{
  return wallFaceMapPtr_();
}


template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::dil() const
{
  return dil_;
}


template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::dwfil() const
{
  return dwfil_;
}


template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::ril() const
{
  return ril_;
}


template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::rilInverse() const
{
  return rilInverse_;
}


template<class ParticleType>
const mousse::labelListList& mousse::InteractionLists<ParticleType>::rwfil() const
{
  return rwfil_;
}


template<class ParticleType>
const mousse::labelListList&
mousse::InteractionLists<ParticleType>::rwfilInverse() const
{
  return rwfilInverse_;
}


template<class ParticleType>
const mousse::List<mousse::labelPair>&
mousse::InteractionLists<ParticleType>::cellIndexAndTransformToDistribute() const
{
  return cellIndexAndTransformToDistribute_;
}


template<class ParticleType>
const mousse::List<mousse::labelPair>&
mousse::InteractionLists<ParticleType>::
wallFaceIndexAndTransformToDistribute() const
{
  return wallFaceIndexAndTransformToDistribute_;
}


template<class ParticleType>
const mousse::List<mousse::referredWallFace>&
mousse::InteractionLists<ParticleType>::referredWallFaces() const
{
  return referredWallFaces_;
}


template<class ParticleType>
const mousse::word& mousse::InteractionLists<ParticleType>::UName() const
{
  return UName_;
}


template<class ParticleType>
const mousse::List<mousse::vector>&
mousse::InteractionLists<ParticleType>::referredWallData() const
{
  return referredWallData_;
}


template<class ParticleType>
const mousse::List<mousse::IDLList<ParticleType>>&
mousse::InteractionLists<ParticleType>::referredParticles() const
{
  return referredParticles_;
}


template<class ParticleType>
mousse::List<mousse::IDLList<ParticleType>>&
mousse::InteractionLists<ParticleType>::referredParticles()
{
  return referredParticles_;
}

#include "_interaction_lists.ipp"

#endif
