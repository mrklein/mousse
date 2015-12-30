// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coordinate_rotation.hpp"
#include "dictionary.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(coordinateRotation, 0);
  defineRunTimeSelectionTable(coordinateRotation, dictionary);
  defineRunTimeSelectionTable(coordinateRotation, objectRegistry);
}
// Member Functions 
mousse::symmTensor mousse::coordinateRotation::transformPrincipal
(
  const tensor& tt,
  const vector& st
) const
{
  return symmTensor
  (
    tt.xx()*st.x()*tt.xx()
   + tt.xy()*st.y()*tt.xy()
   + tt.xz()*st.z()*tt.xz(),
    tt.xx()*st.x()*tt.yx()
   + tt.xy()*st.y()*tt.yy()
   + tt.xz()*st.z()*tt.yz(),
    tt.xx()*st.x()*tt.zx()
   + tt.xy()*st.y()*tt.zy()
   + tt.xz()*st.z()*tt.zz(),
    tt.yx()*st.x()*tt.yx()
   + tt.yy()*st.y()*tt.yy()
   + tt.yz()*st.z()*tt.yz(),
    tt.yx()*st.x()*tt.zx()
   + tt.yy()*st.y()*tt.zy()
   + tt.yz()*st.z()*tt.zz(),
    tt.zx()*st.x()*tt.zx()
   + tt.zy()*st.y()*tt.zy()
   + tt.zz()*st.z()*tt.zz()
  );
}
