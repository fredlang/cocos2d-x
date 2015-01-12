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

#include "3dparticle/ParticleUniverse/ParticleEventHandlers/CCPUParticle3DDoExpireEventHandler.h"
#include "3dparticle/ParticleUniverse/ParticleAffectors/CCPUParticle3DAffector.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"

NS_CC_BEGIN

//-----------------------------------------------------------------------
void PUParticle3DDoExpireEventHandler::handle (PUParticleSystem3D* particleSystem, PUParticle3D* particle, float timeElapsed)
{
	if (!particle)
		return;

	particle->timeToLive = 0.0f;
}

PUParticle3DDoExpireEventHandler* PUParticle3DDoExpireEventHandler::create()
{
	auto peh = new PUParticle3DDoExpireEventHandler();
	peh->autorelease();
	return peh;
}

NS_CC_END
