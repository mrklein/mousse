// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMeshLduAddressing
// Description
//   mousse::fvMeshLduAddressing
// SourceFiles
//   fv_mesh_ldu_addressing.cpp

#ifndef fv_mesh_ldu_addressing_hpp_
#define fv_mesh_ldu_addressing_hpp_

#include "ldu_addressing.hpp"
#include "fv_mesh.hpp"
#include "global_mesh_data.hpp"

namespace mousse
{
class fvMeshLduAddressing
:
  public lduAddressing
{
  // Private data
    //- Lower as a subList of allOwner
    labelList::subList lowerAddr_;
    //- Upper as a reference to neighbour
    const labelList& upperAddr_;
    //- Patch addressing as a list of sublists
    List<const labelUList*> patchAddr_;
    //- Patch field evaluation schedule
    const lduSchedule& patchSchedule_;
public:
  // Constructors
    //- Construct from components
    fvMeshLduAddressing(const fvMesh& mesh)
    :
      lduAddressing{mesh.nCells()},
      lowerAddr_
      {
        labelList::subList
        (
          mesh.faceOwner(),
          mesh.nInternalFaces()
        )
      },
      upperAddr_(mesh.faceNeighbour()),
      patchAddr_(mesh.boundary().size()),
      patchSchedule_(mesh.globalData().patchSchedule())
    {
      FOR_ALL(mesh.boundary(), patchI)
      {
        patchAddr_[patchI] = &mesh.boundary()[patchI].faceCells();
      }
    }
    //- Disallow default bitwise copy construct
    fvMeshLduAddressing(const fvMeshLduAddressing&) = delete;
    //- Disallow default bitwise assignment
    fvMeshLduAddressing& operator=(const fvMeshLduAddressing&) = delete;
  //- Destructor
  ~fvMeshLduAddressing()
  {}
  // Member Functions
    //- Return lower addressing (i.e. lower label = upper triangle)
    const labelUList& lowerAddr() const
    {
      return lowerAddr_;
    }
    //- Return upper addressing (i.e. upper label)
    const labelUList& upperAddr() const
    {
      return upperAddr_;
    }
    //- Return patch addressing
    const labelUList& patchAddr(const label i) const
    {
      return *patchAddr_[i];
    }
    // Return patch field evaluation schedule
    const lduSchedule& patchSchedule() const
    {
      return patchSchedule_;
    }
};
}  // namespace mousse
#endif
