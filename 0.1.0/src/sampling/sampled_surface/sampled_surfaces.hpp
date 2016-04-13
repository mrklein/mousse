#ifndef SAMPLING_SAMPLED_SURFACE_SAMPLED_SURFACES_HPP_
#define SAMPLING_SAMPLED_SURFACE_SAMPLED_SURFACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledSurfaces
// Description
//   Set of surfaces to sample.
//   The write() method is used to sample and write files.

#include "sampled_surface.hpp"
#include "surface_writer.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "word_re_list.hpp"
#include "ioobject_list.hpp"


namespace mousse {

class fvMesh;
class dictionary;


class sampledSurfaces
:
  public PtrList<sampledSurface>
{
  // Private classes
    //- Class used for surface merging information
    class mergeInfo
    {
    public:
      pointField points;
      faceList   faces;
      labelList  pointsMap;
      //- Clear all storage
      void clear()
      {
        points.clear();
        faces.clear();
        pointsMap.clear();
      }
    };
  // Static data members
    //- Output verbosity
    static bool verbose_;
    //- Tolerance for merging points (fraction of mesh bounding box)
    static scalar mergeTol_;
  // Private data
    //- Name of this set of surfaces,
    //  Also used as the name of the sampledSurfaces directory.
    const word name_;
    //- Const reference to fvMesh
    const fvMesh& mesh_;
    //- Load fields from files (not from objectRegistry)
    const bool loadFromFiles_;
    //- Output path
    fileName outputPath_;
    // Read from dictonary
      //- Names of fields to sample
      wordReList fieldSelection_;
      //- Interpolation scheme to use
      word interpolationScheme_;
    // surfaces
      //- Information for merging surfaces
      List<mergeInfo> mergeList_;
    // Calculated
      //- Surface formatter
      autoPtr<surfaceWriter> formatter_;
  // Private Member Functions
    //- Return number of fields
    label classifyFields();
    //- Write geometry only
    void writeGeometry() const;
    //- Write sampled fieldName on surface and on outputDir path
    template<class Type>
    void writeSurface
    (
      const Field<Type>& values,
      const label surfI,
      const word& fieldName,
      const fileName& outputDir
    );
    //- Sample and write a particular volume field
    template<class Type>
    void sampleAndWrite
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    //- Sample and write a particular surface field
    template<class Type>
    void sampleAndWrite
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>&
    );
    //- Sample and write all sampled fields
    template<class Type> void sampleAndWrite(const IOobjectList& objects);
public:
  //- Runtime type information
  TYPE_NAME("surfaces");
  // Constructors
    //- Construct for given objectRegistry and dictionary
    //  allow the possibility to load fields from files
    sampledSurfaces
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct and assignment
    sampledSurfaces(const sampledSurfaces&) = delete;
    void operator=(const sampledSurfaces&) = delete;
  //- Destructor
  virtual ~sampledSurfaces();
  // Member Functions
    //- Does any of the surfaces need an update?
    virtual bool needsUpdate() const;
    //- Mark the surfaces as needing an update.
    //  May also free up unneeded data.
    //  Return false if all surfaces were already marked as expired.
    virtual bool expire();
    //- Update the surfaces as required and merge surface points (parallel).
    //  Return false if no surfaces required an update.
    virtual bool update();
    //- Return name of the set of surfaces
    virtual const word& name() const
    {
      return name_;
    }
    //- Set verbosity level
    void verbose(const bool verbosity = true);
    //- Execute, currently does nothing
    virtual void execute();
    //- Execute at the final time-loop, currently does nothing
    virtual void end();
    //- Called when time was set at the end of the Time::operator++
    virtual void timeSet();
    //- Sample and write
    virtual void write();
    //- Read the sampledSurfaces dictionary
    virtual void read(const dictionary&);
    //- Update for changes of mesh - expires the surfaces
    virtual void updateMesh(const mapPolyMesh&);
    //- Update for mesh point-motion - expires the surfaces
    virtual void movePoints(const polyMesh&);
    //- Update for changes of mesh due to readUpdate - expires the surfaces
    virtual void readUpdate(const polyMesh::readUpdateState state);
};
}  // namespace mousse

#include "sampled_surfaces.ipp"

#endif
