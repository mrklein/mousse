// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcTypes::mag
// Description
//   Calculates and writes the magnitude of a field for each time
// SourceFiles
//   mag.cpp
#ifndef mag_hpp_
#define mag_hpp_
#include "calc_type.hpp"
namespace mousse
{
namespace calcTypes
{
class mag
:
  public calcType
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    mag(const mag&);
    //- Disallow default bitwise assignment
    void operator=(const mag&);
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
  //- Destructor
  virtual ~mag();
};
}  // namespace calcTypes
}  // namespace mousse
#ifdef NoRepository
#   include "write_mag_field.cpp"
#endif
#endif
