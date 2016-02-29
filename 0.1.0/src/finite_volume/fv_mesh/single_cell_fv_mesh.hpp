#ifndef FINITE_VOLUME_FV_MESH_SINGLE_CELL_FV_MESH_HPP_
#define FINITE_VOLUME_FV_MESH_SINGLE_CELL_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::singleCellFvMesh
// Description
//   fvMesh as subset of other mesh. Consists of one cell and all original
//   bounday faces. Useful when manipulating boundary data. Single internal
//   cell only needed to be able to manipulate in a standard way.
// SourceFiles
//   single_cell_fv_mesh.cpp
//   single_cell_fv_mesh_interpolate.cpp


#include "fv_patch_field_mapper.hpp"
#include "fv_mesh.hpp"
#include "label_list_io_list.hpp"

namespace mousse
{
class singleCellFvMesh
:
  public fvMesh
{
  // Private data
    const labelListIOList patchFaceAgglomeration_;
    //- From patch faces back to agglomeration or fine mesh
    labelListIOList patchFaceMap_;
    //- From fine mesh faces to coarse mesh
    labelIOList reverseFaceMap_;
    //- From coarse points back to original mesh
    labelIOList pointMap_;
    //- From fine points to coarse mesh
    labelIOList reversePointMap_;
  // Private Member Functions
    //- Calculate agglomerated mesh
    void agglomerateMesh(const fvMesh&, const labelListList&);
public:
    //- Patch field mapper class for agglomerated meshes
    class agglomPatchFieldMapper
    :
      public fvPatchFieldMapper
    {
      // Private data
        const labelListList& addressing_;
        const scalarListList& weights_;
        bool hasUnmapped_;
    public:
        //- Construct given addressing
        agglomPatchFieldMapper
        (
          const labelListList& addressing,
          const scalarListList& weights
        )
        :
          addressing_{addressing},
          weights_{weights},
          hasUnmapped_{false}
        {
          FOR_ALL(addressing_, i)
          {
            if (addressing_[i].empty())
            {
              hasUnmapped_ = true;
              break;
            }
          }
        }
        virtual label size() const
        {
          return addressing_.size();
        }
        virtual bool direct() const
        {
          return false;
        }
        bool hasUnmapped() const
        {
          return hasUnmapped_;
        }
        virtual const labelListList& addressing() const
        {
          return addressing_;
        }
        virtual const scalarListList& weights() const
        {
          return weights_;
        }
    };
  // Constructors
    //- Construct from fvMesh and no agglomeration
    singleCellFvMesh(const IOobject& io, const fvMesh&);
    //- Construct from fvMesh and agglomeration of boundary faces.
    //  agglomeration is per patch, per patch face index the agglomeration
    //  the face goes into.
    singleCellFvMesh
    (
      const IOobject& io,
      const fvMesh&,
      const labelListList& patchFaceAgglomeration
    );
    //- Read from IOobject
    singleCellFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    singleCellFvMesh(const singleCellFvMesh&) = delete;
    //- Disallow default bitwise assignment
    singleCellFvMesh& operator=(const singleCellFvMesh&) = delete;
  // Member Functions
    bool agglomerate() const
    {
      return patchFaceAgglomeration_.size() > 0;
    }
    //- From patchFace on this back to original mesh or agglomeration
    const labelListList& patchFaceMap() const
    {
      return patchFaceMap_;
    }
    //- From point on this back to original mesh
    const labelList& pointMap() const
    {
      return pointMap_;
    }
    //- From face on original mesh to face on this
    const labelList& reverseFaceMap() const
    {
      return reverseFaceMap_;
    }
    //- From point on original mesh to point on this (or -1 for removed
    //  points)
    const labelList& reversePointMap() const
    {
      return reversePointMap_;
    }
    //- Map volField. Internal field set to average, patch fields straight
    //  copies.
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> >
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace mousse

#ifdef NoRepository
#   include "single_cell_fv_mesh_interpolate.cpp"
#endif

#endif
