/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CC_OBB_H_
#define __CC_OBB_H_

#include "CCAABB.h"

NS_CC_BEGIN
    
/**
 * Defines a class to describe oriented bounding box
 */
class OBB
{
public:
    OBB();
    OBB(const OBB& obb);

    // is point in this obb
    bool isPointIn(const Vec3& point) const;

    // clear obb
    void clear();

    // build obb from oriented bounding box
    void build(const AABB& aabb);

    // build obb from points
    void build(const Vec3* verts, int nVerts);

    // face to the obb's -z direction
    // verts[0] : front left bottom corner
    // verts[1] : front right bottom corner
    // verts[2] : front right top corner
    // verts[3] : front left top corner
    // verts[4] : back left bottom corner
    // verts[5] : back right bottom corner
    // verts[6] : back right top corner
    // verts[7] : back left top corner
    void getVertices(Vec3* verts) const;

    // compute extX, extY, extZ
    // if obb axis changed call this function before use ext axis
    void completeExtAxis()
    {
        extX = xAxis * extents.x;
        extY = yAxis * extents.y;
        extZ = zAxis * extents.z;
    }

	/**
     * Transforms the obb by the given transformation matrix.
     */
    void transform(const Mat4& mat);

public:

    Vec3 center; // obb center

    Vec3 xAxis; // x axis of obb, unit vector
    Vec3 yAxis; // y axis of obb, unit vecotr
    Vec3 zAxis; // z axis of obb, unit vector

    Vec3 extX; // xAxis * extents.x
    Vec3 extY; // yAxis * extents.y
    Vec3 extZ; // zAxis * extents.z

    Vec3 extents; // obb length along each axis
};

NS_CC_END

#endif
