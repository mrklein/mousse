#ifndef FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_ROTOR_DISK_SOURCE_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_ROTOR_DISK_SOURCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::rotorDiskSource
// Description
//   Rotor disk source
//   Cell based momemtum source which approximates the mean effects of
//   rotor forces on a cylindrical region within the domain.
//   \heading Source usage
//   Example usage:
//   \verbatim
//   rotorDiskSourceCoeffs
//   {
//     fieldNames      (U);    // names of fields on which to apply source
//     nBlades         3;      // number of blades
//     tipEffect       0.96;   // normalised radius above which lift = 0
//     inletFlowType   local;  // inlet flow type specification
//     geometryMode    auto;   // geometry specification
//     refDirection    (-1 0 0); // reference direction
//                  // - used as reference for psi angle
//     trimModel       fixed;  // fixed || targetForce
//     flapCoeffs
//     {
//       beta0           0;  // coning angle [deg]
//       beta1c          0;  // lateral flapping coeff (cos coeff)
//       beta2s          0;  // longitudinal flapping coeff (sin coeff)
//     }
//     blade
//     {
//       // see bladeModel.H for documentation
//     }
//     profiles
//     {
//       profile1
//       {
//         type    lookup; // lookup || series
//         ...
//         // see lookupProfile.H or seriesProfile.H for documentation
//       }
//       profile2
//       {
//         ...
//       }
//     }
//   }
//   \endverbatim
//   Where:
//   Valid options for the \c geometryMode entry include:
//   - auto          : determine rototor co-ord system from cells
//   - specified     : specified co-ord system
//   Valid options for the \c inletFlowType entry include:
//   - fixed         : specified velocity
//   - local         : use local flow conditions
//   - surfaceNormal : specified normal velocity (positive towards rotor)
// See Also
//   mousse::bladeModel
//   mousse::lookupProfile
//   mousse::seriesProfile
// SourceFiles
//   rotor_disk_source.cpp
//   rotor_disk_source_templates.cpp
#include "cell_set_option.hpp"
#include "cylindrical_cs.hpp"
#include "cylindrical.hpp"
#include "named_enum.hpp"
#include "blade_model.hpp"
#include "profile_model_list.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
// Forward declaration of classes
class trimModel;
namespace fv
{
class rotorDiskSource
:
  public cellSetOption
{
public:
  enum geometryModeType
  {
    gmAuto,
    gmSpecified
  };
  static const NamedEnum<geometryModeType, 2> geometryModeTypeNames_;
  enum inletFlowType
  {
    ifFixed,
    ifSurfaceNormal,
    ifLocal
  };
  static const NamedEnum<inletFlowType, 3> inletFlowTypeNames_;
protected:
  // Helper structures to encapsulate flap and trim data
  // Note: all input in degrees (converted to radians internally)
    struct flapData
    {
      scalar beta0;   // coning angle
      scalar beta1c;  // lateral flapping coeff (cos coeff)
      scalar beta2s;  // longitudinal flapping coeff (sin coeff)
    };
  // Protected data
    //- Reference density for incompressible case
    scalar rhoRef_;
    //- Rotational speed [rad/s]
    //  Positive anti-clockwise when looking along -ve lift direction
    scalar omega_;
    //- Number of blades
    label nBlades_;
    //- Inlet flow type
    inletFlowType inletFlow_;
    //- Inlet velocity for specified iinflow
    vector inletVelocity_;
    //- Tip effect [0-1]
    //  Ratio of blade radius beyond which lift=0
    scalar tipEffect_;
    //- Blade flap coefficients [rad/s]
    flapData flap_;
    //- Cell centre positions in local rotor frame
    //  (Cylindrical r, theta, z)
    List<point> x_;
    //- Rotation tensor for flap angle
    List<tensor> R_;
    //- Inverse rotation tensor for flap angle
    List<tensor> invR_;
    //- Area [m2]
    List<scalar> area_;
    //- Rotor local cylindrical co-ordinate system (r, theta, z)
    cylindricalCS coordSys_;
    //- Rotor transformation co-ordinate system
    autoPtr<cylindrical> cylindrical_;
    //- Maximum radius
    scalar rMax_;
    //- Trim model
    autoPtr<trimModel> trim_;
    //- Blade data
    bladeModel blade_;
    //- Profile data
    profileModelList profiles_;
  // Protected Member Functions
    //- Check data
    void checkData();
    //- Set the face areas per cell, and optionally correct the rotor axis
    void setFaceArea(vector& axis, const bool correct);
    //- Create the co-ordinate system
    void createCoordinateSystem();
    //- Construct geometry
    void constructGeometry();
    //- Return the inlet flow field
    tmp<vectorField> inflowVelocity(const volVectorField& U) const;
    //- Helper function to write rotor values
    template<class Type>
    void writeField
    (
      const word& name,
      const List<Type>& values,
      const bool writeNow = false
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("rotorDisk");
  // Constructors
    //- Construct from components
    rotorDiskSource
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~rotorDiskSource();
  // Member Functions
    // Access
      //- Return the reference density for incompressible case
      inline scalar rhoRef() const;
      //- Return the rotational speed [rad/s]
      //  Positive anti-clockwise when looking along -ve lift direction
      inline scalar omega() const;
      //- Return the cell centre positions in local rotor frame
      // (Cylindrical r, theta, z)
      inline const List<point>& x() const;
      //- Return the rotor co-ordinate system (r, theta, z)
      inline const cylindricalCS& coordSys() const;
    // Evaluation
      //- Calculate forces
      template<class RhoFieldType>
      void calculate
      (
        const RhoFieldType& rho,
        const vectorField& U,
        const scalarField& thetag,
        vectorField& force,
        const bool divideVolume = true,
        const bool output = true
      ) const;
    // Source term addition
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
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse

mousse::scalar mousse::fv::rotorDiskSource::rhoRef() const
{
  return rhoRef_;
}
mousse::scalar mousse::fv::rotorDiskSource::omega() const
{
  return omega_;
}
const mousse::List<mousse::point>& mousse::fv::rotorDiskSource::x() const
{
  return x_;
}
const mousse::cylindricalCS& mousse::fv::rotorDiskSource::coordSys() const
{
  return coordSys_;
}

#ifdef NoRepository
  #include "rotor_disk_source_templates.cpp"
#endif
#endif
