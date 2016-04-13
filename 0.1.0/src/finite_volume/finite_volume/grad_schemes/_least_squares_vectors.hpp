#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_TLEAST_SQUARES_VECTORS_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_TLEAST_SQUARES_VECTORS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::LeastSquaresVectors
// Description
//   Least-squares gradient scheme vectors
// See Also
//   mousse::fv::LeastSquaresGrad

#include "extended_centred_cell_to_cell_stencil.hpp"
#include "_mesh_object.hpp"


namespace mousse {
namespace fv {

template<class Stencil>
class LeastSquaresVectors
:
  public MeshObject<fvMesh, MoveableMeshObject, LeastSquaresVectors<Stencil>>
{
  // Private data
    //- Least-squares gradient vectors
    List<List<vector>> vectors_;

  // Private Member Functions
    //- Calculate Least-squares gradient vectors
    void calcLeastSquaresVectors();
public:
  // Declare name of the class and its debug switch
  TYPE_NAME("LeastSquaresVectors");

  // Constructors
    //- Construct given an fvMesh and the minimum determinant criterion
    LeastSquaresVectors
    (
      const fvMesh&
    );

  //- Destructor
  virtual ~LeastSquaresVectors();

  // Member functions
    //- Return reference to the stencil
    const extendedCentredCellToCellStencil& stencil() const
    {
      return Stencil::New(this->mesh_);
    }

    //- Return reference to the least square vectors
    const List<List<vector>>& vectors() const
    {
      return vectors_;
    }

    //- Update the least square vectors when the mesh moves
    virtual bool movePoints();
};

}  // namespace fv
}  // namespace mousse

#include "_least_squares_vectors.ipp"

#endif
