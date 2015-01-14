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

#ifndef __CC_PU_PARTICLE_3D_SLAVE_BEHAVIOUR_H__
#define __CC_PU_PARTICLE_3D_SLAVE_BEHAVIOUR_H__

#include "base/CCRef.h"
#include "math/CCMath.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"
#include "3dparticle/ParticleUniverse/ParticleBehaviours/CCPUParticle3DBehaviour.h"
#include <vector>
#include <string>

NS_CC_BEGIN

struct PUParticle3D;
class PUParticleSystem3D;

class CC_DLL PUParticle3DSlaveBehaviour : public PUParticle3DBehaviour
{
public:

	static PUParticle3DSlaveBehaviour* create();

	PUParticle3D* masterParticle;

	/** See ParticleBehaviour.
	*/
	virtual void updateBehaviour(PUParticle3D *particle, float deltaTime);

	virtual PUParticle3DSlaveBehaviour* clone() const override;

CC_CONSTRUCTOR_ACCESS:
	PUParticle3DSlaveBehaviour();
	virtual ~PUParticle3DSlaveBehaviour();
};

NS_CC_END

#endif
