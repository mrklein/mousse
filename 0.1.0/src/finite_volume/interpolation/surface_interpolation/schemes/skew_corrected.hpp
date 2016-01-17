// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::skewCorrected
// Description
//   Skewness-corrected interpolation scheme that applies an explicit
//   correction to given scheme.
// SourceFiles
//   skew_corrected.cpp

#ifndef skew_corrected_hpp_
#define skew_corrected_hpp_

#include "surface_interpolation_scheme.hpp"
#include "skew_correction_vectors.hpp"
#include "linear.hpp"
#include "gauss_grad.hpp"
#include "time.hpp"

namespace mousse
{
template<class Type>
class skewCorrected
:
  public surfaceInterpolationScheme<Type>
{
  // Private member data
    tmp<surfaceInterpolationScheme<Type> > tScheme_;
public:
  //- Runtime type information
  TYPE_NAME("skewCorrected");
  // Constructors
    //- Construct from mesh and Istream
    skewCorrected
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      tScheme_
      (
        surfaceInterpolationScheme<Type>::New(mesh, is)
      )
    {}
    //- Construct from mesh, faceFlux and Istream
    skewCorrected
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      tScheme_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      }
    {}
    //- Disallow default bitwise copy construct
    skewCorrected(const skewCorrected&) = delete;
    //- Disallow default bitwise assignment
    skewCorrected& operator=(const skewCorrected&) = delete;
  // Member Functions
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return tScheme_().weights(vf);
    }
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return
        tScheme_().corrected()
      || skewCorrectionVectors::New(this->mesh()).skew();
    }
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    skewCorrection
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      const fvMesh& mesh = this->mesh();
      const skewCorrectionVectors& scv = skewCorrectionVectors::New(mesh);
      tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
      (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
          IOobject
          (
            "skewCorrected::skewCorrection(" + vf.name() + ')',
            mesh.time().timeName(),
            mesh
          ),
          mesh,
          dimensioned<Type>
          (
            vf.name(),
            vf.dimensions(),
            pTraits<Type>::zero
          )
        )
      );
      for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
      {
        tsfCorr().replace
        (
          cmpt,
          scv() & linear
          <
            typename outerProduct
            <
              vector,
              typename pTraits<Type>::cmptType
            >::type
          > (mesh).interpolate
          (
            fv::gaussGrad<typename pTraits<Type>::cmptType>
            (mesh).grad(vf.component(cmpt))
          )
        );
      }
      return tsfCorr;
    }
    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      if
      (
        tScheme_().corrected()
      && skewCorrectionVectors::New(this->mesh()).skew()
      )
      {
        return tScheme_().correction(vf) + skewCorrection(vf);
      }
      else if (tScheme_().corrected())
      {
        return tScheme_().correction(vf);
      }
      else if (skewCorrectionVectors::New(this->mesh()).skew())
      {
        return skewCorrection(vf);
      }
      else
      {
        return
          tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
          (
            NULL
          );
      }
    }
};
}  // namespace mousse
#endif
