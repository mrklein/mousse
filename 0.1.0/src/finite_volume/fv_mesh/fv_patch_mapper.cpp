// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_patch_mapper.hpp"
#include "fv_patch.hpp"
#include "fv_boundary_mesh.hpp"
#include "fv_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "face_mapper.hpp"
#include "demand_driven_data.hpp"
// Private Member Functions 
void mousse::fvPatchMapper::calcAddressing() const
{
  if
  (
    directAddrPtr_
  || interpolationAddrPtr_
  || weightsPtr_
  )
  {
    FATAL_ERROR_IN
    (
      "void fvPatchMapper::calcAddressing() const)"
    )   << "Addressing already calculated"
      << abort(FatalError);
  }
  // Mapping
  const label oldPatchStart =
    faceMap_.oldPatchStarts()[patch_.index()];
  const label oldPatchEnd =
    oldPatchStart + faceMap_.oldPatchSizes()[patch_.index()];
  hasUnmapped_ = false;
  // Assemble the maps: slice to patch
  if (direct())
  {
    // Direct mapping - slice to size
    directAddrPtr_ = new labelList
    (
      patch_.patchSlice
      (
        static_cast<const labelList&>(faceMap_.directAddressing())
      )
    );
    labelList& addr = *directAddrPtr_;
    // Adjust mapping to manage hits into other patches and into
    // internal
    FOR_ALL(addr, faceI)
    {
      if
      (
        addr[faceI] >= oldPatchStart
      && addr[faceI] < oldPatchEnd
      )
      {
        addr[faceI] -= oldPatchStart;
      }
      else
      {
        //addr[faceI] = 0;
        addr[faceI] = -1;
        hasUnmapped_ = true;
      }
    }
    if (fvMesh::debug)
    {
      if (min(addr) < 0)
      {
        //FatalErrorIn
        WARNING_IN
        (
          "void fvPatchMapper::calcAddressing() const"
        )   << "Unmapped entry in patch mapping for patch "
          << patch_.index() << " named " << patch_.name()
          //<< abort(FatalError);
          << endl;
      }
    }
  }
  else
  {
    // Interpolative mapping
    interpolationAddrPtr_ =
      new labelListList
      (
        patch_.patchSlice(faceMap_.addressing())
      );
    labelListList& addr = *interpolationAddrPtr_;
    weightsPtr_ =
      new scalarListList
      (
        patch_.patchSlice(faceMap_.weights())
      );
    scalarListList& w = *weightsPtr_;
    // Adjust mapping to manage hits into other patches and into
    // internal
    FOR_ALL(addr, faceI)
    {
      labelList& curAddr = addr[faceI];
      scalarList& curW = w[faceI];
      if
      (
        min(curAddr) >= oldPatchStart
      && max(curAddr) < oldPatchEnd
      )
      {
        // No adjustment of weights, just subtract patch start
        FOR_ALL(curAddr, i)
        {
          curAddr[i] -= oldPatchStart;
        }
      }
      else
      {
        // Need to recalculate weights to exclude hits into internal
        labelList newAddr(curAddr.size(), false);
        scalarField newWeights(curAddr.size());
        label nActive = 0;
        FOR_ALL(curAddr, lfI)
        {
          if
          (
            curAddr[lfI] >= oldPatchStart
          && curAddr[lfI] < oldPatchEnd
          )
          {
            newAddr[nActive] = curAddr[lfI] - oldPatchStart;
            newWeights[nActive] = curW[lfI];
            nActive++;
          }
        }
        //// Cater for bad mapping
        //if (nActive == 0)
        //{
        //    newAddr[nActive] = 0;
        //    newWeights[nActive] = 1;
        //    nActive++;
        //}
        newAddr.setSize(nActive);
        newWeights.setSize(nActive);
        // Re-scale the weights
        if (nActive > 0)
        {
          newWeights /= sum(newWeights);
        }
        else
        {
          hasUnmapped_ = true;
        }
        // Reset addressing and weights
        curAddr = newAddr;
        curW = newWeights;
      }
    }
    if (fvMesh::debug)
    {
      FOR_ALL(addr, i)
      {
        if (min(addr[i]) < 0)
        {
          FATAL_ERROR_IN
          (
            "void fvPatchMapper::calcAddressing() const"
          )   << "Error in patch mapping for patch "
            << patch_.index() << " named " << patch_.name()
            << abort(FatalError);
        }
      }
    }
  }
}
void mousse::fvPatchMapper::clearOut()
{
  deleteDemandDrivenData(directAddrPtr_);
  deleteDemandDrivenData(interpolationAddrPtr_);
  deleteDemandDrivenData(weightsPtr_);
  hasUnmapped_ = false;
}
// Constructors 
// Construct from components
mousse::fvPatchMapper::fvPatchMapper
(
  const fvPatch& patch,
  const faceMapper& faceMap
)
:
  patch_(patch),
  faceMap_(faceMap),
  sizeBeforeMapping_(faceMap.oldPatchSizes()[patch_.index()]),
  hasUnmapped_(false),
  directAddrPtr_(NULL),
  interpolationAddrPtr_(NULL),
  weightsPtr_(NULL)
{}
// Destructor 
mousse::fvPatchMapper::~fvPatchMapper()
{
  clearOut();
}
// Member Functions 
const mousse::labelUList& mousse::fvPatchMapper::directAddressing() const
{
  if (!direct())
  {
    FATAL_ERROR_IN
    (
      "const labelUList& fvPatchMapper::directAddressing() const"
    )   << "Requested direct addressing for an interpolative mapper."
      << abort(FatalError);
  }
  if (!directAddrPtr_)
  {
    calcAddressing();
  }
  return *directAddrPtr_;
}
const mousse::labelListList& mousse::fvPatchMapper::addressing() const
{
  if (direct())
  {
    FATAL_ERROR_IN
    (
      "const labelListList& fvPatchMapper::addressing() const"
    )   << "Requested interpolative addressing for a direct mapper."
      << abort(FatalError);
  }
  if (!interpolationAddrPtr_)
  {
    calcAddressing();
  }
  return *interpolationAddrPtr_;
}
const mousse::scalarListList& mousse::fvPatchMapper::weights() const
{
  if (direct())
  {
    FATAL_ERROR_IN
    (
      "const scalarListList& fvPatchMapper::weights() const"
    )   << "Requested interpolative weights for a direct mapper."
      << abort(FatalError);
  }
  if (!weightsPtr_)
  {
    calcAddressing();
  }
  return *weightsPtr_;
}
// Member Operators 
// Friend Functions 
// Friend Operators
