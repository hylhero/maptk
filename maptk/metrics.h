/*ckwg +29
 * Copyright 2014-2015 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 * \brief Header for evaluation metric functions.
 */

#ifndef MAPTK_METRICS_H_
#define MAPTK_METRICS_H_

#include <maptk/config.h>

#include "camera.h"
#include "landmark.h"
#include "feature.h"
#include "track.h"
#include <vector>
#include <map>
#include <cmath>

namespace maptk
{


/// Compute the reprojection error vector of lm projected by cam compared to f
/**
 * \param [in] cam is the camera used for projection
 * \param [in] lm is the landmark projected into the camera
 * \param [in] f is the measured feature point location
 * \returns the vector between the projected lm and f in image space
 */
MAPTK_LIB_EXPORT
vector_2d reprojection_error_vec(const camera& cam,
                                 const landmark& lm,
                                 const feature& f);


/// Compute the square reprojection error of lm projected by cam compared to f
/**
 * \param [in] cam is the camera used for projection
 * \param [in] lm is the landmark projected into the camera
 * \param [in] f is the measured feature point location
 * \returns the squared distance between the projected lm and f in image space
 */
inline
double
reprojection_error_sqr(const camera& cam,
                       const landmark& lm,
                       const feature& f)
{
  return reprojection_error_vec(cam, lm, f).squaredNorm();
}


/// Compute the reprojection error of lm projected by cam compared to f
/**
 * \param [in] cam is the camera used for projection
 * \param [in] lm is the landmark projected into the camera
 * \param [in] f is the measured feature point location
 * \returns the distance between the projected lm and f in image space
 */
inline
double
reprojection_error(const camera& cam,
                   const landmark& lm,
                   const feature& f)
{
  return std::sqrt(reprojection_error_sqr(cam, lm, f));
}


/// Compute the Root-Mean-Square-Error (RMSE) of the reprojections
/**
 * \param [in] cameras is the map of frames/cameras used for projection
 * \param [in] landmarks is the map ids/landmarks projected into the cameras
 * \param [in] tracks is the set of tracks providing measurements
 * \returns the RMSE between all landmarks projected by all cameras that have
 *          corresponding image measurements provided by the tracks
 */
MAPTK_LIB_EXPORT
double
reprojection_rmse(const std::map<frame_id_t, camera_sptr>& cameras,
                  const std::map<landmark_id_t, landmark_sptr>& landmarks,
                  const std::vector<track_sptr>& tracks);


} // end namespace maptk


#endif // MAPTK_METRICS_H_
