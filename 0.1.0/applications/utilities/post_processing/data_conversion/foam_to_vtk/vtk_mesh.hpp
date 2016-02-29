#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_VTK_MESH_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_VTK_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vtkMesh
// Description
//   Encapsulation of VTK mesh data. Holds mesh or meshsubset and
//   polyhedral-cell decomposition on it.
// SourceFiles
//   vtk_mesh.cpp
#include "vtk_topo.hpp"
#include "fv_mesh_subset.hpp"
namespace mousse
{
// Forward declaration of classes
class Time;
class vtkMesh
{
  // Private data
    //- Reference to mesh
    fvMesh& baseMesh_;
    //- Subsetting engine + sub-fvMesh
    fvMeshSubset subsetter_;
    //- Current cellSet (or empty)
    const word setName_;
    //- Current decomposition of topology
    mutable autoPtr<vtkTopo> topoPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    vtkMesh(const vtkMesh&);
    //- Disallow default bitwise assignment
    void operator=(const vtkMesh&);
public:
  // Constructors
    //- Construct from components
    vtkMesh(fvMesh& baseMesh, const word& setName = "");
  // Member Functions
    // Access
      //- Whole mesh
      const fvMesh& baseMesh() const
      {
        return baseMesh_;
      }
      const fvMeshSubset& subsetter() const
      {
        return subsetter_;
      }
      //- Check if running subMesh
      bool useSubMesh() const
      {
        return setName_.size();
      }
      //- topology
      const vtkTopo& topo() const
      {
        if (topoPtr_.empty())
        {
          topoPtr_.reset(new vtkTopo(mesh()));
        }
        return topoPtr_();
      }
      //- Access either mesh or submesh
      const fvMesh& mesh() const
      {
        if (useSubMesh())
        {
          return subsetter_.subMesh();
        }
        else
        {
          return baseMesh_;
        }
      }
      //- Number of field cells
      label nFieldCells() const
      {
        return topo().cellTypes().size();
      }
      //- Number of field points
      label nFieldPoints() const
      {
        return mesh().nPoints() + topo().addPointCellLabels().size();
      }
    // Edit
      //- Read mesh
      polyMesh::readUpdateState readUpdate();
      //- Map volume field (does in fact do very little interpolation;
      //  just copied from fvMeshSubset)
      template<class GeoField>
      tmp<GeoField> interpolate(const GeoField& fld) const
      {
        if (useSubMesh())
        {
          tmp<GeoField> subFld = subsetter_.interpolate(fld);
          subFld().rename(fld.name());
          return subFld;
        }
        else
        {
          return fld;
        }
      }
};
}  // namespace mousse
#endif
