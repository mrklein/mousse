// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mapNearestMethod
// Description
//   Map nearest mesh-to-mesh interpolation class
//   Not volume conservative.
//   - cells outside other meshes bounding box do not get mapped
//    (initial filtering)
//   - all remaining cells will be mapped (with weight 1!)
//   - so take care when mapping meshes with different bounding boxes!
// SourceFiles
//   map_nearest_method.cpp
#ifndef map_nearest_method_hpp_
#define map_nearest_method_hpp_
#include "mesh_to_mesh_method.hpp"
namespace mousse
{
class mapNearestMethod
:
  public meshToMeshMethod
{
protected:
  // Protected Member Functions
    //- Find indices of overlapping cells in src and tgt meshes - returns
    //  true if found a matching pair
    virtual bool findInitialSeeds
    (
      const labelList& srcCellIDs,
      const boolList& mapFlag,
      const label startSeedI,
      label& srcSeedI,
      label& tgtSeedI
    ) const;
    //- Calculate the mesh-to-mesh addressing and weights
    virtual void calculateAddressing
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
    //- Find the nearest cell on mesh2 for cell1 on mesh1
    virtual void findNearestCell
    (
      const polyMesh& mesh1,
      const polyMesh& mesh2,
      const label cell1,
      label& cell2
    ) const;
    //- Set the next cells for the marching front algorithm
    virtual void setNextNearestCells
    (
      label& startSeedI,
      label& srcCellI,
      label& tgtCellI,
      boolList& mapFlag,
      const labelList& srcCellIDs
    ) const;
    //- Find a source cell mapped to target cell tgtCellI
    virtual label findMappedSrcCell
    (
      const label tgtCellI,
      const List<DynamicList<label> >& tgtToSrc
    ) const;
    //- Disallow default bitwise copy construct
    mapNearestMethod(const mapNearestMethod&);
    //- Disallow default bitwise assignment
    void operator=(const mapNearestMethod&);
public:
  //- Run-time type information
  TYPE_NAME("mapNearest");
  //- Construct from source and target meshes
  mapNearestMethod(const polyMesh& src, const polyMesh& tgt);
  //- Destructor
  virtual ~mapNearestMethod();
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
