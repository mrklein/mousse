#ifndef FV_OPTIONS_SOURCES_DERIVED_RADIAL_ACTUATION_DISK_SOURCE_RADIAL_ACTUATION_DISK_SOURCE_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_RADIAL_ACTUATION_DISK_SOURCE_RADIAL_ACTUATION_DISK_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::radialActuationDiskSource
// Description
//   Actuation disk source including radial thrust
//   Constant values for momentum source for actuation disk
//   \f[
//     T = 2 \rho A U_{o}^2 a (1-a)
//   \f]
//   and
//   \f[
//     U_1 = (1 - a)U_{o}
//   \f]
//   where:
//   \vartable
//     A   | disk area
//     U_o | upstream velocity
//     a   | 1 - Cp/Ct
//     U_1 | velocity at the disk
//   \endvartable
//   The thrust is distributed by a radial function:
//   \f[
//     thrust(r) = T (C_0 + C_1 r^2 + C_2 r^4)
//   \f]
//   \heading Source usage
//   Example usage:
//   \verbatim
//   actuationDiskSourceCoeffs
//   {
//     fieldName       U;          // name of field to apply source
//     diskDir         (-1 0 0);   // disk direction
//     Cp              0.1;        // power coefficient
//     Ct              0.5;        // thrust coefficient
//     diskArea        5.0;        // disk area
//     coeffs          (0.1 0.5 0.01); // radial distribution coefficients
//     upstreamPoint   (0 0 0);    // upstream point
//   }
//   \endverbatim

#include "actuation_disk_source.hpp"
#include "fixed_list.hpp"


namespace mousse {
namespace fv {

class radialActuationDiskSource
:
  public actuationDiskSource
{
  // Private data
    //- Coeffcients for the radial distribution
    FixedList<scalar, 3> radialCoeffs_;
  // Private Member Functions
    //- Add resistance to the UEqn
    template<class RhoFieldType>
    void addRadialActuationDiskAxialInertialResistance
    (
      vectorField& Usource,
      const labelList& cells,
      const scalarField& V,
      const RhoFieldType& rho,
      const vectorField& U
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("radialActuationDiskSource");
  // Constructors
    //- Construct from components
    radialActuationDiskSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    radialActuationDiskSource(const radialActuationDiskSource&) = delete;
    //- Disallow default bitwise assignment
    radialActuationDiskSource& operator=
    (
      const radialActuationDiskSource&
    ) = delete;
  //- Destructor
  virtual ~radialActuationDiskSource()
  {}
  // Member Functions
    //- Source term to momentum equation
    virtual void addSup
    (
      fvMatrix<vector>& eqn,
      const label fieldI
    );
    //- Source term to compressible momentum equation
    virtual void addSup
    (
      const volScalarField& rho,
      fvMatrix<vector>& eqn,
      const label fieldI
    );
  // IO
    //- Read dictionary
    virtual bool read(const dictionary& dict);
};

}  // namespace fv
}  // namespace mousse

#include "radial_actuation_disk_source.ipp"

#endif
