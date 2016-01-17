// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::yPlus
// Group
//   grpUtilitiesFunctionObjects
// Description
//   Evaluates and outputs turbulence y+ for  models.  Values written to
//   time directories as field 'yPlus'
// SourceFiles
//   y_plus.cpp
//   i_oy_plus.hpp
#ifndef y_plus_hpp_
#define y_plus_hpp_
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
class yPlus
:
  public functionObjectFile
{
  // Private data
    //- Name of this set of yPlus objects
    word name_;
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
    //- Switch to send output to Info as well as to file
    Switch log_;
    //- Name of mass/volume flux field (optional, default = phi)
    word phiName_;
  // Private Member Functions
    //- File header information
    virtual void writeFileHeader(const label i);
    //- Calculate y+
    template<class TurbulenceModel>
    void calcYPlus
    (
      const TurbulenceModel& turbulenceModel,
      const fvMesh& mesh,
      volScalarField& yPlus
    );
public:
  //- Runtime type information
  TYPE_NAME("yPlus");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    yPlus
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    yPlus(const yPlus&) = delete;
    //- Disallow default bitwise assignment
    yPlus& operator=(const yPlus&) = delete;
  //- Destructor
  virtual ~yPlus();
  // Member Functions
    //- Return name of the set of yPlus
    virtual const word& name() const
    {
      return name_;
    }
    //- Read the yPlus data
    virtual void read(const dictionary&);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Calculate the yPlus and write
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
  #include "y_plus_templates.cpp"
#endif
#endif
