// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_gamg_interface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "label_pair.hpp"
#include "hash_table.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicGAMGInterface, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  cyclicGAMGInterface,
  lduInterface
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  cyclicGAMGInterface,
  Istream
);

// Add under name cyclicSlip
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  cyclicGAMGInterface,
  lduInterface,
  cyclicSlip
);

ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  cyclicGAMGInterface,
  Istream,
  cyclicSlip
);

}

// Constructors
mousse::cyclicGAMGInterface::cyclicGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  const lduInterface& fineInterface,
  const labelField& localRestrictAddressing,
  const labelField& neighbourRestrictAddressing,
  const label /*fineLevelIndex*/,
  const label /*coarseComm*/
)
:
  GAMGInterface(index, coarseInterfaces),
  neighbPatchID_
  (
    refCast<const cyclicLduInterface>(fineInterface).neighbPatchID()
  ),
  owner_(refCast<const cyclicLduInterface>(fineInterface).owner()),
  forwardT_(refCast<const cyclicLduInterface>(fineInterface).forwardT()),
  reverseT_(refCast<const cyclicLduInterface>(fineInterface).reverseT())
{
  // From coarse face to coarse cell
  DynamicList<label> dynFaceCells(localRestrictAddressing.size());
  // From fine face to coarse face
  DynamicList<label> dynFaceRestrictAddressing
  (
    localRestrictAddressing.size()
  );
  // From coarse cell pair to coarse face
  HashTable<label, labelPair, labelPair::Hash<> > cellsToCoarseFace
  (
    2*localRestrictAddressing.size()
  );
  FOR_ALL(localRestrictAddressing, ffi)
  {
    labelPair cellPair;
    // Do switching on master/slave indexes based on the owner/neighbour of
    // the processor index such that both sides get the same answer.
    if (owner())
    {
      // Master side
      cellPair = labelPair
      (
        localRestrictAddressing[ffi],
        neighbourRestrictAddressing[ffi]
      );
    }
    else
    {
      // Slave side
      cellPair = labelPair
      (
        neighbourRestrictAddressing[ffi],
        localRestrictAddressing[ffi]
      );
    }
    HashTable<label, labelPair, labelPair::Hash<> >::const_iterator fnd =
      cellsToCoarseFace.find(cellPair);
    if (fnd == cellsToCoarseFace.end())
    {
      // New coarse face
      label coarseI = dynFaceCells.size();
      dynFaceRestrictAddressing.append(coarseI);
      dynFaceCells.append(localRestrictAddressing[ffi]);
      cellsToCoarseFace.insert(cellPair, coarseI);
    }
    else
    {
      // Already have coarse face
      dynFaceRestrictAddressing.append(fnd());
    }
  }
  faceCells_.transfer(dynFaceCells);
  faceRestrictAddressing_.transfer(dynFaceRestrictAddressing);
}
mousse::cyclicGAMGInterface::cyclicGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  Istream& is
)
:
  GAMGInterface(index, coarseInterfaces, is),
  neighbPatchID_(readLabel(is)),
  owner_(readBool(is)),
  forwardT_(is),
  reverseT_(is)
{}

// Desstructor
mousse::cyclicGAMGInterface::~cyclicGAMGInterface()
{}

// Member Functions
mousse::tmp<mousse::labelField> mousse::cyclicGAMGInterface::internalFieldTransfer
(
  const Pstream::commsTypes,
  const labelUList& iF
) const
{
  const cyclicGAMGInterface& nbr = neighbPatch();
  const labelUList& nbrFaceCells = nbr.faceCells();
  tmp<labelField> tpnf(new labelField(size()));
  labelField& pnf = tpnf();
  FOR_ALL(pnf, facei)
  {
    pnf[facei] = iF[nbrFaceCells[facei]];
  }
  return tpnf;
}
void mousse::cyclicGAMGInterface::write(Ostream& os) const
{
  GAMGInterface::write(os);
  os  << token::SPACE << neighbPatchID_
    << token::SPACE << owner_
    << token::SPACE << forwardT_
    << token::SPACE << reverseT_;
}
