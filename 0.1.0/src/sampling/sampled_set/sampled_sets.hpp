// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledSets
// Description
//   Set of sets to sample.
//   Call sampledSets.write() to sample&write files.
// SourceFiles
//   sampled_sets.cpp
#ifndef sampled_sets_hpp_
#define sampled_sets_hpp_
#include "sampled_set.hpp"
#include "vol_fields_fwd.hpp"
#include "mesh_search.hpp"
#include "interpolation.hpp"
#include "coord_set.hpp"
#include "writer.hpp"
#include "word_re_list.hpp"
namespace mousse
{
class objectRegistry;
class dictionary;
class fvMesh;
class sampledSets
:
  public PtrList<sampledSet>
{
  // Private classes
    //- Class used for grouping field types
    template<class Type>
    class fieldGroup
    :
      public DynamicList<word>
    {
    public:
      //- The set formatter
      autoPtr< writer<Type> > formatter;
      //- Construct null
      fieldGroup()
      :
        DynamicList<word>(0),
        formatter(NULL)
      {}
      //- Construct for a particular format
      fieldGroup(const word& writeFormat)
      :
        DynamicList<word>(0),
        formatter(writer<Type>::New(writeFormat))
      {}
      //- Reset format and field list
      void clear()
      {
        DynamicList<word>::clear();
        formatter.clear();
      }
      //- Assign a new formatter
      void operator=(const word& writeFormat)
      {
        formatter = writer<Type>::New(writeFormat);
      }
    };
    //- Class used for sampling volFields
    template<class Type>
    class volFieldSampler
    :
      public List<Field<Type> >
    {
      //- Name of this collection of values
      const word name_;
    public:
      //- Construct interpolating field to the sampleSets
      volFieldSampler
      (
        const word& interpolationScheme,
        const GeometricField<Type, fvPatchField, volMesh>& field,
        const PtrList<sampledSet>&
      );
      //- Construct mapping field to the sampleSets
      volFieldSampler
      (
        const GeometricField<Type, fvPatchField, volMesh>& field,
        const PtrList<sampledSet>&
      );
      //- Construct from components
      volFieldSampler
      (
        const List<Field<Type> >& values,
        const word& name
      );
      //- Return the field name
      const word& name() const
      {
        return name_;
      }
    };
  // Static data members
    //- Output verbosity
    static bool verbose_;
  // Private data
    //- Name of this set of sets,
    //  Also used as the name of the sampledSets directory.
    word name_;
    //- Const reference to fvMesh
    const fvMesh& mesh_;
    //- Keep the dictionary to recreate sets for moving mesh cases
    dictionary dict_;
    //- Load fields from files (not from objectRegistry)
    bool loadFromFiles_;
    //- Output path
    fileName outputPath_;
    //- Mesh search engine
    meshSearch searchEngine_;
    // Read from dictonary
      //- Names of fields to sample
      wordReList fieldSelection_;
      //- Interpolation scheme to use
      word interpolationScheme_;
      //- Output format to use
      word writeFormat_;
    // Categorized scalar/vector/tensor fields
      fieldGroup<scalar> scalarFields_;
      fieldGroup<vector> vectorFields_;
      fieldGroup<sphericalTensor> sphericalTensorFields_;
      fieldGroup<symmTensor> symmTensorFields_;
      fieldGroup<tensor> tensorFields_;
    // Merging structures
      PtrList<coordSet> masterSampledSets_;
      labelListList indexSets_;
  // Private Member Functions
    //- Clear old field groups
    void clearFieldGroups();
    //- Append fieldName to the appropriate group
    label appendFieldGroup(const word& fieldName, const word& fieldType);
    //- Classify field types, returns the number of fields
    label classifyFields();
    //- Combine points from all processors. Sort by curveDist and produce
    //  index list. Valid result only on master processor.
    void combineSampledSets
    (
      PtrList<coordSet>& masterSampledSets,
      labelListList& indexSets
    );
    //- Combine values from all processors.
    //  Valid result only on master processor.
    template<class T>
    void combineSampledValues
    (
      const PtrList<volFieldSampler<T> >& sampledFields,
      const labelListList& indexSets,
      PtrList<volFieldSampler<T> >& masterFields
    );
    template<class Type>
    void writeSampleFile
    (
      const coordSet& masterSampleSet,
      const PtrList<volFieldSampler<Type> >& masterFields,
      const label setI,
      const fileName& timeDir,
      const writer<Type>& formatter
    );
    template<class Type>
    void sampleAndWrite(fieldGroup<Type>& fields);
    //- Disallow default bitwise copy construct and assignment
    sampledSets(const sampledSets&);
    void operator=(const sampledSets&);
public:
  //- Runtime type information
  TypeName("sets");
  // Constructors
    //- Construct for given objectRegistry and dictionary
    //  allow the possibility to load fields from files
    sampledSets
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
  //- Destructor
  virtual ~sampledSets();
  // Member Functions
    //- Return name of the set of probes
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
    //- Read the sampledSets
    virtual void read(const dictionary&);
    //- Correct for mesh changes
    void correct();
    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh&);
    //- Update for mesh point-motion
    virtual void movePoints(const polyMesh&);
    //- Update for changes of mesh due to readUpdate
    virtual void readUpdate(const polyMesh::readUpdateState state);
};
}  // namespace mousse
#ifdef NoRepository
#   include "sampled_sets_templates.cpp"
#endif
#endif
