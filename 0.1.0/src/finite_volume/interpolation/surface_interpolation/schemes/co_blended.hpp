#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_CO_BLENDED_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_CO_BLENDED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Class
//   mousse::CoBlended
// Description
//   Two-scheme Courant number based blending differencing scheme.
//   Similar to localBlended but uses a blending factor computed from the
//   face-based Courant number and the lower and upper Courant number limits
//   supplied:
//   \f[
//     weight = 1 - max(min((Co - Co1)/(Co2 - Co1), 1), 0)
//   \f]
//   where
//   \vartable
//     Co1 | Courant number below which scheme1 is used
//     Co2 | Courant number above which scheme2 is used
//   \endvartable
//   The weight applies to the first scheme and 1-weight to the second scheme.
//   Example of the CoBlended scheme specification using LUST for Courant numbers
//   less than 1 and linearUpwind for Courant numbers greater than 10:
//   \verbatim
//   divSchemes
//   {
//     .
//     .
//     div(phi,U)      Gauss CoBlended 1 LUST grad(U) 10 linearUpwind grad(U);
//     .
//     .
//   }
//   \endverbatim

#include "surface_interpolation_scheme.hpp"
#include "blended_scheme_base.hpp"
#include "surface_interpolate.hpp"
#include "surface_fields.hpp"
#include "time.hpp"


namespace mousse {

template<class Type>
class CoBlended
:
  public surfaceInterpolationScheme<Type>,
  public blendedSchemeBase<Type>
{
  // Private data
    //- Courant number below which scheme1 is used
    const scalar Co1_;
    //- Scheme 1
    tmp<surfaceInterpolationScheme<Type>> tScheme1_;
    //- Courant number above which scheme2 is used
    const scalar Co2_;
    //- Scheme 2
    tmp<surfaceInterpolationScheme<Type>> tScheme2_;
    //- The face-flux used to compute the face Courant number
    const surfaceScalarField& faceFlux_;
public:
  //- Runtime type information
  TYPE_NAME("CoBlended");
  // Constructors
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    CoBlended
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      Co1_{readScalar(is)},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      },
      Co2_{readScalar(is)},
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      },
      faceFlux_
      {
        mesh.lookupObject<surfaceScalarField>(word(is))
      }
    {
      if (Co1_ < 0 || Co2_ < 0 || Co1_ >= Co2_) {
        FATAL_IO_ERROR_IN("CoBlended(const fvMesh&, Istream&)", is)
          << "coefficients = " << Co1_ << " and " << Co2_
          << " should be > 0 and Co2 > Co1"
          << exit(FatalIOError);
      }
    }
    //- Construct from mesh, faceFlux and Istream
    CoBlended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      Co1_{readScalar(is)},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      },
      Co2_{readScalar(is)},
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      },
      faceFlux_{faceFlux}
    {
      if (Co1_ < 0 || Co2_ < 0 || Co1_ >= Co2_) {
        FATAL_IO_ERROR_IN("CoBlended(const fvMesh&, Istream&)", is)
          << "coefficients = " << Co1_ << " and " << Co2_
          << " should be > 0 and Co2 > Co1"
          << exit(FatalIOError);
      }
    }
    //- Disallow default bitwise copy construct
    CoBlended(const CoBlended&) = delete;
    //- Disallow default bitwise assignment
    CoBlended& operator=(const CoBlended&) = delete;
  // Member Functions
    //- Return the face-based blending factor
    virtual tmp<surfaceScalarField> blendingFactor
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      const fvMesh& mesh = this->mesh();
      tmp<surfaceScalarField> tUflux = faceFlux_;
      if (faceFlux_.dimensions() == dimDensity*dimVelocity*dimArea) {
        // Currently assume that the density field
        // corresponding to the mass-flux is named "rho"
        const volScalarField& rho =
          mesh.objectRegistry::template lookupObject<volScalarField>("rho");
        tUflux = faceFlux_/fvc::interpolate(rho);
      } else if (faceFlux_.dimensions() != dimVelocity*dimArea) {
        FATAL_ERROR_IN
        (
          "CoBlended::blendingFactor()"
        )
        << "dimensions of faceFlux are not correct"
        << exit(FatalError);
      }
      return tmp<surfaceScalarField>
      {
        new surfaceScalarField
        {
          vf.name() + "BlendingFactor",
          scalar(1)
            - max
              (
                min
                (
                  (mesh.time().deltaT()*mesh.deltaCoeffs()*mag(tUflux)/mesh.magSf()
                   - Co1_)/(Co2_ - Co1_),
                  scalar(1)
                ),
                scalar(0)
              )
        }
      };
    }

    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      surfaceScalarField bf{blendingFactor(vf)};
      return bf*tScheme1_().weights(vf)
        + (scalar(1.0) - bf)*tScheme2_().weights(vf);
    }

    //- Return the face-interpolate of the given cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      surfaceScalarField bf{blendingFactor(vf)};
      return bf*tScheme1_().interpolate(vf)
        + (scalar(1.0) - bf)*tScheme2_().interpolate(vf);
    }

    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return tScheme1_().corrected() || tScheme2_().corrected();
    }

    //- Return the explicit correction to the face-interpolate
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      surfaceScalarField bf{blendingFactor(vf)};
      if (tScheme1_().corrected()) {
        if (tScheme2_().corrected()) {
          return
          (
            bf*tScheme1_().correction(vf)
            + (scalar(1.0) - bf)* tScheme2_().correction(vf)
          );
        } else {
          return
          (
            bf*tScheme1_().correction(vf)
          );
        }
      } else if (tScheme2_().corrected()) {
        return
        (
          (scalar(1.0) - bf)*tScheme2_().correction(vf)
        );
      } else {
        return tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>{NULL};
      }
    }
};

}  // namespace mousse

#endif

