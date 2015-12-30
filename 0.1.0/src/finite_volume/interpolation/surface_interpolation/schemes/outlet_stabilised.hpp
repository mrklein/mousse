// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::outletStabilised
// Description
//   Outlet-stabilised interpolation scheme which applies upwind differencing
//   to the faces of the cells adjacent to outlets.
//   This is particularly useful to stabilise the velocity at entrainment
//   boundaries for LES cases using linear or other centred differencing
//   schemes.
// SourceFiles
//   outlet_stabilised.cpp
#ifndef outlet_stabilised_hpp_
#define outlet_stabilised_hpp_
#include "surface_interpolation_scheme.hpp"
#include "skew_correction_vectors.hpp"
#include "linear.hpp"
#include "gauss_grad.hpp"
#include "mixed_fv_patch_field.hpp"
#include "direction_mixed_fv_patch_field.hpp"
namespace mousse
{
template<class Type>
class outletStabilised
:
  public surfaceInterpolationScheme<Type>
{
  // Private member data
    const surfaceScalarField& faceFlux_;
    tmp<surfaceInterpolationScheme<Type> > tScheme_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    outletStabilised(const outletStabilised&);
    //- Disallow default bitwise assignment
    void operator=(const outletStabilised&);
public:
  //- Runtime type information
  TypeName("outletStabilised");
  // Constructors
    //- Construct from mesh and Istream
    outletStabilised
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      faceFlux_
      (
        mesh.lookupObject<surfaceScalarField>
        (
          word(is)
        )
      ),
      tScheme_
      (
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux_, is)
      )
    {}
    //- Construct from mesh, faceFlux and Istream
    outletStabilised
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      faceFlux_(faceFlux),
      tScheme_
      (
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      )
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      tmp<surfaceScalarField> tw = tScheme_().weights(vf);
      surfaceScalarField& w = tw();
      const fvMesh& mesh_ = this->mesh();
      const cellList& cells = mesh_.cells();
      forAll(vf.boundaryField(), patchi)
      {
        if
        (
          isA<zeroGradientFvPatchField<Type> >
            (vf.boundaryField()[patchi])
        || isA<mixedFvPatchField<Type> >(vf.boundaryField()[patchi])
        || isA<directionMixedFvPatchField<Type> >
          (vf.boundaryField()[patchi])
        )
        {
          const labelList& pFaceCells =
            mesh_.boundary()[patchi].faceCells();
          forAll(pFaceCells, pFacei)
          {
            const cell& pFaceCell = cells[pFaceCells[pFacei]];
            forAll(pFaceCell, fi)
            {
              label facei = pFaceCell[fi];
              if (mesh_.isInternalFace(facei))
              {
                // Apply upwind differencing
                w[facei] = pos(faceFlux_[facei]);
              }
            }
          }
        }
      }
      return tw;
    }
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return tScheme_().corrected();
    }
    //- Return the explicit correction to the face-interpolate
    //  set to zero on the near-boundary faces where upwinf is applied
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      if (tScheme_().corrected())
      {
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tcorr =
          tScheme_().correction(vf);
        GeometricField<Type, fvsPatchField, surfaceMesh>& corr =
          tcorr();
        const fvMesh& mesh_ = this->mesh();
        const cellList& cells = mesh_.cells();
        forAll(vf.boundaryField(), patchi)
        {
          if
          (
            isA<zeroGradientFvPatchField<Type> >
              (vf.boundaryField()[patchi])
          || isA<mixedFvPatchField<Type> >
              (vf.boundaryField()[patchi])
          )
          {
            const labelList& pFaceCells =
              mesh_.boundary()[patchi].faceCells();
            forAll(pFaceCells, pFacei)
            {
              const cell& pFaceCell = cells[pFaceCells[pFacei]];
              forAll(pFaceCell, fi)
              {
                label facei = pFaceCell[fi];
                if (mesh_.isInternalFace(facei))
                {
                  // Remove correction
                  corr[facei] = pTraits<Type>::zero;
                }
              }
            }
          }
        }
        return tcorr;
      }
      else
      {
        return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        (
          NULL
        );
      }
    }
};
}  // namespace mousse
#endif
