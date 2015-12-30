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
#ifndef inter_region_explicit_porosity_source_hpp_
#define inter_region_explicit_porosity_source_hpp_
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
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    interRegionExplicitPorositySource
    (
      const interRegionExplicitPorositySource&
    );
    //- Disallow default bitwise assignment
    void operator=(const interRegionExplicitPorositySource&);
public:
  //- Runtime type information
  TypeName("interRegionExplicitPorositySource");
  // Constructors
    //- Construct from components
    interRegionExplicitPorositySource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
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
