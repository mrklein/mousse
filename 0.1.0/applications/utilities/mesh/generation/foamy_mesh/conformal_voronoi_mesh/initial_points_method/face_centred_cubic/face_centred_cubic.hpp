// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceCentredCubic
// Description
//   Generate an FCC lattice of points inside the surfaces to be
//   conformed to of the conformalVoronoiMesh
// SourceFiles
//   face_centred_cubic.cpp
#ifndef face_centred_cubic_hpp_
#define face_centred_cubic_hpp_
#include "initial_points_method.hpp"
namespace mousse
{
class faceCentredCubic
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
  TypeName("faceCentredCubic");
  // Constructors
    //- Construct from components
    faceCentredCubic
    (
      const dictionary& initialPointsDict,
      const Time& runTime,
      Random& rndGen,
      const conformationSurfaces& geometryToConformTo,
      const cellShapeControl& cellShapeControls,
      const autoPtr<backgroundMeshDecomposition>& decomposition
    );
  //- Destructor
  virtual ~faceCentredCubic()
  {}
  // Member Functions
    //- Return the initial points for the conformalVoronoiMesh
    virtual List<Vb::Point> initialPoints() const;
};
}  // namespace mousse
#endif