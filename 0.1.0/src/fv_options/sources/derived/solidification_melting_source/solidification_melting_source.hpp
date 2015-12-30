// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::solidificationMeltingSource
// Description
//   This source is designed to model the effect of solidification and melting
//   processes, e.g. windhield defrosting.  The phase change occurs at the
//   melting temperature, \c Tmelt.
//   The presence of the solid phase in the flow field is incorporated into the
//   model as a momentum porosity contribution; the energy associated with the
//   phase change is added as an enthalpy contribution.
//   Based on the references:
//    1. V.R. Voller and C. Prakash, A fixed grid numerical modelling
//     methodology for convection-diffusion mushy phase-change problems,
//     Int. J. Heat Mass Transfer 30(8):17091719, 1987.
//    2. C.R. Swaminathan. and V.R. Voller, A general enthalpy model for
//     modeling solidification processes, Metallurgical Transactions
//     23B:651664, 1992.
//   The model generates a field \c \<name\>:alpha1 which can be visualised to
//   to show the melt distribution as a fraction [0-1]
//   \heading Source usage
//   Example usage:
//   \verbatim
//   solidificationMeltingSource1
//   {
//     type            solidificationMeltingSource;
//     active          yes;
//     solidificationMeltingSourceCoeffs
//     {
//       selectionMode   cellZone;
//       cellZone        iceZone;
//       Tmelt           273;
//       L               334000;
//       thermoMode      thermo;
//       beta            50e-6;
//       rhoRef          800;
//     }
//   }
//   \endverbatim
//   Where:
//   \table
//     Property     | Description             | Required    | Default value
//     Tmelt        | Melting temperature [K] | yes         |
//     L            | Latent heat of fusion [J/kg] | yes    |
//     relax        | Relaxation coefficient [0-1] | no     | 0.9
//     thermoMode   | Thermo mode [thermo|lookup] | yes     |
//     rhoRef       | Reference (solid) density | yes       |
//     rhoName      | Name of density field   | no          | rho
//     TName        | Name of temperature field | no        | T
//     CpName       | Name of specific heat capacity field | no | Cp
//     UName        | Name of velocity field  | no          | U
//     phiName      | Name of flux field      | no          | phi
//     Cu           | Model coefficient       | no          | 100000
//     q            | Model coefficient       | no          | 0.001
//     beta         | Thermal expansion coefficient [1/K] | yes |
//     g            | Accelerartion due to gravity | no     |
//   \endtable
// SourceFiles
//   solidification_melting_source.cpp
//   solidification_melting_source_io.cpp
#ifndef solidification_melting_source_hpp_
#define solidification_melting_source_hpp_
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "cell_set_option.hpp"
#include "named_enum.hpp"
namespace mousse
{
namespace fv
{
class solidificationMeltingSource
:
  public cellSetOption
{
public:
  enum thermoMode
  {
    mdThermo,
    mdLookup
  };
  static const NamedEnum<thermoMode, 2> thermoModeTypeNames_;
private:
  // Private data
    //- Temperature at which melting occurs [K]
    scalar Tmelt_;
    //- Latent heat of fusion [J/kg]
    scalar L_;
    //- Phase fraction under-relaxation coefficient
    scalar relax_;
    //- Thermodynamics mode
    thermoMode mode_;
    //- Reference density - typically the solid density
    scalar rhoRef_;
    //- Name of temperature field - default = "T" (optional)
    word TName_;
    //- Name of specific heat capacity field - default = "Cp" (optional)
    word CpName_;
    //- Name of velocity field - default = "U" (optional)
    word UName_;
    //- Name of flux field - default = "phi" (optional)
    word phiName_;
    //- Mushy region momentum sink coefficient [1/s]; default = 10^5
    scalar Cu_;
    //- Coefficient used in porosity calc - default = 0.001
    scalar q_;
    //- Thermal expansion coefficient [1/K]
    scalar beta_;
    //- Phase fraction indicator field
    volScalarField alpha1_;
    //- Current time index (used for updating)
    label curTimeIndex_;
    //- Temperature change cached for source calculation when alpha1 updated
    scalarField deltaT_;
  // Private Member Functions
    //- Return the specific heat capacity field
    tmp<volScalarField> Cp() const;
    //- Return the gravity vector
    vector g() const;
    //- Update the model
    void update(const volScalarField& Cp);
    //- Helper function to apply to the energy equation
    template<class RhoFieldType>
    void apply(const RhoFieldType& rho, fvMatrix<scalar>& eqn);
    //- Disallow default bitwise copy construct
    solidificationMeltingSource(const solidificationMeltingSource&);
    //- Disallow default bitwise assignment
    void operator=(const solidificationMeltingSource&);
public:
  //- Runtime type information
  TypeName("solidificationMeltingSource");
  // Constructors
    //- Construct from explicit source name and mesh
    solidificationMeltingSource
    (
      const word& sourceName,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  // Member Functions
    // Add explicit and implicit contributions
      //- Add explicit contribution to enthalpy equation
      virtual void addSup(fvMatrix<scalar>& eqn, const label fieldI);
      //- Add implicit contribution to momentum equation
      virtual void addSup(fvMatrix<vector>& eqn, const label fieldI);
    // Add explicit and implicit contributions to compressible equation
      //- Add explicit contribution to compressible enthalpy equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<scalar>& eqn,
        const label fieldI
      );
      //- Add implicit contribution to compressible momentum equation
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
#ifdef NoRepository
  #include "solidification_melting_source_templates.cpp"
#endif
#endif
