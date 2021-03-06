#ifndef DYNAMIC_MESH_MOTION_SMOOTHER_MOTION_SMOOTHER_DATA_HPP_
#define DYNAMIC_MESH_MOTION_SMOOTHER_MOTION_SMOOTHER_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionSmootherData
// Description

#include "point_fields.hpp"


namespace mousse {

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
    motionSmootherData(const pointMesh&);
    //- Construct from pointDisplacement
    motionSmootherData(const pointVectorField&);
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

