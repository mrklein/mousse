#ifndef FINITE_VOLUME_CFD_TOOLS_GENERAL_MRF_ZONE_LIST_HPP_
#define FINITE_VOLUME_CFD_TOOLS_GENERAL_MRF_ZONE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MRFZoneList
// Description
//   List container for MRF zomes

#include "fv_mesh.hpp"
#include "dictionary.hpp"
#include "fv_matrices_fwd.hpp"
#include "mrf_zone.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class MRFZoneList;
Ostream& operator<<(Ostream& os, const MRFZoneList& models);


class MRFZoneList
:
  PtrList<MRFZone>
{
protected:
  // Protected data
    //- Reference to the mesh database
    const fvMesh& mesh_;
public:
  //- Constructor
    MRFZoneList(const fvMesh& mesh, const dictionary& dict);
    //- Disallow default bitwise copy construct
    MRFZoneList(const MRFZoneList&) = delete;
    //- Disallow default bitwise assignment
    MRFZoneList& operator=(const MRFZoneList&) = delete;
  //- Destructor
  ~MRFZoneList();
  // Member Functions
    //- Return active status
    bool active(const bool warn = false) const;
    //- Reset the source list
    void reset(const dictionary& dict);
    //- Add the frame acceleration
    void addAcceleration
    (
      const volVectorField& U,
      volVectorField& ddtU
    ) const;
    //- Add the frame acceleration contribution to the momentum equation
    void addAcceleration(fvVectorMatrix& UEqn) const;
    //- Add the frame acceleration contribution to the momentum equation
    void addAcceleration
    (
      const volScalarField& rho,
      fvVectorMatrix& UEqn
    ) const;
    //- Return the frame acceleration
    tmp<volVectorField> DDt
    (
      const volVectorField& U
    ) const;
    //- Return the frame acceleration
    tmp<volVectorField> DDt
    (
      const volScalarField& rho,
      const volVectorField& U
    ) const;
    //- Make the given absolute velocity relative within the MRF region
    void makeRelative(volVectorField& U) const;
    //- Make the given absolute flux relative within the MRF region
    void makeRelative(surfaceScalarField& phi) const;
    //- Return the given absolute flux relative within the MRF region
    tmp<surfaceScalarField> relative
    (
      const tmp<surfaceScalarField>& phi
    ) const;
    //- Return the given absolute boundary flux relative within
    //  the MRF region
    tmp<FieldField<fvsPatchField, scalar> > relative
    (
      const tmp<FieldField<fvsPatchField, scalar> >& tphi
    ) const;
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
    //- Return the given relative flux absolute within the MRF region
    tmp<surfaceScalarField> absolute
    (
      const tmp<surfaceScalarField>& phi
    ) const;
    //- Make the given relative mass-flux absolute within the MRF region
    void makeAbsolute
    (
      const surfaceScalarField& rho,
      surfaceScalarField& phi
    ) const;
    //- Correct the boundary velocity for the rotation of the MRF region
    void correctBoundaryVelocity(volVectorField& U) const;
    //- Correct the boundary flux for the rotation of the MRF region
    void correctBoundaryFlux
    (
      const volVectorField& U,
      surfaceScalarField& phi
    ) const;
    // I-O
      //- Read dictionary
      bool read(const dictionary& dict);
      //- Write data to Ostream
      bool writeData(Ostream& os) const;
      //- Ostream operator
      friend Ostream& operator<<
      (
        Ostream& os,
        const MRFZoneList& models
      );
};
}  // namespace mousse
#endif
