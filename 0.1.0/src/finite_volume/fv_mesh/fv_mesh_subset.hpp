// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMeshSubset
// Description
//   Post-processing mesh subset tool.  Given the original mesh and the
//   list of selected cells, it creates the mesh consisting only of the
//   desired cells, with the mapping list for points, faces, and cells.
//   Puts all exposed internal faces into either
//   - a user supplied patch
//   - a newly created patch "oldInternalFaces"
//   - setCellSubset is for small subsets. Uses Maps to minimize memory.
//   - setLargeCellSubset is for largish subsets (>10% of mesh).
//    Uses labelLists instead.
//   - setLargeCellSubset does coupled patch subsetting as well. If it detects
//    a face on a coupled patch 'losing' its neighbour it will move the
//    face into the oldInternalFaces patch.
//   - if a user supplied patch is used the mapping becomes a problem.
//   Do the new faces get the value of the internal face they came from?
//   What if e.g. the user supplied patch is a fixedValue 0? So for now
//   they get the face of existing patch face 0.
// SourceFiles
//   fv_mesh_subset.cpp
#ifndef fv_mesh_subset_hpp_
#define fv_mesh_subset_hpp_
#include "fv_mesh.hpp"
#include "point_mesh.hpp"
#include "geometric_field.hpp"
#include "hash_set.hpp"
#include "surface_mesh.hpp"
namespace mousse
{
class fvMeshSubset
{
private:
  // Private data
    //- Mesh to subset from
    const fvMesh& baseMesh_;
    //- Subset mesh pointer
    autoPtr<fvMesh> fvMeshSubsetPtr_;
    //- Point mapping array
    labelList pointMap_;
    //- Face mapping array
    labelList faceMap_;
    //- Cell mapping array
    labelList cellMap_;
    //- Patch mapping array
    labelList patchMap_;
  // Private Member Functions
    //- Check if subset has been performed
    bool checkCellSubset() const;
    //- Mark points in Map
    static void markPoints(const labelList&, Map<label>&);
    //- Mark points (with 0) in labelList
    static void markPoints(const labelList&, labelList&);
    //- Adapt nCellsUsingFace for coupled faces becoming 'uncoupled'.
    void doCoupledPatches
    (
      const bool syncPar,
      labelList& nCellsUsingFace
    ) const;
    //- Subset of subset
    static labelList subset
    (
      const label nElems,
      const labelList& selectedElements,
      const labelList& subsetMap
    );
    //- Create zones for submesh
    void subsetZones();
public:
  // Constructors
    //- Construct given a mesh to subset
    explicit fvMeshSubset(const fvMesh&);
    //- Disallow default bitwise copy construct
    fvMeshSubset(const fvMeshSubset&) = delete;
    //- Disallow default bitwise assignment
    fvMeshSubset& operator=(const fvMeshSubset&) = delete;
  // Member Functions
    // Edit
      //- Set the subset. Create "oldInternalFaces" patch for exposed
      //  internal faces (patchID==-1) or use supplied patch.
      //  Does not handle coupled patches correctly if only one side
      //  gets deleted.
      void setCellSubset
      (
        const labelHashSet& globalCellMap,
        const label patchID = -1
      );
      //- Set the subset from all cells with region == currentRegion.
      //  Create "oldInternalFaces" patch for exposed
      //  internal faces (patchID==-1) or use supplied patch.
      //  Handles coupled patches by if necessary making coupled patch
      //  face part of patchID (so uncoupled)
      void setLargeCellSubset
      (
        const labelList& region,
        const label currentRegion,
        const label patchID = -1,
        const bool syncCouples = true
      );
      //- setLargeCellSubset but with labelHashSet.
      void setLargeCellSubset
      (
        const labelHashSet& globalCellMap,
        const label patchID = -1,
        const bool syncPar = true
      );
    // Access
      //- Original mesh
      const fvMesh& baseMesh() const
      {
        return baseMesh_;
      }
      //- Have subMesh?
      bool hasSubMesh() const;
      //- Return reference to subset mesh
      const fvMesh& subMesh() const;
      fvMesh& subMesh();
      //- Return point map
      const labelList& pointMap() const;
      //- Return face map
      const labelList& faceMap() const;
      //- Return cell map
      const labelList& cellMap() const;
      //- Return patch map
      const labelList& patchMap() const;
    // Field mapping
      //- Map volume field
      template<class Type>
      static tmp<GeometricField<Type, fvPatchField, volMesh> >
      interpolate
      (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const fvMesh& sMesh,
        const labelList& patchMap,
        const labelList& cellMap,
        const labelList& faceMap
      );
      template<class Type>
      tmp<GeometricField<Type, fvPatchField, volMesh> >
      interpolate
      (
        const GeometricField<Type, fvPatchField, volMesh>&
      ) const;
      //- Map surface field
      template<class Type>
      static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
      interpolate
      (
        const GeometricField<Type, fvsPatchField, surfaceMesh>&,
        const fvMesh& sMesh,
        const labelList& patchMap,
        const labelList& faceMap
      );
      template<class Type>
      tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
      interpolate
      (
        const GeometricField<Type, fvsPatchField, surfaceMesh>&
      ) const;
      //- Map point field
      template<class Type>
      static tmp<GeometricField<Type, pointPatchField, pointMesh> >
      interpolate
      (
        const GeometricField<Type, pointPatchField, pointMesh>&,
        const pointMesh& sMesh,
        const labelList& patchMap,
        const labelList& pointMap
      );
      template<class Type>
      tmp<GeometricField<Type, pointPatchField, pointMesh> >
      interpolate
      (
        const GeometricField<Type, pointPatchField, pointMesh>&
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_mesh_subset_interpolate.cpp"
#endif
#endif
