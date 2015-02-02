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

#include "3dparticle/ParticleUniverse/ParticleObservers/CCPUParticle3DOnPositionObserver.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"

NS_CC_BEGIN

// Constants
const Vec3 PUParticle3DOnPositionObserver::DEFAULT_POSITION_THRESHOLD(0, 0, 0);

//-----------------------------------------------------------------------
PUParticle3DOnPositionObserver::PUParticle3DOnPositionObserver(void) : 
    PUParticle3DObserver(),
    _positionXThreshold(DEFAULT_POSITION_THRESHOLD.x),
    _positionYThreshold(DEFAULT_POSITION_THRESHOLD.y),
    _positionZThreshold(DEFAULT_POSITION_THRESHOLD.z),
    _comparePositionX(CO_LESS_THAN),
    _comparePositionY(CO_LESS_THAN),
    _comparePositionZ(CO_LESS_THAN),
    _positionXThresholdSet(false),
    _positionYThresholdSet(false),
    _positionZThresholdSet(false)
{
}
//-----------------------------------------------------------------------
bool PUParticle3DOnPositionObserver::observe (PUParticle3D* particle, float timeElapsed)
{
    if (!particle)
        return false;

    // Added scale in V1.3.1
    if (_positionXThresholdSet && _comparePositionX == CO_LESS_THAN && particle->position.x < _positionXThreshold * _observerScale.x)
        return true;
    if (_positionXThresholdSet && _comparePositionX == CO_GREATER_THAN && particle->position.x > _positionXThreshold * _observerScale.x)
        return true;
    if (_positionXThresholdSet && _comparePositionX == CO_EQUALS && particle->position.x == _positionXThreshold * _observerScale.x)
        return true;

    if (_positionYThresholdSet && _comparePositionY == CO_LESS_THAN && particle->position.y < _positionYThreshold * _observerScale.y)
        return true;
    if (_positionYThresholdSet && _comparePositionY == CO_GREATER_THAN && particle->position.y > _positionYThreshold * _observerScale.y)
        return true;
    if (_positionYThresholdSet && _comparePositionY == CO_EQUALS && particle->position.y == _positionYThreshold * _observerScale.y)
        return true;

    if (_positionZThresholdSet && _comparePositionZ == CO_LESS_THAN && particle->position.z < _positionZThreshold * _observerScale.z)
        return true;
    if (_positionZThresholdSet && _comparePositionZ == CO_GREATER_THAN && particle->position.z > _positionZThreshold * _observerScale.z)
        return true;
    if (_positionZThresholdSet && _comparePositionZ == CO_EQUALS && particle->position.z == _positionZThreshold * _observerScale.z)
        return true;

    return false;
}

PUParticle3DOnPositionObserver* PUParticle3DOnPositionObserver::create()
{
    auto ppo = new PUParticle3DOnPositionObserver();
    ppo->autorelease();
    return ppo;
}

void PUParticle3DOnPositionObserver::copyAttributesTo( PUParticle3DObserver* observer )
{
    PUParticle3DObserver::copyAttributesTo(observer);

    PUParticle3DOnPositionObserver* onPositionObserver = static_cast<PUParticle3DOnPositionObserver*>(observer);
    onPositionObserver->_positionXThreshold = _positionXThreshold;
    onPositionObserver->_positionYThreshold = _positionYThreshold;
    onPositionObserver->_positionZThreshold = _positionZThreshold;
    onPositionObserver->_comparePositionX = _comparePositionX;
    onPositionObserver->_comparePositionY = _comparePositionY;
    onPositionObserver->_comparePositionZ = _comparePositionZ;
    onPositionObserver->_positionXThresholdSet = _positionXThresholdSet;
    onPositionObserver->_positionYThresholdSet = _positionYThresholdSet;
    onPositionObserver->_positionZThresholdSet = _positionZThresholdSet;
}

NS_CC_END
