// Copyright (C) 2007 Anders Logg and Garth N. Wells.
// Licensed under the GNU GPL Version 2.

// First added:  2007-03-01
// Last changed: 2007-03-01

#include <dolfin/constants.h>
#include <dolfin/Mesh.h>
#include <dolfin/UFCCell.h>
#include <dolfin/DofMap.h>

using namespace dolfin;

//-----------------------------------------------------------------------------
DofMap::DofMap(const ufc::dof_map& dof_map, Mesh& mesh) : ufc_dof_map(dof_map)
{
  // Initialize mesh entities used by dof map
  for (uint d = 0; d <= mesh.topology().dim(); d++)
  {
    if ( ufc_dof_map.needs_mesh_entities(d) )
      mesh.init(d);
  }
  
  // Initialize UFC mesh data (must be done after entities are created)
  ufc_mesh.init(mesh);

  // Initialize UFC dof map
  UFCMesh ufc_mesh(mesh);
  //bool init_cells = ufc_dof_map.init_mesh(ufc_mesh);
  //if ( init_cells )
  //{
  //}
 }
 //-----------------------------------------------------------------------------
 DofMap::~DofMap()
 {
  // Delete UFC mesh data
  delete [] ufc_mesh.num_entities;
}
//-----------------------------------------------------------------------------

