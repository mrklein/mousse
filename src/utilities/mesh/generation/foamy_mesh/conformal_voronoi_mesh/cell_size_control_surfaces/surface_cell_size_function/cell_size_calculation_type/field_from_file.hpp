#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_CELL_SIZE_CALCULATION_TYPE_FIELD_FROM_FILE_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SIZE_CONTROL_SURFACES_SURFACE_CELL_SIZE_FUNCTION_CELL_SIZE_CALCULATION_TYPE_FIELD_FROM_FILE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fieldFromFile

#include "cell_size_calculation_type.hpp"
#include "tri_surface_fields.hpp"
#include "primitive_patch_interpolation.hpp"


namespace mousse {

class triSurfaceMesh;


class fieldFromFile
:
  public cellSizeCalculationType
{
private:
  // Private data
    //- Dictionary of coefficients for automatic cell sizing
    const dictionary& coeffsDict_;
    //- Name of the triSurfaceScalarField file to load in. Must be in
    //  constant/triSurface
    const word fileName_;
    //- Multiply all the point sizes by this value
    const scalar cellSizeMultipleCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("fieldFromFile");
  // Constructors
    //- Construct from components
    fieldFromFile
    (
      const dictionary& cellSizeCalcTypeDict,
      const triSurfaceMesh& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~fieldFromFile()
  {}
  // Member Functions
    //- Load the cell size field
    virtual tmp<triSurfacePointScalarField> load();
};

}  // namespace mousse

#endif

