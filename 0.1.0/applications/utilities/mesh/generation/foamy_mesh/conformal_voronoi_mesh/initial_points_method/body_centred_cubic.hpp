// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::bodyCentredCubic
// Description
//   Generate a BCC lattice of points inside the surfaces to be
//   conformed to of the conformalVoronoiMesh
// SourceFiles
//   body_centred_cubic.cpp
#ifndef body_centred_cubic_hpp_
#define body_centred_cubic_hpp_
#include "initial_points_method.hpp"
namespace mousse
{
class bodyCentredCubic
:
  public initialPointsMethod
{
private:
  // Private data
  //- The initial cell spacing
  scalar initialCellSize_;
  //- Should the initial positions be randomised
  Switch randomiseInitialGrid_;
  //- Randomise the initial positions by fraction of the initialCellSize_
  scalar randomPerturbationCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("bodyCentredCubic");
  // Constructors
    //- Construct from components
    bodyCentredCubic
    (
      const dictionary& initialPointsDict,
      const Time& runTime,
      Random& rndGen,
      const conformationSurfaces& geometryToConformTo,
      const cellShapeControl& cellShapeControls,
      const autoPtr<backgroundMeshDecomposition>& decomposition
    );
  //- Destructor
  virtual ~bodyCentredCubic()
  {}
  // Member Functions
    //- Return the initial points for the conformalVoronoiMesh
    virtual List<Vb::Point> initialPoints() const;
};
}  // namespace mousse
#endif
