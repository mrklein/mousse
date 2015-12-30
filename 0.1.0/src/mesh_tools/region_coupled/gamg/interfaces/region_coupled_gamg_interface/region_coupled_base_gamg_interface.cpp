// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ami_interpolation.hpp"
#include "region_coupled_base_gamg_interface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map.hpp"
#include "poly_mesh.hpp"
// Constructors 
mousse::regionCoupledBaseGAMGInterface::regionCoupledBaseGAMGInterface
(
  const label index,
  const lduInterfacePtrsList& coarseInterfaces,
  const lduInterface& fineInterface,
  const labelField& localRestrictAddressing,
  const labelField& neighbourRestrictAddressing,
  const label fineLevelIndex,
  const label coarseComm
)
:
  GAMGInterface
  (
    index,
    coarseInterfaces
  ),
  fineRegionCoupledLduInterface_
  (
    refCast<const regionCoupledLduInterface>(fineInterface)
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
    forAll(localRestrictAddressing, ffi)
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
  /*
  // On the owner side construct the AMI
  if (fineRegionCoupledLduInterface_.owner())
  {
    const polyMesh& nbrMesh =
      fineRegionCoupledLduInterface_.nbrMesh();
    if
    (
      nbrMesh.foundObject<GAMGAgglomeration>(GAMGAgglomeration::typeName)
    )
    {
      const GAMGAgglomeration& nbrAgg = nbrMesh.thisDb().lookupObject
      <
        GAMGAgglomeration
      >
      (
        GAMGAgglomeration::typeName
      );
      label nbrLevel(-1);
      if (nbrAgg.size() > fineLevelIndex)
      {
        nbrLevel = fineLevelIndex;
      }
      else
      {
        nbrLevel = nbrAgg.size() - 1;
      }
      const labelField& nbrRestrictMap =
        nbrAgg.restrictAddressing(nbrLevel);
      const labelUList& nbrFaceCells =
        nbrLduInterface
        (
          nbrLevel,
          neighbPatchID()
        ).faceCells();
      const IndirectList<label> nbrPatchRestrictMap
      (
        nbrRestrictMap,
        nbrFaceCells
      );
      labelList nbrFaceRestrictAddressing;
      {
        // From face to coarse face
        DynamicList<label> dynNbrFaceRestrictAddressing
        (
          nbrPatchRestrictMap.size()
        );
        Map<label> masterToCoarseFace(nbrPatchRestrictMap.size());
        forAll(nbrPatchRestrictMap, ffi)
        {
          label curMaster = nbrPatchRestrictMap[ffi];
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
        nbrFaceRestrictAddressing.transfer
        (
          dynNbrFaceRestrictAddressing
        );
      }
      amiPtr_.reset
      (
        new AMIPatchToPatchInterpolation
        (
          fineRegionCoupledLduInterface_.AMI(),
          faceRestrictAddressing_,
          nbrFaceRestrictAddressing
        )
      );
    }
    else
    {
      FatalErrorIn
      (
        "regionCoupledBaseGAMGInterface::"
        "regionCoupledBaseGAMGInterface"
        "("
        "const label index,"
        "const lduInterfacePtrsList& coarseInterfaces,"
        "const lduInterface& fineInterface,"
        "const labelField& localRestrictAddressing,"
        "const labelField& neighbourRestrictAddressing,"
        "const label fineLevelIndex"
        ")"
      )   << " GAMGAgglomeration was not found in the nbr mesh. "
        << " Check on the cacheAgglomeration flag in fvSolution"
        << exit(FatalError);
    }
  }
  */
}
// Desstructor
mousse::regionCoupledBaseGAMGInterface::~regionCoupledBaseGAMGInterface()
{}
// Member Functions 
mousse::tmp<mousse::labelField> mousse::regionCoupledBaseGAMGInterface::
internalFieldTransfer
(
  const Pstream::commsTypes,
  const labelUList& iF
) const
{
//     WarningIn
//     (
//         "regionCoupledBaseGAMGInterface::internalFieldTransfer"
//         "( const Pstream::commsTypes, const labelUList&)"
//         " the internal field can not be transfered "
//         " as the neighbFvPatch are in different meshes "
//     );
  /*
  //const labelUList& nbrFaceCells = neighbPatch().faceCells();
  const labelUList& nbrFaceCells = nbrLduInterface().faceCells();
  tmp<labelField> tpnf(new labelField(nbrFaceCells.size()));
  labelField& pnf = tpnf();
  forAll(pnf, facei)
  {
    pnf[facei] = iF[nbrFaceCells[facei]];
  }
  */
  tmp<labelField> tpnf(new labelField(iF));
  return tpnf;
}
