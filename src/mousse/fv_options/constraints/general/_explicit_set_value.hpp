#ifndef FV_OPTIONS_CONSTRAINTS_GENERAL_EXPLICIT_SET_VALUE_TEXPLICIT_SET_VALUE_HPP_
#define FV_OPTIONS_CONSTRAINTS_GENERAL_EXPLICIT_SET_VALUE_TEXPLICIT_SET_VALUE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::explicitSetValue
// Description
//   Set values field values explicity.
//   Sources described by:
//   \verbatim
//   <Type>ExplicitSetValueCoeffs
//   {
//     injectionRate
//     {
//       k           30.7;
//       epsilon     1.5;
//     }
//   }
//   \endverbatim
// SeeAlso
//   mousse::fvOption

#include "cell_set_option.hpp"
#include "tuple2.hpp"


namespace mousse {
namespace fv {

template<class Type>
class ExplicitSetValue
:
  public cellSetOption
{
protected:
  // Protected data
    //- Source value per field
    List<Type> injectionRate_;
  // Protected functions
    //- Set the local field data
    void setFieldData(const dictionary& dict);
public:
  //- Runtime type information
  TYPE_NAME("explicitSetValue");
  // Constructors
    //- Construct from components
    ExplicitSetValue
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  // Member Functions
    // Evaluation
      //- Set value on field
      virtual void constrain(fvMatrix<Type>& eqn, const label fieldI);
    // IO
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};

}  // namespace fv
}  // namespace mousse

#include "_explicit_set_value.ipp"
#include "_explicit_set_value_io.ipp"

#endif
