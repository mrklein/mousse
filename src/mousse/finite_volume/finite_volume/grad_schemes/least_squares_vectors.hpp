#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LEAST_SQUARES_VECTORS_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LEAST_SQUARES_VECTORS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::leastSquaresVectors
// Description
//   Least-squares gradient scheme vectors

#include "_mesh_object.hpp"
#include "fv_mesh.hpp"
#include "surface_fields.hpp"


namespace mousse {

class leastSquaresVectors
:
  public MeshObject<fvMesh, MoveableMeshObject, leastSquaresVectors>
{
  // Private data
    //- Least-squares gradient vectors
    surfaceVectorField pVectors_;
    surfaceVectorField nVectors_;
  // Private Member Functions
    //- Construct Least-squares gradient vectors
    void calcLeastSquaresVectors();
public:
  // Declare name of the class and its debug switch
  TYPE_NAME("leastSquaresVectors");
  // Constructors
    //- Construct given an fvMesh
    explicit leastSquaresVectors(const fvMesh&);
  //- Destructor
  virtual ~leastSquaresVectors();
  // Member functions
    //- Return reference to owner least square vectors
    const surfaceVectorField& pVectors() const
    {
      return pVectors_;
    }
    //- Return reference to neighbour least square vectors
    const surfaceVectorField& nVectors() const
    {
      return nVectors_;
    }
    //- Delete the least square vectors when the mesh moves
    virtual bool movePoints();
};
}  // namespace mousse
#endif
