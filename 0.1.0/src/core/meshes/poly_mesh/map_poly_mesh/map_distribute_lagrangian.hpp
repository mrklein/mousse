#ifndef CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_DISTRIBUTE_LAGRANGIAN_HPP_
#define CORE_MESHES_POLY_MESH_MAP_POLY_MESH_MAP_DISTRIBUTE_LAGRANGIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapDistributeLagrangian
// Description
//   Class containing mesh-to-mesh mapping information for particles
// SourceFiles
//   map_distribute_lagrangian.cpp
#include "map_distribute.hpp"
namespace mousse
{
class mapPolyMesh;
class mapDistributeLagrangian
{
  // Private data
    //- Map to distribute particles
    const mapDistribute particleMap_;
    //- Per element in subsetted mesh the cell label
    const labelListList constructCellLabels_;
public:
  // Constructors
    //- Construct from components
    mapDistributeLagrangian
    (
      const label nNewParticles,
      const Xfer<labelListList>& subParticleMap,
      const Xfer<labelListList>& constructParticleMap,
      const Xfer<labelListList>& constructCellLabels
    )
    :
      particleMap_(nNewParticles, subParticleMap, constructParticleMap),
      constructCellLabels_(constructCellLabels)
    {}
  // Member Functions
    // Access
      //- Distribution map
      const mapDistribute& particleMap() const
      {
        return particleMap_;
      }
      //- Per received particle the destination cell label
      const labelListList& constructCellLabels() const
      {
        return constructCellLabels_;
      }
    // Edit
      //- Distribute list of lagrangian data
      template<class T>
      void distributeLagrangianData(List<T>& lst) const
      {
        particleMap_.distribute(lst);
      }
      //- Correct for topo change.
      void updateMesh(const mapPolyMesh&)
      {
        notImplemented
        (
          "mapDistributeLagrangian::updateMesh(const mapPolyMesh&)"
        );
      }
};
}  // namespace mousse
#endif
