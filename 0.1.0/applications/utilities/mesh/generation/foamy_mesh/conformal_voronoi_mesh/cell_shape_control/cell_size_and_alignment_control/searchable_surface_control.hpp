#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_SEARCHABLE_SURFACE_CONTROL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_SEARCHABLE_SURFACE_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceControl
// Description
// SourceFiles
//   searchable_surface_control.cpp
#include "cell_size_function.hpp"
#include "triad.hpp"
namespace mousse
{
class searchableSurfaceControl
:
  public cellSizeAndAlignmentControl
{
  // Private data
    //- Name of the surface
    const word surfaceName_;
    //- Reference to the searchableSurface object holding the geometry data
    const searchableSurface& searchableSurface_;
    const conformationSurfaces& geometryToConformTo_;
    PtrList<cellSizeFunction> cellSizeFunctions_;
    labelList regionToCellSizeFunctions_;
    label maxPriority_;
public:
  //- Runtime type information
  TYPE_NAME("searchableSurfaceControl");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    searchableSurfaceControl
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    searchableSurfaceControl(const searchableSurfaceControl&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const searchableSurfaceControl&) = delete;
  //- Destructor
  ~searchableSurfaceControl();
  // Member Functions
    // Access
    virtual void cellSizeFunctionVertices
    (
      DynamicList<mousse::point>& pts,
      DynamicList<scalar>& sizes
    ) const;
    virtual void initialVertices
    (
      pointField& pts,
      scalarField& sizes,
      triadField& alignments
    ) const;
    const PtrList<cellSizeFunction>& sizeFunctions() const
    {
      return cellSizeFunctions_;
    }
    virtual label maxPriority() const
    {
      return maxPriority_;
    }
    bool cellSize
    (
      const mousse::point& pt,
      scalar& cellSize,
      label& priority
    ) const;
    // Edit
};
}  // namespace mousse
#endif
