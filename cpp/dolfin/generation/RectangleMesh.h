// Copyright (C) 2005-2017 Anders Logg and Garth N. Wells
//
// This file is part of DOLFIN (https://www.fenicsproject.org)
//
// SPDX-License-Identifier:    LGPL-3.0-or-later

#pragma once

#include <array>
#include <dolfin/common/MPI.h>
#include <dolfin/mesh/Mesh.h>
#include <string>

namespace dolfin
{

namespace generation
{

/// Triangular mesh of the 2D rectangle spanned by two points p0 and p1.
/// Given the number of cells (nx, ny) in each direction, the total
/// number of triangles will be 2*nx*ny and the total number of vertices
/// will be (nx + 1)*(ny + 1).

class RectangleMesh
{
public:
  /// @param[in] comm MPI communicator to build the mesh on
  /// @param[in] p Two corner points
  /// @param[in] n Number of cells in each direction
  /// @param[in] cell_type The mesh cell type
  /// @param[in] ghost_mode Mesh ghosting mode
  /// @param[in] diagonal Direction of diagonals: "left", "right",
  ///                     "left/right", "crossed"
  /// @return Mesh
  static mesh::Mesh
    create(MPI_Comm comm, const std::array<Eigen::Vector3d, 2>& p,
         std::array<std::size_t, 2> n, mesh::CellType cell_type,
         const mesh::GhostMode ghost_mode, std::string diagonal = "right");
};
} // namespace generation
} // namespace dolfin
