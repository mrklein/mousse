// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::calcType
// Description
//   Base class for post-processing calculation functions
// SourceFiles
//   calc_type.cpp
#ifndef calc_type_hpp_
#define calc_type_hpp_
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_cfd.hpp"
namespace mousse
{
class calcType
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    calcType(const calcType&);
    //- Disallow default bitwise assignment
    void operator=(const calcType&);
protected:
  // Protected Member Functions
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
      //- Post-time loop calculations
      virtual void postCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
public:
  //- Runtime type information
  TypeName("calcType");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      calcType,
      dictionary,
      (),
      ()
    );
  // Constructors
    //- Construct null
    calcType();
  // Selectors
    static autoPtr<calcType> New(const word& calcTypeName);
  //- Destructor
  virtual ~calcType();
  // Member Functions
    // Calculation routines - wrapped by exception handling loop
      //- Initialise - typically setting static variables,
      //  e.g. command line arguments
      void tryInit();
      //- Pre-time loop calculations
      void tryPreCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
      //- Time loop calculations
      void tryCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
      //- Post-time loop calculations
      void tryPostCalc
      (
        const argList& args,
        const Time& runTime,
        const fvMesh& mesh
      );
};
}  // namespace mousse
#endif
