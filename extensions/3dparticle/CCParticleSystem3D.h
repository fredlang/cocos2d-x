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

#ifndef __CC_PARTICLE_SYSTEM_3D_H__
#define __CC_PARTICLE_SYSTEM_3D_H__

#include "2d/CCNode.h"
#include "math/CCMath.h"
#include <vector>
#include <map>

NS_CC_BEGIN

/**
 * 3d particle system
 */
class Particle3DEmitter;
class Particle3DAffector;
class Particle3DRender;

struct Particle3D
{
    // property of particles, TODO add more properties
    Vec3 position;
    Vec4 color;
    
    float age;
    bool  alive;
    
    //user defined property
    std::map<std::string, void*> userDefs;
};

class ParticleSystem3D : public Node
{
public:
    ParticleSystem3D();
    virtual ~ParticleSystem3D();
    
    virtual void update(float delta) override;
    
    /**
     * particle system play control
     */
    void start();
    
    /**
     * stop particle
     */
    void stop();
    
    /**
     * pause particle
     */
    void pause();
    
    /**
     * resume particle
     */
    void resume();
    
    /**
     * set emitter for particle system, can set your own particle emitter
     */
    void setEmitter(Particle3DEmitter* emitter);
    /**
     * set particle render, can set your own particle render
     */
    void setRender(Particle3DRender* render);
    /**
     * add particle affector
     */
    void addAddAffector(Particle3DAffector* affector);
    
    /**
     * remove affector by index
     */
    void removeAffector(int index);
    
    /**
     * remove all particle affector
     */
    void removeAllAffector();
    
    /**
     * get particle affector by index
     */
    Particle3DAffector* getAffector(int index);
    
protected:
    enum class State
    {
        STOP,
        RUNNING,
        PAUSE,
    };
    State                            _state;
    Particle3DEmitter*               _emitter;
    std::vector<Particle3DAffector*> _affectors;
    Particle3DRender*                _render;
    
    //particles
    std::vector<Particle3D>          _particles;
    int                              _aliveParticlesCnt;
};

NS_CC_END

#endif
