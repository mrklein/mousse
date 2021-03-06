#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PRESSURE_TOOLS_PRESSURE_TOOLS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_PRESSURE_TOOLS_PRESSURE_TOOLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pressureTools
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object includes tools to manipulate the pressure into
//   different forms.  These currently include:
//   - static pressure
//     \f[
//       p_s = \rho p_k
//     \f]
//   - total pressure
//     \f[
//       p_T = p_{ref} + p_s + 0.5 \rho |U|^2
//     \f]
//   - static pressure coefficient
//     \f[
//       Cp_s = \frac{p_s - p_{\inf}}{0.5 \rho_{\inf} |U_{\inf}|^2}
//     \f]
//   - total pressure coefficient
//     \f[
//       Cp_T = \frac{p_T - p_{\inf}}{0.5 \rho_{\inf} |U_{\inf}|^2}
//     \f]
//   where
//   \vartable
//     \rho        | density [kg/m3]
//     U           | velocity [m/s]
//     \rho_{\inf} | freestream density [kg/m3]
//     p_{\inf}    | freestream pressure [Pa]
//     U_{\inf}    | freestream velocity [m/s]
//     p_k         | kinematic pressure (p/rho)[m2/s2]
//     p_s         | pressure [Pa]
//     p_T         | total pressure [Pa]
//     p_{ref}     | reference pressure level [Pa]
//     Cp_{s}      | pressure coefficient
//     Cp_{T}      | total pressure coefficient
//   \endvartable
//   The function object will operate on both kinematic (\f$ p_k \f$) and static
//   pressure (\f$ p_s \f$) fields, and the result is written as a
//   volScalarField.
//   The modes of operation are:
//   \table
//     Mode                        | calcTotal | calcCoeff
//     static pressure             | no        | no
//     total pressure              | yes       | no
//     pressure coefficient        | no        | yes
//     total pressure coefficient  | yes       | yes
//   \endtable
//   Example of function object specification to calculate pressure coefficient:
//   \verbatim
//   pressureTools1
//   {
//     type        pressureTools;
//     functionObjectLibs ("libutilityFunctionObjects.so");
//     ...
//     calcTotal   no;
//     calcCoeff   yes;
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: pressureTools| yes         |
//     calcTotal    | Calculate total coefficient | yes     |
//     pRef         | Reference pressure for total pressure | no | 0.0
//     calcCoeff    | Calculate pressure coefficient | yes  |
//     pInf         | Freestream pressure for coefficient calculation | no |
//     UInf         | Freestream velocity for coefficient calculation | no |
//     rhoInf       | Freestream density for coefficient calculation | no |
//   \endtable

#include "vol_fields_fwd.hpp"
#include "dimensioned_scalar.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;


class pressureTools
{
  // Private data
    //- Name of this set of pressureTools objects
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- On/off switch
    bool active_;
    //- Name of pressure field, default is "p"
    word pName_;
    //- Name of velocity field, default is "U"
    word UName_;
    //- Name of density field, default is "rho"
    word rhoName_;
    // Total pressure calculation
      //- Flag to calculate total pressure
      bool calcTotal_;
      //- Reference pressure level
      scalar pRef_;
    // Pressure coefficient calculation
      //- Flag to calculate pressure coefficient
      bool calcCoeff_;
      //- Freestream pressure
      scalar pInf_;
      //- Freestream velocity
      vector UInf_;
      //- Freestream density
      scalar rhoInf_;
  // Private Member Functions
    //- Return the name of the derived pressure field
    word pName() const;
    //- Return the density scaling if supplied with kinematic pressure
    dimensionedScalar rhoScale(const volScalarField& p) const;
    //- Return the density field
    tmp<volScalarField> rho(const volScalarField& p) const;
    //- Return the reference pressure
    dimensionedScalar pRef() const;
    //- Calculate and return the dynamic pressure
    tmp<volScalarField> pDyn(const volScalarField& p) const;
    //- Convert to coeff by applying the freestream dynamic pressure scaling
    tmp<volScalarField> convertToCoeff(const volScalarField& p) const;
public:
  //- Runtime type information
  TYPE_NAME("pressureTools");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    pressureTools
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    pressureTools(const pressureTools&) = delete;
    //- Disallow default bitwise assignment
    pressureTools& operator=(const pressureTools&) = delete;
  //- Destructor
  virtual ~pressureTools();
  // Member Functions
    //- Return name of the set of pressureTools
    virtual const word& name() const { return name_; }
    //- Read the pressureTools data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the pressureTools and write
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};

}  // namespace mousse

#endif

