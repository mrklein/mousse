#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_SKEW_CORRECTION_VECTORS_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_SKEW_CORRECTION_VECTORS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::skewCorrectionVectors
// Description
//   Skew-correction vectors for the skewness-corrected interpolation scheme
// SourceFiles
//   skew_correction_vectors.cpp
#include "_mesh_object.hpp"
#include "fv_mesh.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class fvMesh;
class skewCorrectionVectors
:
  public MeshObject<fvMesh, MoveableMeshObject, skewCorrectionVectors>
{
  // Private data
    //- Is mesh skew
    bool skew_;
    //- Skew correction vectors
    surfaceVectorField skewCorrectionVectors_;
    //- Calculate skewness correction vectors
    void calcSkewCorrectionVectors();
public:
  TYPE_NAME("skewCorrectionVectors");
  // Constructors
    explicit skewCorrectionVectors(const fvMesh& mesh);
  //- Destructor
  virtual ~skewCorrectionVectors();
  // Member functions
    //- Return whether mesh is skew or not
    bool skew() const
    {
      return skew_;
    }
    //- Return reference to skew vectors array
    const surfaceVectorField& operator()() const
    {
      return skewCorrectionVectors_;
    }
    //- Update the correction vectors when the mesh moves
    virtual bool movePoints();
};
}  // namespace mousse
#endif
