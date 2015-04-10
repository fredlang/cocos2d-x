/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.
 
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

#ifndef __PHYSICS_3D_SHAPE_H__
#define __PHYSICS_3D_SHAPE_H__

#include "base/CCRef.h"
#include "math/CCMath.h"
#include "extensions/ExtensionMacros.h"
#include "extensions/ExtensionExport.h"


#if (CC_ENABLE_BULLET_INTEGRATION)

class btCollisionShape;

NS_CC_EXT_BEGIN

class CC_EX_DLL Physics3DShape : public Ref
{
public:
    enum class ShapeType
    {
        UNKNOWN = 0,
        BOX,
        SPHERE,
        CYLINDER,
        CAPSULE,
        CONVEX,
        MESH,
        HEIGHT_FIELD,
    };
    
    /**
     * get shape type
     */
    virtual ShapeType getShapeType() const;
    
    /**
     * create box shape
     * @param extent The extent of sphere.
     */
    static Physics3DShape* createBox(const cocos2d::Vec3& extent);
    
    /**
     * create sphere shape
     * @param radius The radius of sphere.
     */
    static Physics3DShape* createSphere(float radius);
    
    /**
     * create cylinder shape
     * @param radius The radius of cylinder.
     * @param height The height.
     */
    static Physics3DShape* createCylinder(float radius, float height);
    
    /**
     * create capsule shape
     * @param radius The radius of casule.
     * @param height The height (cylinder part).
     */
    static Physics3DShape* createCapsule(float radius, float height);

    /**
     * create convex hull
     * @param points The vertices of convex hull
     * @param numPoints The number of vertices.
     */
    static Physics3DShape* createConvexHull(const cocos2d::Vec3 *points, int numPoints);

    /**
     * create convex hull
     * @param triangles The pointer of triangle list
     * @param numTriangles The number of triangles.
     */
    static Physics3DShape* createMesh(const cocos2d::Vec3 *triangles, int numTriangles);


    
#if CC_ENABLE_BULLET_INTEGRATION
    btCollisionShape* getbtShape() const { return _btShape; }
#endif
    
protected:
    Physics3DShape();
    ~Physics3DShape();
    
    bool initBox(const cocos2d::Vec3& ext);
    bool initSphere(float radius);
    bool initCylinder(float radius, float height);
    bool initCapsule(float radius, float height);
    bool initConvexHull(const cocos2d::Vec3 *points, int numPoints);
    bool initMesh(const cocos2d::Vec3 *triangles, int numTriangles);
    
    
    ShapeType _shapeType; //shape type
    
#if (CC_ENABLE_BULLET_INTEGRATION)
    btCollisionShape*  _btShape;
#endif
};

NS_CC_EXT_END

#endif // CC_ENABLE_BULLET_INTEGRATION

#endif // __PHYSICS_3D_SHAPE_H__