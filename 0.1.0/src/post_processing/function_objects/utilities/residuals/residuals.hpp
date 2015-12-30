// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::residuals
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object writes out the initial residual for specified fields.
//   Example of function object specification:
//   \verbatim
//   residuals
//   {
//     type            residuals;
//     outputControl   timeStep;
//     outputInterval  1;
//     fields
//     (
//       U
//       p
//     );
//   }
//   \endverbatim
//   Output data is written to the dir postProcessing/residuals/\<timeDir\>/
//   For vector/tensor fields, e.g. U, where an equation is solved for each
//   component, the largest residual of each component is written out.
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   residuals.cpp
//   i_oresiduals.hpp
#ifndef residuals_hpp_
#define residuals_hpp_
#include "function_object_file.hpp"
#include "primitive_fields_fwd.hpp"
#include "vol_fields_fwd.hpp"
#include "hash_set.hpp"
#include "ofstream.hpp"
#include "switch.hpp"
#include "named_enum.hpp"
#include "solver_performance.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class residuals
:
  public functionObjectFile
{
protected:
  // Protected data
    //- Name of this set of residuals
    //  Also used as the name of the output directory
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Fields to write residuals
    wordList fieldSet_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    residuals(const residuals&);
    //- Disallow default bitwise assignment
    void operator=(const residuals&);
    //- Output file header information
    virtual void writeFileHeader(const label i);
public:
  //- Runtime type information
  TypeName("residuals");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    residuals
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~residuals();
  // Member Functions
    //- Return name of the set of field min/max
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the field min/max data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the field min/max
    template<class Type>
    void writeResidual(const word& fieldName);
    //- Write the residuals
    virtual void write();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Update for changes of mesh
    virtual void movePoints(const polyMesh&)
    {}
};
}  // namespace mousse
#ifdef NoRepository
#   include "residuals_templates.cpp"
#endif
#endif
