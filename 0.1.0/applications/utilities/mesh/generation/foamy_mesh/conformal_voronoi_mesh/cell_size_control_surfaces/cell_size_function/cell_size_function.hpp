#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_CELL_SIZE_FUNCTION_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_CELL_SIZE_FUNCTION_CELL_SIZE_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSizeFunction
// Description
//   Abstract base class for specifying target cell sizes
// SourceFiles
//   cell_size_function.cpp
#include "point.hpp"
#include "conformal_voronoi_mesh.hpp"
#include "searchable_surface.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "surface_cell_size_function.hpp"
namespace mousse
{
class cellSizeFunction
:
  public dictionary
{
public:
  //- Surface side mode
  enum sideMode
  {
    smInside,         // Control inside the surface
    smOutside,        // Control outside the surface
    rmBothsides       // Control on both sides of a surface
  };
  //- Runtime type information
  TYPE_NAME("cellSizeFunction");
protected:
  // Static data
    //- Point closeness tolerance to a surface where the function "snaps" to
    //  including the surface
    static scalar snapToSurfaceTol_;
  // Protected data
    //- Reference to the searchableSurface that cellSizeFunction
    //  relates to
    const searchableSurface& surface_;
    //- Cell size at the surface
    scalarField surfaceCellSize_;
    autoPtr<surfaceCellSizeFunction> surfaceCellSizeFunction_;
    //- Method details dictionary
    dictionary coeffsDict_;
    const scalar& defaultCellSize_;
    //- Index of the region of the surface that this cell size function
    //  applies to
    const labelList regionIndices_;
    //- Mode of size specification, i.e. inside, outside or bothSides
    sideMode sideMode_;
    label priority_;
public:
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      cellSizeFunction,
      dictionary,
      (
        const dictionary& cellSizeFunctionDict,
        const searchableSurface& surface,
        const scalar& defaultCellSize,
        const labelList regionIndices
      ),
      (cellSizeFunctionDict, surface, defaultCellSize, regionIndices)
    );
  // Constructors
    //- Construct from components
    cellSizeFunction
    (
      const word& type,
      const dictionary& cellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
    //- Disallow default bitwise copy construct
    cellSizeFunction(const cellSizeFunction&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellSizeFunction&) = delete;
  // Selectors
    //- Return a reference to the selected cellSizeFunction
    static autoPtr<cellSizeFunction> New
    (
      const dictionary& cellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize,
      const labelList regionIndices
    );
  //- Destructor
  virtual ~cellSizeFunction();
  // Member Functions
    //- Const access to the details dictionary
    inline const dictionary& coeffsDict() const
    {
      return coeffsDict_;
    }
    virtual bool sizeLocations
    (
      const pointIndexHit& hitPt,
      const vector& n,
      pointField& shapePts,
      scalarField& shapeSizes
    ) const = 0;
    //- Modify scalar argument to the cell size specified by function.
    //  Return a boolean specifying if the function was used, i.e. false if
    //  the point was not in range of the surface for a spatially varying
    //  size.
    virtual bool cellSize
    (
      const point& pt,
      scalar& size
    ) const = 0;
    virtual bool setCellSize
    (
      const pointField& /*pts*/
    )
    {
      WARNING_IN("cellSizeFunction::setCellSize(const pointField&)")
        << "Not overloaded."
        << endl;
      return false;
    }
    label priority() const
    {
      return priority_;
    }
};
}  // namespace mousse
#endif
