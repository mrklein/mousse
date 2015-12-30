// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::temperatureCoupledBase
// Description
//   Common functions for use in temperature coupled boundaries.
//   For now only provides the following methods:
//   - kappa() : heat conduction at patch. Gets supplied how to lookup/calculate
//    'kappa':
//     - 'lookup' : lookup volScalarField (or volSymmTensorField) with name
//      defined in 'kappaName'
//     - 'fluidThermo' : use fluidThermo and default
//      compressible::turbulenceModel to calculate kappa
//     - 'solidThermo' : use solidThermo kappa()
//     - 'directionalSolidThermo': uses look up for volSymmTensorField for
//      transformed kappa vector. Field name definable in 'alphaAniName',
//      named 'Anialpha' in solid solver by default
//   \heading Keywords provided by this class
//   \table
//     Property     | Description             | Required    | Default value
//     kappa        | heat conduction type at patch, as listed above | yes |
//     kappaName    | Name of thermal conductivity field | yes |
//     alphaAniName | name of the non-isotropic alpha | no | 'Anialpha'
//   \endtable
//   Usage examples:
//   \verbatim
//   nonIsotropicWall
//   {
//     ...
//     kappa           directionalSolidThermo;
//     kappaName       none;
//     alphaAniName    Anialpha;
//     ...
//   }
//   \endverbatim
// SourceFiles
//   temperature_coupled_base.cpp
#ifndef temperature_coupled_base_hpp_
#define temperature_coupled_base_hpp_
#include "scalar_field.hpp"
#include "named_enum.hpp"
#include "fv_patch.hpp"
namespace mousse
{
class temperatureCoupledBase
{
public:
  // Public enumerations
    //- Type of supplied Kappa
    enum KMethodType
    {
      mtFluidThermo,
      mtSolidThermo,
      mtDirectionalSolidThermo,
      mtLookup
    };
protected:
  // Protected data
    static const NamedEnum<KMethodType, 4> KMethodTypeNames_;
    //- Underlying patch
    const fvPatch& patch_;
    //- How to get K
    const KMethodType method_;
    //- Name of thermal conductivity field (if looked up from database)
    const word kappaName_;
    //- Name of the non-Isotropic alpha (default: Anialpha)
    const word alphaAniName_;
public:
  // Constructors
    //- Construct from patch and K name
    temperatureCoupledBase
    (
      const fvPatch& patch,
      const word& calculationMethod,
      const word& kappaName,
      const word& alphaAniName
    );
    //- Construct from patch and dictionary
    temperatureCoupledBase
    (
      const fvPatch& patch,
      const dictionary& dict
    );
    //- Construct from patch and temperatureCoupledBase
    temperatureCoupledBase
    (
      const fvPatch& patch,
      const temperatureCoupledBase& base
    );
  // Member functions
    //- Method to obtain K
    word KMethod() const
    {
      return KMethodTypeNames_[method_];
    }
    //- Name of thermal conductivity field
    const word& kappaName() const
    {
      return kappaName_;
    }
    //- Given patch temperature calculate corresponding K field
    tmp<scalarField> kappa(const scalarField& Tp) const;
    //- Write
    void write(Ostream&) const;
};
}  // namespace mousse
#endif
