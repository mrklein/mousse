// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshToMeshMethod
// Description
//   Base class for mesh-to-mesh calculation methods
// SourceFiles
//   mesh_to_mesh_method.cpp
#ifndef mesh_to_mesh_method_hpp_
#define mesh_to_mesh_method_hpp_
#include "poly_mesh.hpp"
namespace mousse
{
class meshToMeshMethod
{
protected:
  // Protected data
    //- Reference to the source mesh
    const polyMesh& src_;
    //- Reference to the target mesh
    const polyMesh& tgt_;
    //- Cell total volume in overlap region [m3]
    scalar V_;
    //- Tolerance used in volume overlap calculations
    static scalar tolerance_;
  // Protected Member Functions
    //- Return src cell IDs for the overlap region
    labelList maskCells() const;
    //- Return the true if cells intersect
    virtual bool intersect
    (
      const label srcCellI,
      const label tgtCellI
    ) const;
    //- Return the intersection volume between two cells
    virtual scalar interVol
    (
      const label srcCellI,
      const label tgtCellI
    ) const;
    //- Append target cell neihgbour cells to cellIDs list
    virtual void appendNbrCells
    (
      const label tgtCellI,
      const polyMesh& mesh,
      const DynamicList<label>& visitedTgtCells,
      DynamicList<label>& nbrTgtCellIDs
    ) const;
    virtual bool initialise
    (
      labelListList& srcToTgtAddr,
      scalarListList& srcToTgtWght,
      labelListList& tgtToTgtAddr,
      scalarListList& tgtToTgtWght
    ) const;
    //- Disallow default bitwise copy construct
    meshToMeshMethod(const meshToMeshMethod&);
    //- Disallow default bitwise assignment
    void operator=(const meshToMeshMethod&);
public:
  //- Run-time type information
  TYPE_NAME("meshToMeshMethod");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    meshToMeshMethod,
    components,
    (
      const polyMesh& src,
      const polyMesh& tgt
    ),
    (src, tgt)
  );
  //- Construct from source and target meshes
  meshToMeshMethod(const polyMesh& src, const polyMesh& tgt);
  //- Selector
  static autoPtr<meshToMeshMethod> New
  (
    const word& methodName,
    const polyMesh& src,
    const polyMesh& tgt
  );
  //- Destructor
  virtual ~meshToMeshMethod();
  // Member Functions
    // Evaluate
      //- Calculate addressing and weights
      virtual void calculate
      (
        labelListList& srcToTgtAddr,
        scalarListList& srcToTgtWght,
        labelListList& tgtToTgtAddr,
        scalarListList& tgtToTgtWght
      ) = 0;
    // Access
      //- Return const access to the source mesh
      inline const polyMesh& src() const;
      //- Return const access to the target mesh
      inline const polyMesh& tgt() const;
      //- Return const access to the overlap volume
      inline scalar V() const;
    // Check
      //- Write the connectivity (debugging)
      void writeConnectivity
      (
        const polyMesh& mesh1,
        const polyMesh& mesh2,
        const labelListList& mesh1ToMesh2Addr
      ) const;
};
}  // namespace mousse
#include "mesh_to_mesh_method_i.hpp"
#endif
