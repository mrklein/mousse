// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceWriter
// Description
//   Base class for surface writers
// SourceFiles
//   surface_writer.cpp
#ifndef surface_writer_hpp_
#define surface_writer_hpp_
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "point_field.hpp"
#include "face_list.hpp"
#include "file_name.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class surfaceWriter
{
public:
  //- Runtime type information
  TYPE_NAME("surfaceWriter");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      surfaceWriter,
      word,
      (),
      ()
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      surfaceWriter,
      wordDict,
      (
        const dictionary& optDict
      ),
      (optDict)
    );
  // Selectors
    //- Return a reference to the selected surfaceWriter
    static autoPtr<surfaceWriter> New(const word& writeType);
    //- Return a reference to the selected surfaceWriter
    //  Select with extra write option
    static autoPtr<surfaceWriter> New
    (
      const word& writeType,
      const dictionary& writeOptions
    );
  // Constructors
    //- Construct null
    surfaceWriter();
  //- Destructor
  virtual ~surfaceWriter();
  // Member Functions
    //- True if the surface format supports geometry in a separate file.
    //  False if geometry and field must be in a single file
    virtual bool separateGeometry()
    {
      return false;
    }
    //- Write single surface geometry to file.
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const bool /*verbose*/ = false
    ) const
    {}
    //- Write scalarField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const word& /*fieldName*/,          // name of field
      const Field<scalar>& /*values*/,
      const bool /*isNodeValues*/,
      const bool /*verbose*/ = false
    ) const
    {}
    //- Write vectorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const word& /*fieldName*/,          // name of field
      const Field<vector>& /*values*/,
      const bool /*isNodeValues*/,
      const bool /*verbose*/ = false
    ) const
    {}
    //- Write sphericalTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const word& /*fieldName*/,          // name of field
      const Field<sphericalTensor>& /*values*/,
      const bool /*isNodeValues*/,
      const bool /*verbose*/ = false
    ) const
    {}
    //- Write symmTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const word& /*fieldName*/,          // name of field
      const Field<symmTensor>& /*values*/,
      const bool /*isNodeValues*/,
      const bool /*verbose*/ = false
    ) const
    {}
    //- Write tensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& /*outputDir*/,      // <case>/surface/TIME
      const fileName& /*surfaceName*/,    // name of surface
      const pointField& /*points*/,
      const faceList& /*faces*/,
      const word& /*fieldName*/,          // name of field
      const Field<tensor>& /*values*/,
      const bool /*isNodeValues*/,
      const bool /*verbose*/ = false
    ) const
    {}
};
}  // namespace mousse
#endif
