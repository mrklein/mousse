#ifndef MESH_TOOLS_REGION_COUPLED_GAMG_INTERFACES_REGION_COUPLED_GAMG_INTERFACE_REGION_COUPLED_BASE_GAMG_INTERFACE_HPP_
#define MESH_TOOLS_REGION_COUPLED_GAMG_INTERFACES_REGION_COUPLED_GAMG_INTERFACE_REGION_COUPLED_BASE_GAMG_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionCoupledBaseGAMGInterface
// Description
//   Base class for GAMG agglomerated coupled region interface.
// SourceFiles
//   region_coupled_base_gamg_interface.cpp
#include "gamg_interface.hpp"
#include "region_coupled_ldu_interface.hpp"
#include "gamg_agglomeration.hpp"
namespace mousse
{
class regionCoupledBaseGAMGInterface
:
  public GAMGInterface,
  virtual public regionCoupledLduInterface
{
  // Private data
    //- Reference for the regionCoupledBaseFvPatch from which this is
    //  agglomerated
    const regionCoupledLduInterface& fineRegionCoupledLduInterface_;
    //- AMI interface
    //autoPtr<AMIPatchToPatchInterpolation> amiPtr_;
public:
  // Constructors
    //- Construct from fine level interface,
    //  local and neighbour restrict addressing
    regionCoupledBaseGAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& restrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );
    //- Disallow default bitwise copy construct
    regionCoupledBaseGAMGInterface
    (
      const regionCoupledBaseGAMGInterface&
    ) = delete;
    //- Disallow default bitwise assignment
    regionCoupledBaseGAMGInterface& operator=
    (
      const regionCoupledBaseGAMGInterface&
    ) = delete;
  //- Destructor
  virtual ~regionCoupledBaseGAMGInterface();
  // Member Functions
    // Interface transfer functions
      //- Transfer and return internal field adjacent to the interface
      virtual tmp<labelField> internalFieldTransfer
      (
        const Pstream::commsTypes commsType,
        const labelUList& iF
      ) const;
    //- Cyclic interface functions
      //- Return neigbour processor number
      virtual label neighbPatchID() const
      {
        return fineRegionCoupledLduInterface_.neighbPatchID();
      }
      virtual bool owner() const
      {
        return fineRegionCoupledLduInterface_.owner();
      }
      virtual const regionCoupledBaseGAMGInterface& neighbPatch() const
      {
        return dynamic_cast<const regionCoupledBaseGAMGInterface&>
        (
          fineRegionCoupledLduInterface_.neighbPatch()
        );
      }
      //virtual const AMIPatchToPatchInterpolation& AMI() const
      //{
      //    return amiPtr_();
      //}
      virtual const polyMesh& nbrMesh() const
      {
        return fineRegionCoupledLduInterface_.nbrMesh();
      }
      const lduInterface& nbrLduInterface
      (
        const label fineLevelIndex,
        const label index
      ) const
      {
        const GAMGAgglomeration& agg = nbrMesh().thisDb().lookupObject
        <
          GAMGAgglomeration
        >
        (
          GAMGAgglomeration::typeName
        );
        const lduMesh& mesh = agg.meshLevel(fineLevelIndex);
        return mesh.interfaces()[index];
      }
};
}  // namespace mousse
#endif
