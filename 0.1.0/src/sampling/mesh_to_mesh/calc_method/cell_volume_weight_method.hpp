#ifndef SAMPLING_MESH_TO_MESH_CALC_METHOD_CELL_VOLUME_WEIGHT_METHOD_HPP_
#define SAMPLING_MESH_TO_MESH_CALC_METHOD_CELL_VOLUME_WEIGHT_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellVolumeWeightMethod
// Description
//   Cell-volume-weighted mesh-to-mesh interpolation class
//   Volume conservative.

#include "mesh_to_mesh_method.hpp"


namespace mousse {

class cellVolumeWeightMethod
:
  public meshToMeshMethod
{
protected:
  // Protected Member Functions
    //- Find indices of overlapping cells in src and tgt meshes - returns
    //  true if found a matching pair
    bool findInitialSeeds
    (
      const labelList& srcCellIDs,
      const boolList& mapFlag,
      const label startSeedI,
      label& srcSeedI,
      label& tgtSeedI
    ) const;
    //- Calculate the mesh-to-mesh addressing and weights
    void calculateAddressing
    (
      labelListList& srcToTgtCellAddr,
      scalarListList& srcToTgtCellWght,
      labelListList& tgtToSrcCellAddr,
      scalarListList& tgtToSrcCellWght,
      const label srcSeedI,
      const label tgtSeedI,
      const labelList& srcCellIDs,
      boolList& mapFlag,
      label& startSeedI
    );
    //- Set the next cells in the advancing front algorithm
    void setNextCells
    (
      label& startSeedI,
      label& srcCellI,
      label& tgtCellI,
      const labelList& srcCellIDs,
      const boolList& mapFlag,
      const DynamicList<label>& visitedCells,
      labelList& seedCells
    ) const;
public:
  //- Run-time type information
  TYPE_NAME("cellVolumeWeight");
  // Constructors
    //- Construct from source and target meshes
    cellVolumeWeightMethod(const polyMesh& src, const polyMesh& tgt);
    //- Disallow default bitwise copy construct
    cellVolumeWeightMethod(const cellVolumeWeightMethod&) = delete;
    //- Disallow default bitwise assignment
    cellVolumeWeightMethod& operator=(const cellVolumeWeightMethod&) = delete;
  //- Destructor
  virtual ~cellVolumeWeightMethod();
  // Member Functions
    // Evaluate
      //- Calculate addressing and weights
      virtual void calculate
      (
        labelListList& srcToTgtAddr,
        scalarListList& srcToTgtWght,
        labelListList& tgtToTgtAddr,
        scalarListList& tgtToTgtWght
      );
};

}  // namespace mousse

#endif

