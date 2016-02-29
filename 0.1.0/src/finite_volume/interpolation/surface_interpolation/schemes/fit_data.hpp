#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_FIT_DATA_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_FIT_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FitData
// Description
//   Data for the upwinded and centred polynomial fit interpolation schemes.
//   The linearCorrection_ determines whether the fit is for a corrected
//   linear scheme (first two coefficients are corrections for owner and
//   neighbour) or a pure upwind scheme (first coefficient is correction for
//   owner; weight on face taken as 1).
// SourceFiles
//   fit_data.cpp


#include "_mesh_object.hpp"
#include "fv_mesh.hpp"

namespace mousse
{

template<class FitDataType, class ExtendedStencil, class Polynomial>
class FitData
:
  public MeshObject<fvMesh, MoveableMeshObject, FitDataType>
{
  // Private data

    //- The stencil the fit is based on
    const ExtendedStencil& stencil_;

    //- Is scheme correction on linear (true) or on upwind (false)
    const bool linearCorrection_;

    //- Factor the fit is allowed to deviate from the base scheme
    //  (linear or pure upwind)
    //  This limits the amount of high-order correction and increases
    //  stability on bad meshes
    const scalar linearLimitFactor_;

    //- Weights for central stencil
    const scalar centralWeight_;

    //- Dimensionality of the geometry
    const label dim_;

    //- Minimum stencil size
    const label minSize_;

protected:
    //- Find the normal direction (i) and j and k directions for face faci
    void findFaceDirs
    (
      vector& idir,        // value changed in return
      vector& jdir,        // value changed in return
      vector& kdir,        // value changed in return
      const label faci
    );

public:
  // Constructors

    //- Construct from components
    FitData
    (
      const fvMesh& mesh,
      const ExtendedStencil& stencil,
      const bool linearCorrection,
      const scalar linearLimitFactor,
      const scalar centralWeight
    );

  //- Destructor
  virtual ~FitData()
  {}

  // Member functions

    //- Return reference to the stencil
    const ExtendedStencil& stencil() const
    {
      return stencil_;
    }

    //- Factor the fit is allowed to deviate from the base scheme
    scalar linearLimitFactor() const
    {
      return linearLimitFactor_;
    }

    //- Return weight for central stencil
    scalar centralWeight() const
    {
      return centralWeight_;
    }

    //- Dimensionality of the geometry
    label dim() const
    {
      return dim_;
    }

    //- Minimum stencil size
    label minSize() const
    {
      return minSize_;
    }

    bool linearCorrection() const
    {
      return linearCorrection_;
    }

    //- Calculate the fit for the specified face and set the coefficients
    void calcFit
    (
      scalarList& coeffsi, // coefficients to be set
      const List<point>&,  // Stencil points
      const scalar wLin,   // Weight for linear approximation (weights
                // nearest neighbours)
      const label faci     // Current face index
    );

    //- Calculate the fit for all the faces
    virtual void calcFit() = 0;

    //- Recalculate weights (but not stencil) when the mesh moves
    bool movePoints();
};
}  // namespace mousse

#ifdef NoRepository
#   include "fit_data.cpp"
#endif
#endif
