// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lagrangianWriter
// Description
//   Write fields (internal).
// SourceFiles
//   lagrangian_writer.cpp
//   lagrangian_writer_templates.cpp
#ifndef lagrangian_writer_hpp_
#define lagrangian_writer_hpp_
#include "ofstream.hpp"
#include "cloud.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
#include "vtk_mesh.hpp"
using namespace mousse;
namespace mousse
{
class volPointInterpolation;
class lagrangianWriter
{
  const vtkMesh& vMesh_;
  const bool binary_;
  const fileName fName_;
  const word cloudName_;
  std::ofstream os_;
  label nParcels_;
public:
  // Constructors
    //- Construct from components
    lagrangianWriter
    (
      const vtkMesh&,
      const bool binary,
      const fileName&,
      const word&,
      const bool dummyCloud
    );
  // Member Functions
    std::ofstream& os()
    {
      return os_;
    }
    void writeParcelHeader(const label nFields);
    //- Write IOField
    template<class Type>
    void writeIOField(const wordList&);
};
}  // namespace mousse
#ifdef NoRepository
  #include "lagrangian_writer_templates.cpp"
#endif
#endif
