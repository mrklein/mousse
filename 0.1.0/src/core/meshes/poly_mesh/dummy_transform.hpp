// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef dummy_transform_hpp_
#define dummy_transform_hpp_
namespace mousse
{
class dummyTransform
{
public:
  template<class T>
  void operator()
  (
    const vectorTensorTransform&,
    const bool /*forward*/,
    List<T>& /*fld*/
  ) const
  {};
  template<class T>
  void operator()(const coupledPolyPatch&, Field<T>&) const
  {}
  template<class T, template<class> class Container>
  void operator()(const coupledPolyPatch&, Container<T>& /*map*/) const
  {}
};
template<class T>
class pTraits<List<T> >
:
  public List<T>
{
public:
  typedef label cmptType;
  pTraits(Istream& is)
  :
    List<T>(is)
  {}
};
template<class T>
class pTraits<UList<T> >
:
  public UList<T>
{
public:
  typedef label cmptType;
  pTraits(Istream& is)
  :
    UList<T>(is)
  {}
};
template<class T>
class pTraits<Field<T> >
:
  public Field<T>
{
public:
  typedef label cmptType;
  pTraits(Istream& is)
  :
    Field<T>(is)
  {}
};
template<>
class pTraits<face>
:
  public face
{
public:
  typedef label cmptType;
  pTraits(Istream& is)
  :
    face(is)
  {}
};
}  // namespace mousse
#endif
