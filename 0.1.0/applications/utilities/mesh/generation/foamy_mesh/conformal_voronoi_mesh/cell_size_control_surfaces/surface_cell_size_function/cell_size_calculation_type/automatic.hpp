// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::automatic
// Description
// SourceFiles
//   automatic.cpp
#ifndef automatic_hpp_
#define automatic_hpp_
#include "cell_size_calculation_type.hpp"
#include "tri_surface_fields.hpp"
#include "primitive_patch_interpolation.hpp"
#include "switch.hpp"
namespace mousse
{
class triSurfaceMesh;
class automatic
:
  public cellSizeCalculationType
{
private:
  // Private data
    //- Dictionary of coefficients for automatic cell sizing
    const dictionary& coeffsDict_;
    //- Name of the surface. Used to write the cell size field
    const fileName surfaceName_;
    const Switch readCurvature_;
    const word curvatureFile_;
    const Switch readFeatureProximity_;
    const word featureProximityFile_;
    const Switch readInternalCloseness_;
    const word internalClosenessFile_;
    //- The curvature values are multiplied by the inverse of this value to
    //  get the cell size
    const scalar curvatureCellSizeCoeff_;
    //- The maximum allowable sell size
    const scalar maximumCellSize_;
  // Private Member Functions
    void smoothField(triSurfaceScalarField& surf);
public:
  //- Runtime type information
  TYPE_NAME("automatic");
  // Constructors
    //- Construct from components
    automatic
    (
      const dictionary& cellSizeCalcTypeDict,
      const triSurfaceMesh& surface,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~automatic()
  {}
  // Member Functions
    //- Load the cell size field
    virtual tmp<triSurfacePointScalarField> load();
};
}  // namespace mousse
#endif
