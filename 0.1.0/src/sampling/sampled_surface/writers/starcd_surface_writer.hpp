// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::starcdSurfaceWriter
// Description
//   A surfaceWriter for STARCD files.
//   The geometry is written via the MeshedSurfaceProxy, the fields
//   are written in a trivial ASCII format with ID and VALUE as
//   so-called user data. These \c .usr files can be read into proSTAR
//   with these types of commands. For element data:
//   \verbatim
//     getuser FILENAME.usr cell scalar free
//     getuser FILENAME.usr cell vector free
//   \endverbatim
//   and for vertex data:
//   \verbatim
//     getuser FILENAME.usr vertex scalar free
//     getuser FILENAME.usr vertex vector free
//   \endverbatim
// Note
//   Only scalar and vector fields are supported directly.
//   A sphericalTensor is written as a scalar.
//   Other field types are not written.
// SourceFiles
//   starcd_surface_writer.cpp
#ifndef starcd_surface_writer_hpp_
#define starcd_surface_writer_hpp_
#include "surface_writer.hpp"
namespace mousse
{
class starcdSurfaceWriter
:
  public surfaceWriter
{
  // Private Member Functions
    template<class Type>
    static inline void writeData(Ostream&, const Type&);
    //- Templated write operation
    template<class Type>
    void writeTemplate
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const word& fieldName,
      const Field<Type>& values,
      const bool isNodeValues,
      const bool verbose
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("starcd");
  // Constructors
    //- Construct null
    starcdSurfaceWriter();
  //- Destructor
  virtual ~starcdSurfaceWriter();
  // Member Functions
    //- True if the surface format supports geometry in a separate file.
    //  False if geometry and field must be in a single file
    virtual bool separateGeometry()
    {
      return true;
    }
    //- Write single surface geometry to file.
    virtual void write
    (
      const fileName& outputDir,
      const fileName& surfaceName,
      const pointField& points,
      const faceList& faces,
      const bool verbose = false
    ) const;
    //- Write scalarField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<scalar>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write vectorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<vector>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write sphericalTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<sphericalTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
};
}  // namespace mousse
#endif
