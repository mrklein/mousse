#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_INITIAL_POINTS_METHOD_POINT_FILE_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_INITIAL_POINTS_METHOD_POINT_FILE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointFile
// Description
//   Inserts points at locations specified in a pointFile into the surfaces to
//   be conformed to of the conformalVoronoiMesh
// SourceFiles
//   point_file.cpp
#include "file_name.hpp"
#include "point_io_field.hpp"
#include "switch.hpp"
#include "initial_points_method.hpp"
namespace mousse
{
class pointFile
:
  public initialPointsMethod
{
private:
  // Private data
    //- The initial cell spacing
    fileName pointFileName_;
    //- Check if inserted points are inside or outside
    Switch insideOutsideCheck_;
    //- Should the initial positions be randomised
    Switch randomiseInitialGrid_;
    //- Randomise the initial positions by fraction of the initialCellSize_
    scalar randomPerturbationCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("pointFile");
  // Constructors
    //- Construct from components
    pointFile
    (
      const dictionary& initialPointsDict,
      const Time& runTime,
      Random& rndGen,
      const conformationSurfaces& geometryToConformTo,
      const cellShapeControl& cellShapeControls,
      const autoPtr<backgroundMeshDecomposition>& decomposition
    );
  //- Destructor
  virtual ~pointFile()
  {}
  // Member Functions
    //- Return the initial points for the conformalVoronoiMesh
    virtual List<Vb::Point> initialPoints() const;
};
}  // namespace mousse
#endif
