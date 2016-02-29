#ifndef POST_PROCESSING_FUNCTION_OBJECTS_CLOUD_CLOUD_INFO_CLOUD_INFO_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_CLOUD_CLOUD_INFO_CLOUD_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cloudInfo
// Group
//   grpCloudFunctionObjects
// Description
//   This function object outputs Lagrangian cloud information to a file.  The
//   current outputs include:
//   - total current number of parcels
//   - total current mass of parcels
//   Example of function object specification:
//   \verbatim
//   cloudInfo1
//   {
//     type        cloudInfo;
//     functionObjectLibs ("libcloudFunctionObjects.so");
//     ...
//     clouds
//     (
//       kinematicCloud1
//       thermoCloud1
//     );
//   }
//   \endverbatim
//   \heading Function object usage
//   \table
//     Property     | Description             | Required    | Default value
//     type         | type name: cloudInfo    | yes         |
//     clouds       | list of clouds names to process |yes  |
//   \endtable
//   The output data of each cloud is written to a file named \<cloudName\>.dat
// SeeAlso
//   mousse::functionObject
//   mousse::OutputFilterFunctionObject
// SourceFiles
//   cloud_info.cpp
//   i_ocloud_info.hpp
#include "function_object_file.hpp"
#include "ptr_list.hpp"
#include "point_field_fwd.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;
class cloudInfo
:
  public functionObjectFile
{
protected:
  // Protected data
    //- Name of this set of cloudInfo object
    word name_;
    //- Reference to the database
    const objectRegistry& obr_;
    //- on/off switch
    bool active_;
  // Protected Member Functions
    //- File header information
    virtual void writeFileHeader(const label i);
public:
  //- Runtime type information
  TYPE_NAME("cloudInfo");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    cloudInfo
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    cloudInfo(const cloudInfo&) = delete;
    //- Disallow default bitwise assignment
    cloudInfo& operator=(const cloudInfo&) = delete;
  //- Destructor
  virtual ~cloudInfo();
  // Member Functions
    //- Return name of the cloudInfo object
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
    //- Write
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
