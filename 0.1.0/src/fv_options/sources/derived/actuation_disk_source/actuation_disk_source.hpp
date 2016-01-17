// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::actuationDiskSource
// Description
//   Actuation disk source
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
//   \heading Source usage
//   Example usage:
//   \verbatim
//   actuationDiskSourceCoeffs
//   {
//     fieldNames      (U);        // names of fields to apply source
//     diskDir         (-1 0 0);   // disk direction
//     Cp              0.1;        // power coefficient
//     Ct              0.5;        // thrust coefficient
//     diskArea        5.0;        // disk area
//     upstreamPoint   (0 0 0);    // upstream point
//   }
//   \endverbatim
// SourceFiles
//   actuation_disk_source.cpp
//   actuation_disk_source_templates.cpp
#ifndef actuation_disk_source_hpp_
#define actuation_disk_source_hpp_
#include "cell_set_option.hpp"
namespace mousse
{
namespace fv
{
class actuationDiskSource
:
  public cellSetOption
{
protected:
  // Protected data
    //- Disk area normal
    vector diskDir_;
    //- Power coefficient
    scalar Cp_;
    //- Thrust coefficient
    scalar Ct_;
    //- Disk area
    scalar diskArea_;
    //- Upstream point sample
    point upstreamPoint_;
    //- Upstream cell ID
    label upstreamCellId_;
private:
  // Private Member Functions
    //- Check data
    void checkData() const;
    //- Add resistance to the UEqn
    template<class RhoFieldType>
    void addActuationDiskAxialInertialResistance
    (
      vectorField& Usource,
      const labelList& cells,
      const scalarField& V,
      const RhoFieldType& rho,
      const vectorField& U
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("actuationDiskSource");
  // Constructors
    //- Construct from components
    actuationDiskSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    actuationDiskSource(const actuationDiskSource&) = delete;
    //- Disallow default bitwise assignment
    actuationDiskSource& operator=(const actuationDiskSource&) = delete;
  //- Destructor
  virtual ~actuationDiskSource()
  {}
  // Member Functions
    // Access
      //- Return Cp
      scalar Cp() const
      {
        return Cp_;
      }
      //- Return Ct
      scalar Ct() const
      {
        return Ct_;
      }
      //- Normal disk direction
      const vector& diskDir() const
      {
        return diskDir_;
      }
      //- Disk area
      scalar diskArea() const
      {
        return diskArea_;
      }
    // Add explicit and implicit contributions
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
#ifdef NoRepository
#   include "actuation_disk_source_templates.cpp"
#endif
#endif
