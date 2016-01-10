// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directFieldMapper
// Description
//   FieldMapper with direct mapping.
#ifndef direct_field_mapper_hpp_
#define direct_field_mapper_hpp_
namespace mousse
{
class directFieldMapper
:
  public FieldMapper
{
  const labelUList& directAddressing_;
  bool hasUnmapped_;
public:
  // Constructors
    //- Construct given addressing
    patchFieldSubset(const labelUList& directAddressing)
    :
      directAddressing_(directAddressing),
      hasUnmapped_(false)
    {
      if (directAddressing_.size() && min(directAddressing_) < 0)
      {
        hasUnmapped_ = true;
      }
    }
  //- Destructor
  virtual ~directFieldMapper()
  {}
  // Member Functions
    label size() const
    {
      return directAddressing_.size();
    }
    bool direct() const
    {
      return true;
    }
    bool hasUnmapped() const
    {
      return hasUnmapped_;
    }
    const labelUList& directAddressing() const
    {
      return directAddressing_;
    }
};
}  // namespace mousse
#endif
