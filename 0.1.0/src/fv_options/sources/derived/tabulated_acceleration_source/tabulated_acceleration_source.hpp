// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::tabulatedAccelerationSource
// Description
//   Solid-body 6-DoF acceleration source
//   \heading Source usage
//   Example usage:
//   \verbatim
//   SBM
//   {
//     type            tabulatedAccelerationSource;
//     active          yes;
//     tabulatedAccelerationSourceCoeffs
//     {
//       timeDataFileName "constant/acceleration-terms.dat";
//     }
//   }
//   \endverbatim
// SourceFiles
//   tabulated_acceleration_source.cpp
#ifndef tabulated_acceleration_source_hpp_
#define tabulated_acceleration_source_hpp_
#include "fv_option.hpp"
#include "tabulated_6dof_acceleration.hpp"
#include "dimensioned_types.hpp"
namespace mousse
{
namespace fv
{
class tabulatedAccelerationSource
:
  public option
{
protected:
  // Protected data
    //- Run-time selectable acceleration model
    tabulated6DoFAcceleration motion_;
    //- Velocity field name, default = U
    word UName_;
    dimensionedVector g0_;
private:
  // Private Member Functions
    //- Source term to momentum equation
    template<class RhoFieldType>
    void addSup
    (
      const RhoFieldType& rho,
      fvMatrix<vector>& eqn,
      const label fieldi
    );
public:
  //- Runtime type information
  TYPE_NAME("tabulatedAccelerationSource");
  // Constructors
    //- Construct from components
    tabulatedAccelerationSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    tabulatedAccelerationSource(const tabulatedAccelerationSource&) = delete;
    //- Disallow default bitwise assignment
    tabulatedAccelerationSource& operator=
    (
      const tabulatedAccelerationSource&
    ) = delete;
  //- Destructor
  virtual ~tabulatedAccelerationSource()
  {}
  // Member Functions
    //- Source term to momentum equation
    virtual void addSup
    (
      fvMatrix<vector>& eqn,
      const label fieldi
    );
    //- Source term to compressible momentum equation
    virtual void addSup
    (
      const volScalarField& rho,
      fvMatrix<vector>& eqn,
      const label fieldi
    );
    //- Read dictionary
    virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
  #include "tabulated_acceleration_source_templates.cpp"
#endif
#endif
