// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionSmootherData
// Description
// SourceFiles
//   motion_smoother_data.cpp
#ifndef motion_smoother_data_hpp_
#define motion_smoother_data_hpp_
#include "point_fields.hpp"
namespace mousse
{
class motionSmootherData
{
protected:
  // Private data
    //- Displacement field
    pointVectorField displacement_;
    //- Scale factor for displacement
    pointScalarField scale_;
    //- Starting mesh position
    pointField oldPoints_;
public:
  // Constructors
    //- Construct read
    motionSmootherData
    (
      const pointMesh&
    );
    //- Construct from pointDisplacement
    motionSmootherData
    (
      const pointVectorField&
    );
  // Member Functions
    //- Reference to displacement field
    pointVectorField& displacement();
    //- Reference to displacement field
    const pointVectorField& displacement() const;
    //- Reference to scale field
    const pointScalarField& scale() const;
    //- Starting mesh position
    const pointField& oldPoints() const;
};
}  // namespace mousse
#endif
