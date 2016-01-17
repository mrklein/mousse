// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nonUniformField
// Description
// SourceFiles
//   non_uniform_field.cpp
#ifndef non_uniform_field_hpp_
#define non_uniform_field_hpp_
#include "tri_surface_fields.hpp"
#include "primitive_patch_interpolation.hpp"
#include "surface_cell_size_function.hpp"
#include "cell_size_calculation_type.hpp"
namespace mousse
{
class triSurfaceMesh;
class searchableSurface;
class nonUniformField
:
  public surfaceCellSizeFunction
{
protected:
  // Private typedef
    typedef PrimitivePatchInterpolation
    <
      PrimitivePatch<labelledTri, List, pointField, point>
    >   primitivePatchInterpolation;
  // Private data
    const triSurfaceMesh& surfaceTriMesh_;
    autoPtr<cellSizeCalculationType> cellSizeCalculationType_;
    triSurfacePointScalarField pointCellSize_;
public:
  //- Runtime type information
  TypeName("nonUniformField");
  // Constructors
    //- Construct from components
    nonUniformField
    (
      const dictionary& cellSizeFunctionDict,
      const searchableSurface& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~nonUniformField()
  {}
  // Member Functions
    // Query
      //- Return the interpolated cell size for a point in the given
      //  surface triangle
      virtual scalar interpolate
      (
        const point& pt,
        const label index
      ) const;
};
}  // namespace mousse
#endif
