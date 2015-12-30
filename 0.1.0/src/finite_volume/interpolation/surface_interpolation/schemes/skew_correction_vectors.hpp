// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::skewCorrectionVectors
// Description
//   Skew-correction vectors for the skewness-corrected interpolation scheme
// SourceFiles
//   skew_correction_vectors.cpp
#ifndef skew_correction_vectors_hpp_
#define skew_correction_vectors_hpp_
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
  TypeName("skewCorrectionVectors");
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
