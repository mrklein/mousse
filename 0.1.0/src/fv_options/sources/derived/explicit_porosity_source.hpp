#ifndef FV_OPTIONS_SOURCES_DERIVED_EXPLICIT_POROSITY_SOURCE_EXPLICIT_POROSITY_SOURCE_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_EXPLICIT_POROSITY_SOURCE_EXPLICIT_POROSITY_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::explicitPorositySource
// Description
//   Explicit porosity source
//   \heading Source usage
//   Example usage, here employing the Darcy-Forchheimer model:
//   \verbatim
//   explicitPorositySourceCoeffs
//   {
//     type            DarcyForchheimer;
//     DarcyForchheimerCoeffs
//     {
//       d   d [0 -2 0 0 0 0 0] (5e7 -1000 -1000);
//       f   f [0 -1 0 0 0 0 0] (0 0 0);
//       coordinateSystem
//       {
//         type    cartesian;
//         origin  (0 0 0);
//         coordinateRotation
//         {
//           type    axesRotation;
//           e1  (0.70710678 0.70710678 0);
//           e2  (0 0 1);
//         }
//       }
//     }
//   }
//   \endverbatim
// Note:
//   The porous region must be selected as a cellZone.

#include "cell_set_option.hpp"


namespace mousse {
class porosityModel;

namespace fv {

class explicitPorositySource
:
  public cellSetOption
{
protected:
  // Protected data
    //- Run-time selectable porosity model
    autoPtr<porosityModel> porosityPtr_;
public:
  //- Runtime type information
  TYPE_NAME("explicitPorositySource");
  // Constructors
    //- Construct from components
    explicitPorositySource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    explicitPorositySource(const explicitPorositySource&) = delete;
    //- Disallow default bitwise assignment
    explicitPorositySource& operator=(const explicitPorositySource&) = delete;
  //- Destructor
  virtual ~explicitPorositySource()
  {}
  // Member Functions
    // Add explicit and implicit contributions
      //- Add implicit contribution to momentum equation
      virtual void addSup
      (
        fvMatrix<vector>& eqn,
        const label fieldI
      );
      //- Add implicit contribution to compressible momentum equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<vector>& eqn,
        const label fieldI
      );
      //- Add implicit contribution to phase momentum equation
      virtual void addSup
      (
        const volScalarField& alpha,
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

#endif

