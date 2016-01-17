// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directMethod
// Description
//   Direct (one-to-one cell correspondence) mesh-to-mesh interpolation class
//   Volume conservative.
// SourceFiles
//   direct_method.cpp
#ifndef direct_method_hpp_
#define direct_method_hpp_
#include "mesh_to_mesh_method.hpp"
namespace mousse
{
class directMethod
:
  public meshToMeshMethod
{
protected:
  // Protected Member Functions
    //- Return the true if cells intersect
    virtual bool intersect
    (
      const label srcCellI,
      const label tgtCellI
    ) const;
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
    //- Append to list of src mesh seed indices
    virtual void appendToDirectSeeds
    (
      boolList& mapFlag,
      labelList& srcTgtSeed,
      DynamicList<label>& srcSeeds,
      label& srcSeedI,
      label& tgtSeedI
    ) const;
public:
  //- Run-time type information
  TYPE_NAME("direct");
  // Constructors
    //- Construct from source and target meshes
    directMethod(const polyMesh& src, const polyMesh& tgt);
    //- Disallow default bitwise copy construct
    directMethod(const directMethod&) = delete;
    //- Disallow default bitwise assignment
    directMethod& operator=(const directMethod&) = delete;
  //- Destructor
  virtual ~directMethod();
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
