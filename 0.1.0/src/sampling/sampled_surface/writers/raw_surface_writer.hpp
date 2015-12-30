// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rawSurfaceWriter
// Description
//   A surfaceWriter for raw output.
// SourceFiles
//   raw_surface_writer.cpp
#ifndef raw_surface_writer_hpp_
#define raw_surface_writer_hpp_
#include "surface_writer.hpp"
namespace mousse
{
class rawSurfaceWriter
:
  public surfaceWriter
{
  // Private Member Functions
    static inline void writeLocation
    (
      Ostream&,
      const pointField&,
      const label pointI
    );
    static inline void writeLocation
    (
      Ostream&,
      const pointField&,
      const faceList&,
      const label faceI
    );
    template<class Type>
    static void writeHeader
    (
      Ostream&,
      const word& fieldName,
      const Field<Type>&
    );
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
  TypeName("raw");
  // Constructors
    //- Construct null
    rawSurfaceWriter();
  //- Destructor
  virtual ~rawSurfaceWriter();
  // Member Functions
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
    //- Write symmTensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<symmTensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
    //- Write tensorField for a single surface to file.
    //  One value per face or vertex (isNodeValues = true)
    virtual void write
    (
      const fileName& outputDir,      // <case>/surface/TIME
      const fileName& surfaceName,    // name of surface
      const pointField& points,
      const faceList& faces,
      const word& fieldName,          // name of field
      const Field<tensor>& values,
      const bool isNodeValues,
      const bool verbose = false
    ) const;
};
}  // namespace mousse
#endif
