/******************************************************************************
 *  This file is part of Yune".
 *
 *  Copyright (C) 2018 by Umair Ahmed and Syed Moiz Hussain.
 *
 *  "Yune" is a framework for a Physically Based Renderer. It's aimed at young
 *  researchers trying to implement Physically Based Rendering techniques.
 *
 *  "Yune" is a free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Yune" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include <CL_headers.h>
#include <Eigen_typedefs.h>
#include <vector>
namespace yune
{
    /** \brief This class defines a Camera based on the standard right handed system approach used in OpenGL.
     *
     *  The View to World Matrix is based on the basis vectors of the Camera and thus contains the complete information of
     *  the orientation and the position. The Camera determines the it's view plane distance by varying values of Vertical Field of View.
     *  Greater values of Camera::y_FOV generate a smaller Camera::view_plane_dist and vice versa.
     *
     *  The Camera has a right handed coordinate system, and initially the side vector (right side of camera) represents the +X-axis, the Up vector
     *  represents the +Y-axis and the look at vector represents the -Z-axis. Note that we do not need to store the World to View matrix as
     *  Rays are already generated in Camera/View Space inside the kernel. Thus a View to World matrix is needed to transform them to World Space.
     *
     *  The class keeps an up-to-date copy of all the basis vectors as public members. Note that setting them directly doesn't change the matrix.
     *  These are only given for reading/checking the current values.
     */
    class Camera
    {
        public:
            Camera();   /**< Default Constructor. */

            /** \brief Overloaded Constructor.
             *
             * \param[in] y_FOV The vertical field of view in degrees.
             */
            Camera(float y_FOV, float rot_speed = 0.25f, float mov_speed = 0.3f);
            ~Camera();  /**< Default Destructor. */

            /** \brief Set the Orientation of the Camera when moved by mouse/keyboard.
             *
             * \param[in] dir A vec4 determining the direction vector in which the camera moves.
             * \param[in] pitch The angles in radian by which to rotate around the X-axis.
             * \param[in] yaw The angles in radian by which to rotate around the Y-axis.
             *
             */
            void setOrientation(const Vec4f& dir, float pitch, float yaw);

            /** \brief Set the View Matrix of the camera.
             *
             * \param[in] side A vec4 determining the Side basis vector of the Camera.
             * \param[in] up A vec4 determining the Up basis vector of the Camera.
             * \param[in] look_at A vec4 determining the LookAt basis vector of the Camera.
             * \param[in] eye A vec4 determining the position of the Camera.
             *
             */
            void setViewMatrix(const Vec4f& side, const Vec4f& up, const Vec4f& look_at, const Vec4f& eye);

            /** \brief Set values to be passed on to the Camera buffer.
             *
             * \param[in,out] cam_data The strucutre holding the camera data which is passed on to the OpenCL buffer object.
             *
             */
            void setBuffer(Cam* cam_data);

            /** \brief Set Rotation Speed of the camera.
             *
             * \param[in] rot_speed The rotation speed of the camera. Range between 0 and 1. Values closer to 0 give a more smooth and slower rotation.
             *
             */
            void setRotationSpeed(float rot_speed);

            /** \brief Set Movement Speed of the camera.
             *
             * \param[in] mov_speed The movement speed of the camera. Give a value between 0 and 1. Values closer to 0 give a more smooth and slower movement.
             *
             */
            void setMovementSpeed(float mov_speed);

            bool is_changed;                /**< A flag determining if the Camera changed orientation, hence if the buffer needs to be updated. */
            Vec4f side;                     /**< The Camera's Side vector. */
            Vec4f up;                       /**< The Camera's Up vector. */
            Vec4f look_at;                  /**< The Camera's Look At vector. */
            Vec4f eye;                      /**< The Camera's Eye/Position vector. */

        private:
            float view_plane_dist;          /**< The distance of the View Plane from the Camera; This is determined based on the vertical field of view. */
            float y_FOV;                    /**< The vertical Field of View in degrees. */
            float rotation_speed;           /**< The rotation speed of the camera. Range between 0 and 1. Values closer to 0 guarantess a more smooth and slower rotation. */
            float move_speed;               /**< The movement speed of the camera. Range between 0 and 1. Values closer to 0 guarantess a more smooth and slower movement. */
            Mat4x4f view_to_world_mat;      /**< A glm mat4x4 defining the View to World Matrix. Values in here aren't guaranteed to be normalized. */
    };
}
#endif // CAMERA_H
