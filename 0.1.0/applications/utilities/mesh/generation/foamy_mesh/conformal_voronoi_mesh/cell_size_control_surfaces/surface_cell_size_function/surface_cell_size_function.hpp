#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_SURFACE_CELL_SIZE_FUNCTION_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_SURFACE_CELL_SIZE_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceCellSizeFunction
// Description
//   Abstract base class for specifying target cell sizes
// SourceFiles
//   surface_cell_size_function.cpp
#include "searchable_surface.hpp"
#include "dictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class surfaceCellSizeFunction
:
  public dictionary
{
protected:
  // Protected data
    //- Reference to the searchableSurface that surfaceCellSizeFunction
    //  relates to
    const searchableSurface& surface_;
    const dictionary coeffsDict_;
    const scalar& defaultCellSize_;
    //- If cell resizing is allowed, this is the factor of the old cell size
    //  to get the new cell size
    scalar refinementFactor_;
public:
  //- Runtime type information
  TYPE_NAME("surfaceCellSizeFunction");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      surfaceCellSizeFunction,
      dictionary,
      (
        const dictionary& surfaceCellSizeFunctionDict,
        const searchableSurface& surface,
        const scalar& defaultCellSize
      ),
      (surfaceCellSizeFunctionDict, surface, defaultCellSize)
    );
  // Constructors
    //- Construct from components
    surfaceCellSizeFunction
    (
      const word& type,
      const dictionary& surfaceCellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    surfaceCellSizeFunction(const surfaceCellSizeFunction&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const surfaceCellSizeFunction&) = delete;
  // Selectors
    //- Return a reference to the selected surfaceCellSizeFunction
    static autoPtr<surfaceCellSizeFunction> New
    (
      const dictionary& surfaceCellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~surfaceCellSizeFunction();
  // Member Functions
    //- Const access to the details dictionary
    inline const dictionary& coeffsDict() const
    {
      return coeffsDict_;
    }
    virtual scalar interpolate
    (
      const point& pt,
      const label index
    ) const = 0;
};
}  // namespace mousse
#endif
