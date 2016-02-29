#ifndef POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_MAG_MAG_HPP_
#define POST_PROCESSING_MOUSSE_CALC_FUNCTIONS_FIELD_MAG_MAG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::mag
// Description
//   Calculates and writes the magnitude of a field for each time
// SourceFiles
//   mag.cpp
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class mag
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
      //- Write component fields
      template<class Type>
      void writeMagField
      (
        const IOobject& header,
        const fvMesh& mesh,
        bool& processed
      );
public:
  //- Runtime type information
  TYPE_NAME("mag");
  // Constructors
    //- Construct null
    mag();
    //- Disallow default bitwise copy construct
    mag(const mag&) = delete;
    //- Disallow default bitwise assignment
    mag& operator=(const mag&) = delete;
  //- Destructor
  virtual ~mag();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_mag_field.cpp"
#endif
#endif
