// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidMixtureProperties
// Description
//   A mixture of solids
//   An example of a two component solid mixture:
//   \verbatim
//     <parentDictionary>
//     {
//       C
//       {
//         defaultCoeffs   yes;     // employ default coefficients
//       }
//       ash
//       {
//         defaultCoeffs   no;
//         ashCoeffs
//         {
//           ... user defined properties for ash
//         }
//       }
//     }
//   \endverbatim
// SourceFiles
//   solid_mixture_properties.cpp
//   see_also
//   foam::solid_properties
#ifndef solid_mixture_properties_hpp_
#define solid_mixture_properties_hpp_
#include "scalar_field.hpp"
#include "ptr_list.hpp"
#include "solid_properties.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class solidMixtureProperties
{
  // Private data
    //- The names of the solids
    List<word> components_;
    //- The solidProperties properties
    PtrList<solidProperties> properties_;
public:
  // Constructors
    //- Construct from dictionary
    solidMixtureProperties(const dictionary&);
    //- Construct copy
    solidMixtureProperties(const solidMixtureProperties& lm);
    //- Construct and return a clone
    virtual autoPtr<solidMixtureProperties> clone() const
    {
      return autoPtr<solidMixtureProperties>
      (
        new solidMixtureProperties(*this)
      );
    }
  //- Destructor
  virtual ~solidMixtureProperties()
  {}
  // Selectors
    //- Select construct from dictionary
    static autoPtr<solidMixtureProperties> New(const dictionary&);
  // Member Functions
    //- Return the solidProperties names
    inline const List<word>& components() const
    {
      return components_;
    }
    //- Return the solidProperties properties
    inline const PtrList<solidProperties>& properties() const
    {
      return properties_;
    }
    //- Return the number of solids in the mixture
    inline label size() const
    {
      return components_.size();
    }
    //- Returns the mass fractions, given mole fractions
    scalarField Y(const scalarField& X) const;
    //- Returns the mole fractions, given mass fractions
    scalarField X(const scalarField& Y) const;
    //- Calculate the mixture density [kg/m^3] as a function of
    //  volume fractions
    scalar rho(const scalarField& X) const;
    //- Calculate the mixture heat capacity [J/(kg K)] as a function
    //  of mass fractions
    scalar Cp(const scalarField& Y) const;
};
}  // namespace mousse
#endif
