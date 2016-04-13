#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LOCAL_BLENDED_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LOCAL_BLENDED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::localBlended
// Description
//   Two-scheme localBlended differencing scheme.

#include "surface_interpolation_scheme.hpp"
#include "blended_scheme_base.hpp"
#include "surface_fields.hpp"


namespace mousse {

template<class Type>
class localBlended
:
  public surfaceInterpolationScheme<Type>,
  public blendedSchemeBase<Type>
{
  // Private Member Functions
    //- Scheme 1
    tmp<surfaceInterpolationScheme<Type>> tScheme1_;
    //- Scheme 2
    tmp<surfaceInterpolationScheme<Type>> tScheme2_;
public:
  //- Runtime type information
  TYPE_NAME("localBlended");
  // Constructors
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    localBlended
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      },
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      }
    {}
    //- Construct from mesh, faceFlux and Istream
    localBlended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      },
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      }
    {}
    //- Disallow default bitwise copy construct
    localBlended(const localBlended&) = delete;
    //- Disallow default bitwise assignment
    localBlended& operator=(const localBlended&) = delete;
  //- Destructor
  virtual ~localBlended()
  {}
  // Member Functions
    //- Return the face-based blending factor
    virtual tmp<surfaceScalarField> blendingFactor
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return
        this->mesh().objectRegistry::template
          lookupObject<const surfaceScalarField>
          (
            word{vf.name() + "BlendingFactor"}
          );
    }
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      const surfaceScalarField& blendingFactor =
        this->mesh().objectRegistry::template
          lookupObject<const surfaceScalarField>
          (
            word{vf.name() + "BlendingFactor"}
          );
      return
        blendingFactor*tScheme1_().weights(vf)
        + (scalar(1) - blendingFactor)*tScheme2_().weights(vf);
    }
    //- Return the face-interpolate of the given cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    interpolate(const GeometricField<Type, fvPatchField, volMesh>& vf) const
    {
      const surfaceScalarField& blendingFactor =
      (
        this->mesh().objectRegistry::template
        lookupObject<const surfaceScalarField>
        (
          word{vf.name() + "BlendingFactor"}
        )
      );
      return
        blendingFactor*tScheme1_().interpolate(vf)
        + (scalar(1) - blendingFactor)*tScheme2_().interpolate(vf);
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
      const surfaceScalarField& blendingFactor =
        this->mesh().objectRegistry::template
        lookupObject<const surfaceScalarField>
        (
          word(vf.name() + "BlendingFactor")
        );
      if (tScheme1_().corrected()) {
        if (tScheme2_().corrected()) {
          return
          (
            blendingFactor*tScheme1_().correction(vf)
            + (scalar(1.0) - blendingFactor)*tScheme2_().correction(vf)
          );
        } else {
          return blendingFactor*tScheme1_().correction(vf);
        }
      } else if (tScheme2_().corrected()) {
        return (scalar(1.0) - blendingFactor)*tScheme2_().correction(vf);
      } else {
        return tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
        {
          NULL
        };
      }
    }
};

}  // namespace mousse

#endif

