// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MRFZone
// Description
//   MRF zone definition based on cell zone and parameters
//   obtained from a control dictionary constructed from the given stream.
//   The rotation of the MRF region is defined by an origin and axis of
//   rotation and an angular speed.
// SourceFiles
//   mrf_zone.cpp

#ifndef mrf_zone_hpp_
#define mrf_zone_hpp_

#include "dictionary.hpp"
#include "word_list.hpp"
#include "label_list.hpp"
#include "dimensioned_scalar.hpp"
#include "dimensioned_vector.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields.hpp"
#include "fv_matrices_fwd.hpp"
#include "map_poly_mesh.hpp"
#include "data_entry.hpp"
#include "auto_ptr.hpp"

namespace mousse
{

// Forward declaration of classes
class fvMesh;

class MRFZone
{
  // Private data

    //- Reference to the mesh database
    const fvMesh& mesh_;

    //- Name of the MRF region
    const word name_;

    //- Coefficients dictionary
    dictionary coeffs_;

    //- MRF region active flag
    bool active_;

    //- Name of cell zone
    word cellZoneName_;

    //- Cell zone ID
    label cellZoneID_;

    const wordReList excludedPatchNames_;

    labelList excludedPatchLabels_;

    //- Internal faces that are part of MRF
    labelList internalFaces_;

    //- Outside faces (per patch) that move with the MRF
    labelListList includedFaces_;

    //- Excluded faces (per patch) that do not move with the MRF
    labelListList excludedFaces_;

    //- Origin of the axis
    const vector origin_;

    //- Axis vector
    vector axis_;

    //- Angular velocty (rad/sec)
    autoPtr<DataEntry<scalar>> omega_;

  // Private Member Functions

    //- Divide faces in frame according to patch
    void setMRFFaces();

    //- Make the given absolute mass/vol flux relative within the MRF region
    template<class RhoFieldType>
    void makeRelativeRhoFlux
    (
      const RhoFieldType& rho,
      surfaceScalarField& phi
    ) const;

    //- Make the given absolute mass/vol flux relative within the MRF region
    template<class RhoFieldType>
    void makeRelativeRhoFlux
    (
      const RhoFieldType& rho,
      FieldField<fvsPatchField, scalar>& phi
    ) const;

    //- Make the given relative mass/vol flux absolute within the MRF region
    template<class RhoFieldType>
    void makeAbsoluteRhoFlux
    (
      const RhoFieldType& rho,
      surfaceScalarField& phi
    ) const;

public:
  // Declare name of the class and its debug switch
  CLASS_NAME("MRFZone");

  // Constructors

    //- Construct from fvMesh
    MRFZone
    (
      const word& name,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& cellZoneName = word::null
    );

    //- Disallow default bitwise copy construct
    MRFZone(const MRFZone&) = delete;

    //- Disallow default bitwise assignment
    MRFZone& operator=(const MRFZone&) = delete;

    //- Return clone
    autoPtr<MRFZone> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<MRFZone> clone() const");
      return autoPtr<MRFZone>(NULL);
    }

  // Member Functions

    // Access

      //- Return const access to the MRF region name
      inline const word& name() const;

      //- Return const access to the MRF active flag
      inline bool active() const;

      //- Return the current Omega vector
      vector Omega() const;

    // Evaluation

      //- Update the mesh corresponding to given map
      void updateMesh(const mapPolyMesh&)
      {
        // Only updates face addressing
        setMRFFaces();
      }

      //- Add the Coriolis force contribution to the acceleration field
      void addCoriolis
      (
        const volVectorField& U,
        volVectorField& ddtU
      ) const;

      //- Add the Coriolis force contribution to the momentum equation
      //  Adds to the lhs of the equation; optionally add to rhs
      void addCoriolis
      (
        fvVectorMatrix& UEqn,
        const bool rhs = false
      ) const;

      //- Add the Coriolis force contribution to the momentum equation
      //  Adds to the lhs of the equation; optionally add to rhs
      void addCoriolis
      (
        const volScalarField& rho,
        fvVectorMatrix& UEqn,
        const bool rhs = false
      ) const;

      //- Make the given absolute velocity relative within the MRF region
      void makeRelative(volVectorField& U) const;

      //- Make the given absolute flux relative within the MRF region
      void makeRelative(surfaceScalarField& phi) const;

      //- Make the given absolute boundary flux relative
      //  within the MRF region
      void makeRelative(FieldField<fvsPatchField, scalar>& phi) const;

      //- Make the given absolute mass-flux relative within the MRF region
      void makeRelative
      (
        const surfaceScalarField& rho,
        surfaceScalarField& phi
      ) const;

      //- Make the given relative velocity absolute within the MRF region
      void makeAbsolute(volVectorField& U) const;

      //- Make the given relative flux absolute within the MRF region
      void makeAbsolute(surfaceScalarField& phi) const;

      //- Make the given relative mass-flux absolute within the MRF region
      void makeAbsolute
      (
        const surfaceScalarField& rho,
        surfaceScalarField& phi
      ) const;

      //- Correct the boundary velocity for the rotation of the MRF region
      void correctBoundaryVelocity(volVectorField& U) const;

  // I-O

    //- Write
    void writeData(Ostream& os) const;

    //- Read MRF dictionary
    bool read(const dictionary& dict);
};
}  // namespace mousse

inline const mousse::word& mousse::MRFZone::name() const
{
  return name_;
}
inline bool mousse::MRFZone::active() const
{
  return active_;
}

#ifdef NoRepository
#   include "mrf_zone_templates.cpp"
#endif

#endif
