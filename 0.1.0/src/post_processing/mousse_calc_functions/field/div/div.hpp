#ifndef POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_DIV_DIV_HPP_
#define POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_DIV_DIV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::div
// Description
//   Writes scalar fields corresponding to the divergence of the supplied
//   field (name) for each time.

#include "calc_type.hpp"


namespace mousse {
namespace calcTypes {

class div
:
  public calcType
{
protected:
  // Member Functions
    // Calculation routines
      //- Initialise - typically setting static variables,
      //  e.g. command line arguments
      virtual void init();
      //- Pre-time loop calculations
      virtual void preCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
      //- Time loop calculations
      virtual void calc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
    // I-O
      //- Write div fields
      template<class Type>
      void writeDivField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("div");
  // Constructors
    //- Construct null
    div();
    //- Disallow default bitwise copy construct
    div(const div&) = delete;
    //- Disallow default bitwise assignment
    div& operator=(const div&) = delete;
  //- Destructor
  virtual ~div();
};

}  // namespace calcTypes
}  // namespace mousse

#include "write_div_field.ipp"

#endif
