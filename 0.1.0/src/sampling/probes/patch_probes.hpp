#ifndef SAMPLING_PROBES_PATCH_PROBES_HPP_
#define SAMPLING_PROBES_PATCH_PROBES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchProbes
// Description
//   Set of locations to sample.at patches
//   Call write() to sample and write files.

#include "probes.hpp"


namespace mousse {

// Forward declaration of classes
class objectRegistry;
class dictionary;
class fvMesh;
class mapPolyMesh;


class patchProbes
:
  public probes
{
  // Private data
    //- Patch name
    word patchName_;
  // Private Member Functions
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
    //- Sample and write all the fields of the given type
    template<class Type>
    void sampleAndWrite(const fieldGroup<Type>&);
    //- Sample and write all the surface fields of the given type
    template<class Type>
    void sampleAndWriteSurfaceFields(const fieldGroup<Type>&);
    //- Sample a volume field at all locations
    template<class Type>
    tmp<Field<Type>> sample
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    //- Sample a surface field at all locations
    template<class Type>
    tmp<Field<Type>> sample
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>&
    ) const;
    //- Sample a single field on all sample locations
    template<class Type>
    tmp<Field<Type>> sample(const word& fieldName) const;
public:
  //- Runtime type information
  TYPE_NAME("patchProbes");
  // Constructors
    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    patchProbes
    (
      const word& name,
      const objectRegistry&,
      const dictionary&,
      const bool loadFromFiles = false
    );
    //- Disallow default bitwise copy construct
    patchProbes(const patchProbes&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const patchProbes&) = delete;
  //- Destructor
  virtual ~patchProbes();
  //- Public members
    //- Sample and write
    virtual void write();
    //- Read
    virtual void read(const dictionary&);
    //- Find elements containing patchProbes
    virtual void findElements(const fvMesh&);
};

}  // namespace mousse

#include "patch_probes.ipp"

#endif

