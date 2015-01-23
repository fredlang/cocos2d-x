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

#include "3dparticle/CCParticleSystem3D.h"
#include "3dparticle/ParticleUniverse/ParticleRenders/CCPUParticle3DRibbonTrailRender.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DUtil.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DSimpleSpline.h"
#include "renderer/CCMeshCommand.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCVertexIndexBuffer.h"
#include "base/CCDirector.h"
#include "3d/CCSprite3D.h"
#include "3d/CCMesh.h"
#include "2d/CCCamera.h"
#include <sstream>

NS_CC_BEGIN

// Constants
const bool PUParticle3DRibbonTrailRender::DEFAULT_USE_VERTEX_COLOURS = true;
const size_t PUParticle3DRibbonTrailRender::DEFAULT_MAX_ELEMENTS = 10;
const float PUParticle3DRibbonTrailRender::DEFAULT_LENGTH = 400;
const float PUParticle3DRibbonTrailRender::DEFAULT_WIDTH = 5;
const bool PUParticle3DRibbonTrailRender::DEFAULT_RANDOM_INITIAL_COLOUR = true;
const Vec4 PUParticle3DRibbonTrailRender::DEFAULT_INITIAL_COLOUR(1, 1, 1, 1);
const Vec4 PUParticle3DRibbonTrailRender::DEFAULT_COLOUR_CHANGE(0.5, 0.5, 0.5, 0.5);

PUParticle3DRibbonTrailRender* PUParticle3DRibbonTrailRender::create( const std::string &texFile )
{
	auto br = new PUParticle3DRibbonTrailRender();
	br->autorelease();
	br->_texFile = texFile;
	return br;
}

void PUParticle3DRibbonTrailRender::render( Renderer* renderer, const Mat4 &transform, ParticleSystem3D* particleSystem )
{
	const ParticlePool &particlePool = particleSystem->getParticlePool();
	if (!_isVisible || particlePool.empty() || !_trail)
		return;

	PUParticle3DRibbonTrailVisualData* visualData = nullptr;
	Vec3 basePosition = static_cast<PUParticleSystem3D *>(_particleSystem)->getDerivedPosition();
	for (auto iter : particlePool.getActiveParticleList())
	{
		auto particle = static_cast<PUParticle3D *>(iter);
		if (!particle->visualData && !_visualData.empty())
		{
			particle->visualData = _visualData.back();
			_visualData.pop_back();
		}
		visualData = static_cast<PUParticle3DRibbonTrailVisualData*>(particle->visualData);
		if (visualData)
		{
			Node* node = visualData->node;
			node->setPosition3D(particle->positionInWorld);

			// Set the width of the trail if required
			if (particle->particleType == PUParticle3D::PT_VISUAL)
			{
				if (particle->ownDimensions)
				{
					_trail->setInitialWidth(visualData->index, particle->width);
				}
			}
			visualData->setVisible(true);
		}
	}

	_trail->render(renderer, transform, particleSystem);
}

PUParticle3DRibbonTrailRender::PUParticle3DRibbonTrailRender() : 
	_quota(0),
	_trail(0),
	_useVertexColours(DEFAULT_USE_VERTEX_COLOURS),
	_maxChainElements(DEFAULT_MAX_ELEMENTS),
	_setLength(false),
	_trailLength(DEFAULT_LENGTH),
	_setWidth(false),
	_trailWidth(DEFAULT_WIDTH),
	_randomInitialColour(DEFAULT_RANDOM_INITIAL_COLOUR),
	_initialColour(DEFAULT_INITIAL_COLOUR),
	_colorChange(DEFAULT_COLOUR_CHANGE),
	_childNode(0)
{
	std::stringstream ss; 
	ss << this;
	_ribbonTrailName = "RibbonTrail" + ss.str();
}

PUParticle3DRibbonTrailRender::~PUParticle3DRibbonTrailRender()
{
	if (!_particleSystem)
		return;

	destroyAll();
}

void PUParticle3DRibbonTrailRender::particleEmitted( PUParticleSystem3D* particleSystem, PUParticle3D* particle )
{
	// Assigning visual data already done in _updateRenderQueue()
}

void PUParticle3DRibbonTrailRender::particleExpired( PUParticleSystem3D* particleSystem, PUParticle3D* particle )
{
	if (particle->visualData)
	{
		PUParticle3DRibbonTrailVisualData* ribbonTrailRendererVisualData = static_cast<PUParticle3DRibbonTrailVisualData*>(particle->visualData);
		ribbonTrailRendererVisualData->setVisible(false);
	}
}

//-----------------------------------------------------------------------
bool PUParticle3DRibbonTrailRender::isUseVertexColors(void) const
{
	return _useVertexColours;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setUseVertexColors(bool useVertexColours)
{
	_useVertexColours = useVertexColours;
} 
//-----------------------------------------------------------------------
size_t PUParticle3DRibbonTrailRender::getMaxChainElements(void) const
{
	return _maxChainElements;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setMaxChainElements(size_t maxChainElements)
{
	_maxChainElements = maxChainElements;
} 
//-----------------------------------------------------------------------
float PUParticle3DRibbonTrailRender::getTrailLength(void) const
{
	return _trailLength;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setTrailLength(float trailLength)
{
	_trailLength = trailLength;
	_setLength = true;
} 
//-----------------------------------------------------------------------
float PUParticle3DRibbonTrailRender::getTrailWidth(void) const
{
	return _trailWidth;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setTrailWidth(float trailWidth)
{
	_trailWidth = trailWidth;
	_setWidth = true;
} 
//-----------------------------------------------------------------------
bool PUParticle3DRibbonTrailRender::isRandomInitialColor(void) const
{
	return _randomInitialColour;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setRandomInitialColor(bool randomInitialColour)
{
	_randomInitialColour = randomInitialColour;
} 
//-----------------------------------------------------------------------
const Vec4& PUParticle3DRibbonTrailRender::getInitialColor(void) const
{
	return _initialColour;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setInitialColor(const Vec4& initialColour)
{
	_initialColour = initialColour;
} 
//-----------------------------------------------------------------------
const Vec4& PUParticle3DRibbonTrailRender::getColorChange(void) const
{
	return _colorChange;
} 
//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::setColorChange(const Vec4& colourChange)
{
	_colorChange = colourChange;
} 
//-----------------------------------------------------------------------

void PUParticle3DRibbonTrailRender::prepare()
{
	if (!_particleSystem)
		return;

	// Register itself to the technique
	if (_particleSystem)
	{
		// Although it is safe to assume that technique == mParentTechnique, use the mParentTechnique, because the mParentTechnique is
		// also used for unregistering.
		static_cast<PUParticleSystem3D *>(_particleSystem)->addListener(this);
	}

	_quota = _particleSystem->getParticleQuota();
	if (!_childNode)
	{
		// Create a childnode if not created earlier
		std::stringstream ss; 
		ss << this;
		std::string childNodeNodeName = "ParticleUniverse" + ss.str();
		//_childNode = mParentTechnique->getParentSystem()->getParentSceneNode()->createChildSceneNode(childNodeNodeName);
		//_childNode->setInheritOrientation(false);
		auto parent = _particleSystem->getParent()->getParent();
		if (parent)
		{
			_childNode = Node::create();
			parent->addChild(_childNode);
		}
	}

	if (_childNode)
	{
		// Create RibbonTrail
		//Ogre::SceneManager* sceneManager = mParentTechnique->getParentSystem()->getSceneManager();
		_trail = new PURibbonTrail(_ribbonTrailName, _texFile);
		_trail->setNumberOfChains(_quota);
		_trail->setMaxChainElements(_maxChainElements);
		if (_setLength)
		{
			_trail->setTrailLength(_rendererScale.y * _trailLength);
		}
		else
		{
			_trail->setTrailLength(_rendererScale.y * static_cast<PUParticleSystem3D *>(_particleSystem)->getDefaultHeight());
		}
		_trail->setUseVertexColours(_useVertexColours);

		// Create number of VisualData objects
		std::string sceneNodeName;
		std::stringstream ss; 
		ss << this;
		for (size_t i = 0; i < _quota; i++)
		{
			//sceneNodeName = "ParticleUniverse" + ss.str() + StringConverter::toString(i);
			//RibbonTrailRendererVisualData* visualData = 
			//	PU_NEW_T(RibbonTrailRendererVisualData, MEMCATEGORY_SCENE_OBJECTS)(_childNode->createChildSceneNode(sceneNodeName), _trail);
			//visualData->node->setInheritOrientation(false);
			auto node = Node::create();
			_childNode->addChild(node);
			PUParticle3DRibbonTrailVisualData* visualData = new PUParticle3DRibbonTrailVisualData(node, _trail);
			visualData->index = i;
			_allVisualData.push_back(visualData); // Managed by this renderer
			_visualData.push_back(visualData); // Used to assign to a particle
			if (_randomInitialColour)
			{
				_trail->setInitialColour(i, CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
			}
			else
			{
				_trail->setInitialColour(i, _initialColour);
			}
			_trail->setColourChange(i, _colorChange);
			if (_setWidth)
			{
				_trail->setInitialWidth(i, _rendererScale.x * _trailWidth);
			}
			else
			{
				_trail->setInitialWidth(i, _rendererScale.x * static_cast<PUParticleSystem3D *>(_particleSystem)->getDefaultWidth());
			}
		}
		//_childNode->attachObject(_trail);
		_trail->setAttachedNode(_childNode);
	}
}

void PUParticle3DRibbonTrailRender::unPrepare()
{
	destroyAll();
}

void PUParticle3DRibbonTrailRender::updateRender( PUParticle3D *particle, float deltaTime, bool firstParticle )
{
	if (_trail)
		_trail->update(deltaTime);
}

//-----------------------------------------------------------------------
void PUParticle3DRibbonTrailRender::destroyAll(void)
{
	if (!_particleSystem || !_trail || !_childNode)
		return;

	// Remove the listener
	static_cast<PUParticleSystem3D *>(_particleSystem)->removeListener(this);

	// Detach the Ribbontrail
	if (_childNode && _trail)
	{
		_trail->setAttachedNode(nullptr);
		//_childNode->detachObject(_trail);
	}

	// Delete the Ribbontrail
	CC_SAFE_DELETE(_trail);

	// Delete the visual data
	std::vector<PUParticle3DRibbonTrailVisualData*>::const_iterator it;
	std::vector<PUParticle3DRibbonTrailVisualData*>::const_iterator itEnd = _allVisualData.end();
	for (it = _allVisualData.begin(); it != itEnd; ++it)
	{
		delete *it;
	}

	_allVisualData.clear();
	_visualData.clear();

	// Destroy the children of the childnode, but not the childnode itself, because this gives a delete-order problem
	if (_childNode)
	{
		//_childNode->removeAndDestroyAllChildren();
		_childNode->removeAllChildren();
		if (_childNode->getParent())
			_childNode->getParent()->removeChild(_childNode);
		_childNode = nullptr;
	}
}


void PUParticle3DRibbonTrailRender::notifyRescaled( const Vec3& scale )
{
	PUParticle3DRender::notifyRescaled(scale);
	if (_trail)
	{
		// Use the average length (is there a good alternative?
		_trail->setTrailLength(scale.y * _trailLength);
		size_t numberOfChains = _trail->getNumberOfChains();
		for (size_t i = 0; i < numberOfChains; ++i)
		{
			_trail->setInitialWidth(i, scale.x * _trailWidth);
		}
	}
}

NS_CC_END
