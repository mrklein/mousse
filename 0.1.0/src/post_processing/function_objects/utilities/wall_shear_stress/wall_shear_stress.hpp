// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallShearStress
// Group
//   grpUtilitiesFunctionObjects
// Description
//   This function object evaluates and outputs the shear stress at wall
//   patches.  The result is written as a volVectorField to time directories as
//   field 'wallShearStress'
//     \f[
//       Stress = R \dot n
//     \f]
//   where
//   \vartable
//     R       | stress tensor
//     n       | patch normal vector (into the domain)
//   \endvartable
//   The shear stress (symmetrical) tensor field is retrieved from the
//   turbulence model.  All wall patches are included by default; to restrict
//   the calculation to certain patches, use the optional 'patches' entry.
//   Example of function object specification:
//   \verbatim
//   wallShearStress1
//   {
//     type        wallShearStress;
//     functionObjectLibs ("libutilityFunctionObjects.so");
//     ...
//     patches     (".*Wall");
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property | Description               | Required    | Default value
//     type     | type name: wallShearStress | yes        |
//     patches  | list of patches to process | no         | all wall patches
//   \endtable
// SourceFiles
//   wall_shear_stress.cpp
//   i_owall_shear_stress.hpp
#ifndef wall_shear_stress_hpp_
#define wall_shear_stress_hpp_
#include "function_object_file.hpp"
#include "vol_fields_fwd.hpp"
#include "switch.hpp"
#include "ofstream.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fvMesh;
class wallShearStress
:
  public functionObjectFile
{
protected:
  // Protected data
    //- Name of this set of wallShearStress object
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Switch to send output to Info as well as to file
    Switch log_;
    //- Optional list of patches to process
    labelHashSet patchSet_;
  // Protected Member Functions
    //- File header information
    virtual void writeFileHeader(const label i);
    //- Calculate the shear stress
    void calcShearStress
    (
      const fvMesh& mesh,
      const volSymmTensorField& Reff,
      volVectorField& shearStress
    );
public:
  //- Runtime type information
  TYPE_NAME("wallShearStress");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    wallShearStress
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    wallShearStress(const wallShearStress&) = delete;
    //- Disallow default bitwise assignment
    wallShearStress& operator=(const wallShearStress&) = delete;
  //- Destructor
  virtual ~wallShearStress();
  // Member Functions
    //- Return name of the set of wallShearStress
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the wallShearStress data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the wallShearStress and write
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
