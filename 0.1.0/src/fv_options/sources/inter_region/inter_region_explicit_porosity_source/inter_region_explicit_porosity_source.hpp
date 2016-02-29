#ifndef FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_EXPLICIT_POROSITY_SOURCE_INTER_REGION_EXPLICIT_POROSITY_SOURCE_HPP_
#define FV_OPTIONS_SOURCES_INTER_REGION_INTER_REGION_EXPLICIT_POROSITY_SOURCE_INTER_REGION_EXPLICIT_POROSITY_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::interRegionExplicitPorositySource
// Description
//   Inter-region explicit porosity source
//   Sources described by, for example using the DarcyForchheimer model:
//     interRegionExplicitPorositySourceCoeffs
//     {
//       type            DarcyForchheimer;
//       DarcyForchheimerCoeffs
//       {
//         d   d [0 -2 0 0 0 0 0] (5e7 -1000 -1000);
//         f   f [0 -1 0 0 0 0 0] (0 0 0);
//         coordinateSystem
//         {
//           e1  (0.70710678 0.70710678 0);
//           e2  (0 0 1);
//         }
//       }
//     }
// Note:
//   The porous region must be selected as a cellZone.
// SourceFiles
//   inter_region_explicit_porosity_source.cpp
#include "inter_region_option.hpp"
namespace mousse
{
class porosityModel;
namespace fv
{
class interRegionExplicitPorositySource
:
  public interRegionOption
{
protected:
  // Protected data
    //- Run-time selectable porosity model
    autoPtr<porosityModel> porosityPtr_;
    //- First iteration
    bool firstIter_;
    //- Velocity field name, default = U
    word UName_;
    //- Dynamic viscosity field name (compressible case only)
    //  default = thermo:mu
    word muName_;
  // Protected Member Functions
    //- Initialise
    void initialise();
public:
  //- Runtime type information
  TYPE_NAME("interRegionExplicitPorositySource");
  // Constructors
    //- Construct from components
    interRegionExplicitPorositySource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    interRegionExplicitPorositySource
    (
      const interRegionExplicitPorositySource&
    ) = delete;
    //- Disallow default bitwise assignment
    interRegionExplicitPorositySource& operator=
    (
      const interRegionExplicitPorositySource&
    ) = delete;
  //- Destructor
  virtual ~interRegionExplicitPorositySource()
  {}
  // Member Functions
    // Add explicit and implicit contributions
      //- Vector
      virtual void addSup
      (
        fvMatrix<vector>& eqn,
        const label fieldI
      );
    // Add explicit and implicit contributions to compressible equation
      //- Vector
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
#endif
