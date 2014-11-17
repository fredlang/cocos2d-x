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

#include "CCParticleSystem3D.h"
#include "CCParticle3DEmitter.h"
#include "CCParticle3DAffector.h"

NS_CC_BEGIN

ParticleSystem3D::ParticleSystem3D()
: _emitter(nullptr)
, _render(nullptr)
, _aliveParticlesCnt(0)
, _state(State::RUNNING)
{
    
}
ParticleSystem3D::~ParticleSystem3D()
{
    
}

void ParticleSystem3D::start()
{
    
}

void ParticleSystem3D::stop()
{
    
}

void ParticleSystem3D::pause()
{
    
}

void ParticleSystem3D::resume()
{
    
}

void ParticleSystem3D::setEmitter(Particle3DEmitter* emitter)
{
    
}

void ParticleSystem3D::setRender(Particle3DRender* render)
{
    
}

void ParticleSystem3D::addAddAffector(Particle3DAffector* affector)
{
    
}

void ParticleSystem3D::removeAffector(int index)
{
    
}

void ParticleSystem3D::removeAllAffector()
{
    //release all affectors
    _affectors.clear();
}

Particle3DAffector* ParticleSystem3D::getAffector(int index)
{
    CCASSERT(index < _affectors.size(), "wrong index");
    return _affectors[index];
}

void ParticleSystem3D::update(float delta)
{
    if (_state != State::RUNNING)
        return;
    
    if (_emitter)
        _emitter->updateEmitter(delta);
    
    for (auto& it : _affectors) {
        it->updateAffector(delta);
    }
        
}

NS_CC_END
