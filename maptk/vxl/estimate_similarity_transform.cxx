/*ckwg +5
 * Copyright 2014 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

/**
 * \file
 * \brief Implementation of VXL similarity transform estimation
 */

#include "estimate_similarity_transform.h"
#include <maptk/core/exceptions/algorithm.h>
#include <maptk/core/rotation.h>

#include <iostream>
#include <sstream>

#include <vcl_vector.h>
//#include <vgl/algo/vgl_compute_similarity_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vpgl/algo/vpgl_ortho_procrustes.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_quaternion.h>
#include <vnl/vnl_vector_fixed.h>


namespace maptk
{

namespace vxl
{


/// Estimate the similarity transform between two corresponding point sets
similarity_d
estimate_similarity_transform
::estimate_transform(std::vector<vector_3d> const& from,
                     std::vector<vector_3d> const& to) const
{
  if (from.size() != to.size())
  {
    std::ostringstream sstr;
    sstr << "from and to sets for similarity estimation are not of equivalent "
         << "size! (from: " << from.size() << ", to: " << to.size() << ")";
    throw algorithm_exception(this->type_name(), this->impl_name(),
                              sstr.str());
  }
  // TODO: Determine why there is a deficiency with 3 correspondences
  else if (from.size() < 4)
  {
    std::ostringstream sstr;
    sstr << "At least 4 or more point pairs must be give in order to estimate "
         << "the similarity transformation. Given: " << from.size();
    throw algorithm_exception(this->type_name(), this->impl_name(), sstr.str());
  }

  // TODO: Test for collinearity
  // a <- (from[0], to[0])
  // b <- (from[1], to[1])
  // e <- some epsillon value
  // collinear <- true
  // for c in [(from[2], to[2]), ... , (from[n-1], to[n-1])] {
  //     if cross((b-a), (c-a)).magnitude > e {
  //        collinear <- false
  //        break
  //     }
  // }
  // if collinear {
  //    raise exception
  // }

  // Convert given point correspondences into corresponding matrices of size
  // 3xN. Already checked for size congruency above.
  vnl_matrix<double> from_mat(3, from.size()),
                     to_mat(3, to.size());
  // using the same loop for both vectors as they are the same size.
  for (unsigned i = 0; i < from.size(); ++i)
  {
    from_mat(0,i) = from[i].x();
    from_mat(1,i) = from[i].y();
    from_mat(2,i) = from[i].z();
    to_mat(0,i) = to[i].x();
    to_mat(1,i) = to[i].y();
    to_mat(2,i) = to[i].z();
  }

  vpgl_ortho_procrustes op(from_mat, to_mat);
  if (!op.compute_ok())
  {
    // TODO: Do some exception handling here
    std::cerr << "ERROR: Invalid vpgl_ortho_procrustes construction" << std::endl;
    return similarity_d();
  }

  // Computation happend when a result property is requested
  vnl_quaternion<double> const& v_quat = op.R().as_quaternion();
  vnl_vector_fixed<double, 3> const& v_trans = op.t();

  if (!op.compute_ok())
  {
    // TODO: Do some exception handling here.
    std::cerr << "ERROR: vpgl_ortho_procrustes failed computation" << std::endl;
    return similarity_d();
  }

  rotation_d const m_rot(vector_4d(v_quat.x(), v_quat.y(), v_quat.z(), v_quat.r()));
  vector_3d const m_trans(v_trans[0], v_trans[1], v_trans[2]);

  return similarity_d(op.s(), m_rot, m_trans);
}


} // end namespace vxl

} // end namespace maptk
