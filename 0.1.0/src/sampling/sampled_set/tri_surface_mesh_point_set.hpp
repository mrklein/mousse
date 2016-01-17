// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::triSurfaceMeshPointSet
// Description
//   sampleSet from all points of a triSurfaceMesh.
// SourceFiles
//   tri_surface_mesh_point_set.cpp
#ifndef tri_surface_mesh_point_set_hpp_
#define tri_surface_mesh_point_set_hpp_
#include "sampled_set.hpp"
namespace mousse
{
// Forward declaration of classes
class triSurfaceMeshPointSet
:
  public sampledSet
{
  // Private data
    //- Name of triSurfaceMesh
    const word surface_;
    //- Sampling points
    List<point> sampleCoords_;
  // Private Member Functions
    //- Samples all points in sampleCoords.
    void calcSamples
    (
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<label>& samplingSegments,
      DynamicList<scalar>& samplingCurveDist
    ) const;
    //- Uses calcSamples to obtain samples. Copies them into *this.
    void genSamples();
public:
  //- Runtime type information
  TYPE_NAME("triSurfaceMeshPointSet");
  // Constructors
    //- Construct from dictionary
    triSurfaceMeshPointSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~triSurfaceMeshPointSet();
  // Member Functions
    //- Get reference point
    virtual point getRefPoint(const List<point>&) const;
};
}  // namespace mousse
#endif
