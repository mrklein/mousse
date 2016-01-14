// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::forceCoeffs
// Group
//   grpForcesFunctionObjects
// Description
//   This function object extends the mousse::forces function object by providing
//   lift, drag and moment coefficients.  The data can optionally be output into
//   bins, defined in a given direction.
//   Example of function object specification:
//   \verbatim
//   forceCoeffs1
//   {
//     type        forceCoeffs;
//     functionObjectLibs ("libforces.so");
//     ...
//     log         yes;
//     patches     (walls);
//     liftDir     (0 1 0);
//     dragDir     (-1 0 0);
//     pitchAxis   (0 0 1);
//     magUInf     100;
//     lRef        3.5;
//     Aref        2.2;
//     binData
//     {
//       nBin        20;
//       direction   (1 0 0);
//       cumulative  yes;
//     }
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: forces       | yes         |
//     log          | write force data to standard output | no | no
//     patches      | patches included in the forces calculation | yes |
//     liftDir      | lift direction          | yes         |
//     dragDir      | drag direction          | yes         |
//     pitchAxis    | picth axis              | yes         |
//     magUInf      | free stream velocity magnitude | yes  |
//     lRef         | reference length scale for moment calculations | yes |
//     Aref         | reference area          | yes |
//   \endtable
//   Bin data is optional, but if the dictionary is present, the entries must
//   be defined according o
//   \table
//     nBin         | number of data bins     | yes         |
//     direction    | direction along which bins are defined | yes |
//     cumulative   | bin data accumulated with incresing distance | yes |
//   \endtable
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
//   mousse::forces
// SourceFiles
//   force_coeffs.cpp
//   i_oforce_coeffs.hpp
#ifndef force_coeffs_hpp_
#define force_coeffs_hpp_
#include "forces.hpp"
namespace mousse
{
class forceCoeffs
:
  public forces
{
  // Private data
    // Force coefficient geometry
      //- Lift
      vector liftDir_;
      //- Drag
      vector  dragDir_;
      //- Pitch
      vector pitchAxis_;
    // Free-stream conditions
      //- Velocity magnitude
      scalar magUInf_;
    // Reference scales
      //- Length
      scalar lRef_;
      //- Area
      scalar Aref_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    forceCoeffs(const forceCoeffs&);
    //- Disallow default bitwise assignment
    void operator=(const forceCoeffs&);
protected:
  //- Output file header information
  virtual void writeFileHeader(const label i);
public:
  //- Runtime type information
  TYPE_NAME("forceCoeffs");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    forceCoeffs
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~forceCoeffs();
  // Member Functions
    //- Read the forces data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Write the forces
    virtual void write();
};
}  // namespace mousse
#endif
