// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchDistMethod
// Description
//   Specialisation of patchDist for wall distance calculation
// SourceFiles
//   patch_dist_method.cpp
#ifndef patch_dist_method_hpp_
#define patch_dist_method_hpp_
#include "dictionary.hpp"
#include "hash_set.hpp"
#include "vol_fields_fwd.hpp"
#include "map_poly_mesh.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class fvMesh;
class patchDistMethod
{
protected:
  // Protected Member Data
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Set of patch IDs
    const labelHashSet patchIDs_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    patchDistMethod(const patchDistMethod&);
    //- Disallow default bitwise assignment
    void operator=(const patchDistMethod&);
public:
  //- Runtime type information
  TypeName("patchDistMethod");
  // Declare runtime construction
    declareRunTimeSelectionTable
    (
      autoPtr,
      patchDistMethod,
      dictionary,
      (
        const dictionary& dict,
        const fvMesh& mesh,
        const labelHashSet& patchIDs
      ),
      (dict, mesh, patchIDs)
    );
  // Constructors
    //- Construct from mesh and patch ID set
    patchDistMethod
    (
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
  // Selectors
    static autoPtr<patchDistMethod> New
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
  //- Destructor
  virtual ~patchDistMethod();
  // Static Functions
    //- Return the patch types for y and n
    //  These are fixedValue for the set provided otherwise zero-gradient
    template<class Type>
    static wordList patchTypes
    (
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
  // Member Functions
    //- Return the patchIDs
    const labelHashSet& patchIDs() const
    {
      return patchIDs_;
    }
    //- Update cached geometry when the mesh moves
    virtual bool movePoints()
    {
      return true;
    }
    //- Update cached topology and geometry when the mesh changes
    virtual void updateMesh(const mapPolyMesh&)
    {}
    //- Correct the given distance-to-patch field
    virtual bool correct(volScalarField& y) = 0;
    //- Correct the given distance-to-patch and normal-to-patch fields
    virtual bool correct(volScalarField& y, volVectorField& n) = 0;
};
}  // namespace mousse
#ifdef NoRepository
#   include "patch_dist_method_templates.cpp"
#endif
#endif
