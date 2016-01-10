// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ami_interpolation.hpp"
#include "cyclic_amigamg_interface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cyclicAMIGAMGInterface, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterface,
  cyclicAMIGAMGInterface,
  lduInterface
);
}

// Constructors 
mousse::cyclicAMIGAMGInterface::cyclicAMIGAMGInterface
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
  GAMGInterface
  (
    index,
    coarseInterfaces
  ),
  fineCyclicAMIInterface_
  (
    refCast<const cyclicAMILduInterface>(fineInterface)
  )
{
  // Construct face agglomeration from cell agglomeration
  {
    // From coarse face to cell
    DynamicList<label> dynFaceCells(localRestrictAddressing.size());
    // From face to coarse face
    DynamicList<label> dynFaceRestrictAddressing
    (
      localRestrictAddressing.size()
    );
    Map<label> masterToCoarseFace(localRestrictAddressing.size());
    FOR_ALL(localRestrictAddressing, ffi)
    {
      label curMaster = localRestrictAddressing[ffi];
      Map<label>::const_iterator fnd = masterToCoarseFace.find
      (
        curMaster
      );
      if (fnd == masterToCoarseFace.end())
      {
        // New coarse face
        label coarseI = dynFaceCells.size();
        dynFaceRestrictAddressing.append(coarseI);
        dynFaceCells.append(curMaster);
        masterToCoarseFace.insert(curMaster, coarseI);
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
  // On the owner side construct the AMI
  if (fineCyclicAMIInterface_.owner())
  {
    // Construct the neighbour side agglomeration (as the neighbour would
    // do it so it the exact loop above using neighbourRestrictAddressing
    // instead of localRestrictAddressing)
    labelList nbrFaceRestrictAddressing;
    {
      // From face to coarse face
      DynamicList<label> dynNbrFaceRestrictAddressing
      (
        neighbourRestrictAddressing.size()
      );
      Map<label> masterToCoarseFace(neighbourRestrictAddressing.size());
      FOR_ALL(neighbourRestrictAddressing, ffi)
      {
        label curMaster = neighbourRestrictAddressing[ffi];
        Map<label>::const_iterator fnd = masterToCoarseFace.find
        (
          curMaster
        );
        if (fnd == masterToCoarseFace.end())
        {
          // New coarse face
          label coarseI = masterToCoarseFace.size();
          dynNbrFaceRestrictAddressing.append(coarseI);
          masterToCoarseFace.insert(curMaster, coarseI);
        }
        else
        {
          // Already have coarse face
          dynNbrFaceRestrictAddressing.append(fnd());
        }
      }
      nbrFaceRestrictAddressing.transfer(dynNbrFaceRestrictAddressing);
    }
    amiPtr_.reset
    (
      new AMIPatchToPatchInterpolation
      (
        fineCyclicAMIInterface_.AMI(),
        faceRestrictAddressing_,
        nbrFaceRestrictAddressing
      )
    );
  }
}
// Desstructor
mousse::cyclicAMIGAMGInterface::~cyclicAMIGAMGInterface()
{}
// Member Functions 
mousse::tmp<mousse::labelField> mousse::cyclicAMIGAMGInterface::internalFieldTransfer
(
  const Pstream::commsTypes,
  const labelUList& iF
) const
{
  const cyclicAMIGAMGInterface& nbr =
    dynamic_cast<const cyclicAMIGAMGInterface&>(neighbPatch());
  const labelUList& nbrFaceCells = nbr.faceCells();
  tmp<labelField> tpnf(new labelField(nbrFaceCells.size()));
  labelField& pnf = tpnf();
  FOR_ALL(pnf, facei)
  {
    pnf[facei] = iF[nbrFaceCells[facei]];
  }
  return tpnf;
}
