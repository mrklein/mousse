#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_VTK_TOPO_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_VTK_TOPO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vtkTopo
// Description
//   Polyhedral cell decomposition for VTK.
// SourceFiles
//   vtk_topo.cpp
#include "label_list.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class vtkTopo
{
  // Private data
    const polyMesh& mesh_;
    //- Vertices per cell (including added cells) in vtk ordering
    labelListList vertLabels_;
    //- Cell types (including added cells) in vtk numbering
    labelList cellTypes_;
    labelList addPointCellLabels_;
    labelList superCells_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    vtkTopo(const vtkTopo&);
    //- Disallow default bitwise assignment
    void operator=(const vtkTopo&);
public:
 // Public static data
    //- Equivalent to enumeration in "vtkCellType.h"
    enum vtkTypes
    {
      VTK_TRIANGLE   = 5,
      VTK_POLYGON    = 7,
      VTK_QUAD       = 9,
      VTK_TETRA      = 10,
      VTK_HEXAHEDRON = 12,
      VTK_WEDGE      = 13,
      VTK_PYRAMID    = 14,
      VTK_POLYHEDRON = 42
    };
  //- Enable/disable polyhedron decomposition. Default = true
  static bool decomposePoly;
  // Constructors
    //- Construct from components
    vtkTopo(const polyMesh&);
  // Member Functions
    // Access
      const labelListList& vertLabels() const
      {
        return vertLabels_;
      }
      const labelList& cellTypes() const
      {
        return cellTypes_;
      }
      const labelList& addPointCellLabels() const
      {
        return addPointCellLabels_;
      }
      const labelList& superCells() const
      {
        return superCells_;
      }
};
}  // namespace mousse
#endif
