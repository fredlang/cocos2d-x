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

#include "3dparticle/ParticleUniverse/ParticleEventHandlers/CCPUParticle3DDoPlacementParticleEventHandler.h"
#include "3dparticle/ParticleUniverse/ParticleAffectors/CCPUParticle3DAffector.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"

NS_CC_BEGIN

// Constants
const unsigned int PUParticle3DDoPlacementParticleEventHandler::DEFAULT_NUMBER_OF_PARTICLES = 1;

//-----------------------------------------------------------------------
PUParticle3DDoPlacementParticleEventHandler::PUParticle3DDoPlacementParticleEventHandler(void) : 
	PUParticle3DEventHandler(),
	PUParticle3DListener(),
	_numberOfParticles(DEFAULT_NUMBER_OF_PARTICLES),
	_found(false),
	_alwaysUsePosition(true),
	_emitter(0),
	_system(0),
	_inheritPosition(true),
	_inheritDirection(false),
	_inheritOrientation(false),
	_inheritTimeToLive(false),
	_inheritMass(false),
	_inheritTextureCoordinate(false),
	_inheritColour(false),
	_inheritParticleWidth(false),
	_inheritParticleHeight(false),
	_inheritParticleDepth(false),
	_baseParticle(0)
{
}
//-----------------------------------------------------------------------
PUParticle3DDoPlacementParticleEventHandler::~PUParticle3DDoPlacementParticleEventHandler(void)
{
	// We cannot remove this listener from mTechnique, because it is undetermined whether the ParticleTechnique 
	// still exist.
}
//-----------------------------------------------------------------------
void PUParticle3DDoPlacementParticleEventHandler::handle (PUParticleSystem3D* particleSystem, PUParticle3D* particle, float timeElapsed)
{
	if (!particle)
		return;

	if (!_found)
	{
		auto system = particleSystem;
		auto emitter = system->getEmitter(_forceEmitterName);
		//ParticleTechnique* technique = particleTechnique;
		//ParticleEmitter* emitter = particleTechnique->getEmitter(_forceEmitterName);
		if (!emitter)
		{
			// Search all techniques in this ParticleSystem for an emitter with the correct name
			PUParticleSystem3D* parentSystem = dynamic_cast<PUParticleSystem3D *>(system->getParent());
			if (parentSystem){
				auto children = parentSystem->getChildren();
				for(auto iter : children)		
				{
					PUParticleSystem3D *child  = dynamic_cast<PUParticleSystem3D *>(iter);
					if (child){
						system = child;
						emitter = system->getEmitter(_forceEmitterName);
						if (emitter)
						{
							break;
						}
					}
				}
			}
		}
		if (emitter)
		{
			_system = system;
			_emitter = emitter;
			if (_system)
			{
				_system->addListener(this);
			}
			_found = true;
		}
		else
		{
			return;
		}
	}

	// Emit 1 or more particles
	if (_system)
	{
		_baseParticle = particle;
		_system->forceEmission(_emitter, _numberOfParticles);
	}

	_baseParticle = 0;
}
//-----------------------------------------------------------------------
void PUParticle3DDoPlacementParticleEventHandler::particleEmitted(PUParticleSystem3D* particleSystem, PUParticle3D* particle)
{
	if (!_baseParticle)
		return;

	if (particle && _emitter == particle->parentEmitter)
	{
		if (_inheritPosition)
		{
#ifdef PU_PHYSICS
			// Do not assume that the contact point is always used if a physics engine is used.
			if (!_alwaysUsePosition && particle->physicsActor)
			{
				particle->position = _baseParticle->physicsActor->contactPoint; // Store the contact point to spawn new particles on that position.
			}
			else
			{
				particle->position = _baseParticle->position; // Store the particles' position to spawn new particles on that position.
			}
#else
			particle->position = _baseParticle->position; // Store the particles' position to spawn new particles on that position.
#endif // PU_PHYSICS
			particle->originalPosition = particle->position;
		}
		if (_inheritDirection)
		{
			particle->direction = _baseParticle->direction;
			particle->originalDirection = particle->direction;
			particle->originalDirectionLength = _baseParticle->originalDirectionLength;
			particle->originalScaledDirectionLength = _baseParticle->originalScaledDirectionLength;
			particle->originalVelocity = _baseParticle->originalVelocity;
		}
		if (_inheritOrientation)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->orientation = _baseParticle->orientation;
				particle->originalOrientation = _baseParticle->originalOrientation;
			}
		}
		if (_inheritTimeToLive)
		{
			particle->timeToLive = _baseParticle->timeToLive;
			particle->totalTimeToLive = _baseParticle->totalTimeToLive;
			particle->timeFraction = _baseParticle->timeFraction;
		}
		if (_inheritMass)
		{
			particle->mass = _baseParticle->mass;
		}
		if (_inheritTextureCoordinate)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->textureAnimationTimeStep = _baseParticle->textureAnimationTimeStep;
				particle->textureAnimationTimeStepCount = _baseParticle->textureAnimationTimeStepCount;
				particle->textureCoordsCurrent = _baseParticle->textureCoordsCurrent;
				particle->textureAnimationDirectionUp = _baseParticle->textureAnimationDirectionUp;
			}
		}
		if (_inheritColour)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->color = _baseParticle->color;
				particle->originalColor = _baseParticle->originalColor;
			}
		}
		if (_inheritParticleWidth)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->setOwnDimensions(_baseParticle->width, particle->height, particle->depth);
			}
		}
		if (_inheritParticleHeight)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->setOwnDimensions(particle->width, _baseParticle->height, particle->depth);
			}
		}
		if (_inheritParticleDepth)
		{
			//if (_baseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
			{
				//VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(_baseParticle);
				//VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				particle->setOwnDimensions(particle->width, particle->height, _baseParticle->depth);
			}
		}
	}
}
//-----------------------------------------------------------------------
void PUParticle3DDoPlacementParticleEventHandler::setForceEmitterName(const std::string& forceEmitterName)
{
	_forceEmitterName = forceEmitterName;
}
//-----------------------------------------------------------------------
PUParticle3DEmitter* PUParticle3DDoPlacementParticleEventHandler::getForceEmitter(void) const
{
	return _emitter;
}
//-----------------------------------------------------------------------
void PUParticle3DDoPlacementParticleEventHandler::removeAsListener(void)
{
	// Reset some values and remove this as a listener from the old technique.
	if (_system)
	{
		_system->removeListener(this);
	}
	_found = false;
	_emitter = 0;
	_system = 0;
}

PUParticle3DDoPlacementParticleEventHandler* PUParticle3DDoPlacementParticleEventHandler::create()
{
	auto peh = new PUParticle3DDoPlacementParticleEventHandler();
	peh->autorelease();
	return peh;
}

NS_CC_END
