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

#ifndef __CC_PARTICLE_3D_RENDER_H__
#define __CC_PARTICLE_3D_RENDER_H__

#include "base/CCRef.h"
#include "math/CCMath.h"
#include <vector>

NS_CC_BEGIN

class ParticleSystem3D;
class Renderer;
class MeshCommand;
class Sprite3D;
class GLProgramState;
class IndexBuffer;
class VertexBuffer;
class Texture2D;

/**
 * 3d particle render
 */
class CC_DLL Particle3DRender : public Ref
{
    friend class ParticleSystem3D;
public:

    virtual void render(Renderer* renderer, const Mat4 &transform, ParticleSystem3D* particleSystem) = 0;

    /** Perform activities when a Renderer is started.
    */
    virtual void notifyStart(void);
    /** Perform activities when a Renderer is stopped.
    */
    virtual void notifyStop(void);
    /** Notify that the Particle System is rescaled.
    */
    virtual void notifyRescaled(const Vec3& scale);
    
    void setVisible(bool isVisible) { _isVisible = isVisible; }
    
    bool isVisible() const { return _isVisible; }

    virtual void setDepthTest(bool isDepthTest) { _depthTest = isDepthTest; }
    virtual void setDepthWrite(bool isDepthWrite) {_depthWrite = isDepthWrite; }
    
CC_CONSTRUCTOR_ACCESS:
    Particle3DRender()
        : _particleSystem(nullptr)
        , _isVisible(true)
        , _rendererScale(Vec3::ONE)
        , _depthTest(true)
        , _depthWrite(false)
    {};
    virtual ~Particle3DRender(){};
    
protected:
    ParticleSystem3D *_particleSystem;
    bool  _isVisible;
    Vec3 _rendererScale;
    bool _depthTest;
    bool _depthWrite;
};

// particle render for quad
class CC_DLL Particle3DQuadRender : public Particle3DRender
{
public:
    static Particle3DQuadRender* create(const std::string& texFile = "");
    
    virtual void render(Renderer* renderer, const Mat4 &transform, ParticleSystem3D* particleSystem) override;

    virtual void setDepthTest(bool isDepthTest);
    virtual void setDepthWrite(bool isDepthWrite);
    
CC_CONSTRUCTOR_ACCESS:
    Particle3DQuadRender();
    virtual ~Particle3DQuadRender();

protected:

    void initQuadRender(const std::string& texFile);
    
protected:
    MeshCommand* _meshCommand;
    Texture2D*             _texture;
    GLProgramState*        _glProgramState;
    IndexBuffer*           _indexBuffer; //index buffer
    VertexBuffer*          _vertexBuffer; // vertex buffer
    
    struct posuvcolor
    {
        Vec3 position;
        Vec2 uv;
        Vec4 color;
    };

    std::vector<posuvcolor> _posuvcolors;   //vertex data
    std::vector<unsigned short> _indexData; //index data
};

// particle render for Sprite3D
class CC_DLL Particle3DModelRender : public Particle3DRender
{
public:
    static Particle3DModelRender* create(const std::string& modelFile, const std::string &texFile = "");

    virtual void render(Renderer* renderer, const Mat4 &transform, ParticleSystem3D* particleSystem) override;
    
CC_CONSTRUCTOR_ACCESS:
    Particle3DModelRender();
    virtual ~Particle3DModelRender();
    
protected:
    std::vector<Sprite3D *> _spriteList;
    std::string _modelFile;
    std::string _texFile;
    Vec3 _spriteSize;
};

NS_CC_END

#endif
