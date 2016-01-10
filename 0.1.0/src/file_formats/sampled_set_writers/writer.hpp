// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::writer
// Description
//   Base class for graphics format writing. Entry points are
//   - write(..). \n
//    Write to an Ostream a table of points with corresponding values.
//   - write(scalar/vector/sphericalTensor/symmTensor/tensor). \n
//    Write single scalar/vector/sphericalTensor/symmTensor/tensor.
//    Default is to write space separated components.
//   Example:
//   \verbatim
//     // Construct writer of xmgr type
//     autoPtr<writer<scalar> > scalarFormatter(writer<scalar>::New("xmgr"));
//     // Output list of points and corresponding values
//     scalarFormatter().write
//     (
//       coordSet(...)
//       "U.component(0)",   // name of values
//       vals                // values
//     );
//   \endverbatim
// SourceFiles
//   writer.cpp

#ifndef writer_hpp_
#define writer_hpp_

#include "file_name.hpp"
#include "word_list.hpp"
#include "vector.hpp"
#include "tensor.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "field.hpp"

namespace mousse
{

// Forward declaration of classes
class coordSet;
template<class Type>
class writer
{

protected:

  //- Generates filename from coordSet and sampled fields
  fileName getBaseName(const coordSet&, const wordList&) const;
  void writeCoord(const coordSet&, const label sampleI, Ostream&) const;

  //- Writes single-column ascii write. Column 1 is coordSet coordinate,
  //  columns 2 is the value. Uses write() function
  //  to write coordinate in correct format.
  void writeTable(const coordSet&, const List<Type>&, Ostream&) const;

  //- Writes multi-column ascii write. Column 1 is coordSet coordinate,
  //  columns 2..n are the values. Uses write() function
  //  to write coordinate in correct format.
  void writeTable
  (
    const coordSet&,
    const List<const List<Type>*>&,
    Ostream& os
  ) const;

  //- Writes a separator. Used by write functions.
  virtual void writeSeparator(Ostream& os) const;

public:
  //- Runtime type information
  TYPE_NAME("writer");

  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      writer,
      word,
      (),
      ()
    );

  // Selectors

    //- Return a reference to the selected writer
    static autoPtr<writer> New(const word& writeFormat);

  // Constructors

    //- Construct null
    writer();

  //- Destructor
  virtual ~writer() = 0;

  // Member Functions

    //- Generate file name with correct extension
    virtual fileName getFileName
    (
      const coordSet&,
      const wordList&
    ) const = 0;

    //- General entry point for writing.
    //  The data is organized in a set of point with one or more values
    //  per point
    virtual void write
    (
      const coordSet&,
      const wordList&,
      const List<const Field<Type>*>&,
      Ostream&
    ) const = 0;

    //- General entry point for writing.
    //  The data is organized in a set of point with one or more values
    //  per point
    virtual void write
    (
      const coordSet&,
      const wordList&,
      const List<Field<Type> >&,
      Ostream&
    ) const;

    //- General entry point for writing of multiple coordSets.
    //  Each coordSet (track) has same data variables.
    //  The data is per variable, per track, per point of track.
    //  If writeTracks adds connecting lines (wherever applicable)
    virtual void write
    (
      const bool writeTracks,
      const PtrList<coordSet>&,
      const wordList& valueSetNames,
      const List<List<Field<Type> > >&,
      Ostream&
    ) const = 0;

    //- Write scalar as ascii
    virtual Ostream& write(const scalar, Ostream&) const;
    template<class VSType>
    Ostream& writeVS(const VSType&, Ostream&) const;

    //- Write vector. Tab separated ascii
    virtual Ostream& write(const vector&, Ostream&) const;

    //- Write sphericalTensor. Tab separated ascii
    virtual Ostream& write(const sphericalTensor&, Ostream&) const;

    //- Write symmTensor. Tab separated ascii
    virtual Ostream& write(const symmTensor&, Ostream&) const;

    //- Write tensor. Tab separated ascii
    virtual Ostream& write(const tensor&, Ostream&) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "writer.cpp"
#endif

#endif
