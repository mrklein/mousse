#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_CELL_SIZE_CALCULATION_TYPE_CELL_SIZE_CALCULATION_TYPE_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_CELL_SIZE_CALCULATION_TYPE_CELL_SIZE_CALCULATION_TYPE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSizeCalculationType
// Description
//   Abstract base class for specifying target cell sizes
// SourceFiles
//   cell_size_calculation_type.cpp
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "tri_surface_fields.hpp"
namespace mousse
{
class triSurfaceMesh;
class cellSizeCalculationType
{
protected:
    const dictionary& cellSizeCalculationTypeDict_;
    //- Reference to the triSurfaceMesh
    const triSurfaceMesh& surface_;
    const scalar& defaultCellSize_;
public:
  //- Runtime type information
  TYPE_NAME("cellSizeCalculationType");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      cellSizeCalculationType,
      dictionary,
      (
        const dictionary& cellSizeCalculationTypeDict,
        const triSurfaceMesh& surface,
        const scalar& defaultCellSize
      ),
      (cellSizeCalculationTypeDict, surface, defaultCellSize)
    );
  // Constructors
    //- Construct from components
    cellSizeCalculationType
    (
      const word& type,
      const dictionary& cellSizeCalculationTypeDict,
      const triSurfaceMesh& surface,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    cellSizeCalculationType(const cellSizeCalculationType&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellSizeCalculationType&) = delete;
  // Selectors
    //- Return a reference to the selected cellSizeCalculationType
    static autoPtr<cellSizeCalculationType> New
    (
      const dictionary& cellSizeCalculationTypeDict,
      const triSurfaceMesh& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~cellSizeCalculationType();
  // Member Functions
    //- Load the cell size
    virtual tmp<triSurfacePointScalarField> load() = 0;
};
}  // namespace mousse
#endif
