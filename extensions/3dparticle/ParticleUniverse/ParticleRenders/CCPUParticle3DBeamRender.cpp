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
#include "3dparticle/ParticleUniverse/ParticleRenders/CCPUParticle3DBeamRender.h"
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
const bool PUParticle3DBeamRender::DEFAULT_USE_VERTEX_COLOURS = false;
const size_t PUParticle3DBeamRender::DEFAULT_MAX_ELEMENTS = 10;
const float PUParticle3DBeamRender::DEFAULT_UPDATE_INTERVAL = 0.1f;
const float PUParticle3DBeamRender::DEFAULT_DEVIATION = 300;
const size_t PUParticle3DBeamRender::DEFAULT_NUMBER_OF_SEGMENTS = 2;
const PUBillboardChain::TexCoordDirection PUParticle3DBeamRender::DEFAULT_TEXTURE_DIRECTION = PUBillboardChain::TCD_V;

PUParticle3DBeamRender* PUParticle3DBeamRender::create( const std::string &texFile )
{
	auto br = new PUParticle3DBeamRender();
	br->autorelease();
	br->_texFile = texFile;
	return br;
}

void PUParticle3DBeamRender::render( Renderer* renderer, const Mat4 &transform, ParticleSystem3D* particleSystem )
{
	const ParticlePool &particlePool = particleSystem->getParticlePool();
	if (!_isVisible || particlePool.empty() || !_billboardChain)
		return;

	PUParticle3DBeamVisualData* visualData = nullptr;
	Vec3 basePosition = static_cast<PUParticleSystem3D *>(_particleSystem)->getDerivedPosition();
	for (auto iter : particlePool.getActiveParticleList())
	{
		auto particle = static_cast<PUParticle3D *>(iter);
		auto visualData = static_cast<PUParticle3DBeamVisualData*>(particle->visualData);
		if (visualData){
			Vec3 end = particle->positionInWorld - basePosition;
			PUSimpleSpline spline;

			// Add points
			spline.addPoint(Vec3::ZERO);
			for (size_t numDev = 0; numDev < _numberOfSegments; ++numDev)
			{
				spline.addPoint(visualData->half[numDev]);
			}
			spline.addPoint(end);

			// Loop through all chain elements
			for (size_t j = 0; j < _maxChainElements; ++j)
			{
				PUBillboardChain::Element element = _billboardChain->getChainElement(visualData->chainIndex, j);

				// 1. Set the width of the chain if required
				if (particle->ownDimensions)
				{
					element.width = _rendererScale.x * particle->width;
				}

				// 2. Set positions of the elements
				element.position = spline.interpolate((float)j / (float)_maxChainElements);

				// 3. Set the colour
				element.color = particle->color;

				// 4. Update
				_billboardChain->updateChainElement(visualData->chainIndex, j, element);
			}
			visualData->setVisible(true);
		}
	}

	_billboardChain->render(renderer, transform, particleSystem);
}

PUParticle3DBeamRender::PUParticle3DBeamRender() : 
	_billboardChain(0),
	_quota(0),
	_useVertexColours(DEFAULT_USE_VERTEX_COLOURS),
	_maxChainElements(DEFAULT_MAX_ELEMENTS),
	_updateInterval(DEFAULT_UPDATE_INTERVAL),
	_deviation(DEFAULT_DEVIATION),
	_numberOfSegments(DEFAULT_NUMBER_OF_SEGMENTS),
	_jump(false),
	_texCoordDirection(DEFAULT_TEXTURE_DIRECTION)
{

}

PUParticle3DBeamRender::~PUParticle3DBeamRender()
{
	if (!_particleSystem)
		return;

	destroyAll();
}

void PUParticle3DBeamRender::particleEmitted( PUParticleSystem3D* particleSystem, PUParticle3D* particle )
{
	if (!particle->visualData && !_visualData.empty() && particle->particleType == PUParticle3D::PT_VISUAL)
	{
		particle->visualData = _visualData.back();
		PUParticle3DBeamVisualData* beamRendererVisualData = static_cast<PUParticle3DBeamVisualData*>(particle->visualData);
		beamRendererVisualData->setVisible(true, _rendererScale.x * particleSystem->getDefaultWidth()); // PU 1.4
		_visualData.pop_back();
	}
}

void PUParticle3DBeamRender::particleExpired( PUParticleSystem3D* particleSystem, PUParticle3D* particle )
{
	if (particle->visualData)
	{
		PUParticle3DBeamVisualData* beamRendererVisualData = static_cast<PUParticle3DBeamVisualData*>(particle->visualData);
		beamRendererVisualData->setVisible(false, 0); // PU 1.4
		_visualData.push_back(beamRendererVisualData);
		particle->visualData = nullptr;
	}
}

//-----------------------------------------------------------------------
bool PUParticle3DBeamRender::isUseVertexColours(void) const
{
	return _useVertexColours;
} 
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setUseVertexColours(bool useVertexColours)
{
	_useVertexColours = useVertexColours;
	if (!_billboardChain)
		return;

	_billboardChain->setUseVertexColours(_useVertexColours);
	_billboardChain->setUseTextureCoords(!_useVertexColours);
} 
//-----------------------------------------------------------------------
size_t PUParticle3DBeamRender::getMaxChainElements(void) const
{
	return _maxChainElements;
} 
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setMaxChainElements(size_t maxChainElements)
{
	_maxChainElements = maxChainElements;
} 
//-----------------------------------------------------------------------
float PUParticle3DBeamRender::getUpdateInterval(void) const
{
	return _updateInterval;
}
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setUpdateInterval(float updateInterval)
{
	_updateInterval = updateInterval;
}
//-----------------------------------------------------------------------
float PUParticle3DBeamRender::getDeviation(void) const
{
	return _deviation;
}
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setDeviation(float deviation)
{
	_deviation = deviation;
}
//-----------------------------------------------------------------------
size_t PUParticle3DBeamRender::getNumberOfSegments(void) const
{
	return _numberOfSegments;
}
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setNumberOfSegments(size_t numberOfSegments)
{
	_numberOfSegments = numberOfSegments;
}
//-----------------------------------------------------------------------
bool PUParticle3DBeamRender::isJump(void) const
{
	return _jump;
}
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setJump(bool jump)
{
	_jump = jump;
}
//-----------------------------------------------------------------------
PUBillboardChain::TexCoordDirection PUParticle3DBeamRender::getTexCoordDirection(void) const
{
	return _texCoordDirection;
}
//-----------------------------------------------------------------------
void PUParticle3DBeamRender::setTexCoordDirection(PUBillboardChain::TexCoordDirection texCoordDirection)
{
	_texCoordDirection = texCoordDirection;
}
//-----------------------------------------------------------------------

void PUParticle3DBeamRender::prepare()
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

	// Create BillboardChain
	std::stringstream ss; 
	ss << this;
	_billboardChainName = "Beam" + ss.str();
	_billboardChain = new PUBillboardChain(_billboardChainName, _texFile);
	_billboardChain->setDynamic(true);
	_billboardChain->setNumberOfChains(_quota);
	_billboardChain->setMaxChainElements(_maxChainElements);
	_billboardChain->setTextureCoordDirection(_texCoordDirection);
	setUseVertexColours(_useVertexColours);
	_billboardChain->setOtherTextureCoordRange(0.0f, 1.0f);

	// Create number of VisualData objects
	for (size_t i = 0; i < _quota; i++)
	{
		for (size_t j = 0; j < _maxChainElements; j++)
		{
			PUBillboardChain::Element element;
			element = PUBillboardChain::Element(Vec3::ZERO, _rendererScale.x * static_cast<PUParticleSystem3D *>(_particleSystem)->getDefaultWidth(), 0.0f, Vec4::ONE, Quaternion::identity()); // V1.51
			_billboardChain->addChainElement(i, element);
		}

		PUParticle3DBeamVisualData* visualData = new PUParticle3DBeamVisualData(i, _billboardChain);
		for (size_t numDev = 0; numDev < _numberOfSegments; ++numDev)
		{
			// Initialise the positions
			visualData->half[numDev] = Vec3::ZERO;
			visualData->destinationHalf[numDev] = Vec3::ZERO;
		}
		_allVisualData.push_back(visualData); // Managed by this renderer
		_visualData.push_back(visualData); // Used to assign to a particle
	}
}

void PUParticle3DBeamRender::unPrepare()
{
	destroyAll();
}

void PUParticle3DBeamRender::updateRender( PUParticle3D *particle, float deltaTime, bool firstParticle )
{
	if (!particle->visualData)
		return;

	PUParticle3DBeamVisualData* beamRendererVisualData = static_cast<PUParticle3DBeamVisualData*>(particle->visualData);
	beamRendererVisualData->timeSinceLastUpdate -= deltaTime;

	if (beamRendererVisualData->timeSinceLastUpdate < 0)
	{

		Vec3 end = particle->positionInWorld - static_cast<PUParticleSystem3D *>(_particleSystem)->getDerivedPosition();
		Vec3 perpendicular;
		float divide = (float)_numberOfSegments + 1.0f;
		for (size_t numDev = 0; numDev < _numberOfSegments; ++numDev)
		{
			Vec3::cross(end, Vec3(CCRANDOM_MINUS1_1(), CCRANDOM_MINUS1_1(), CCRANDOM_MINUS1_1()), &perpendicular);
			perpendicular.normalize();
			beamRendererVisualData->destinationHalf[numDev] = (((float)numDev + 1.0f) / divide) * end
				+ Vec3(_rendererScale.x * _deviation * perpendicular.x
				     , _rendererScale.y * _deviation * perpendicular.y
				     , _rendererScale.z * _deviation * perpendicular.z);
		}
		beamRendererVisualData->timeSinceLastUpdate += _updateInterval;
	}

	Vec3 diff;
	for (size_t numDev = 0; numDev < _numberOfSegments; ++numDev)
	{
		if (_jump)
		{
			beamRendererVisualData->half[numDev] = beamRendererVisualData->destinationHalf[numDev];
		}
		else
		{
			diff = beamRendererVisualData->destinationHalf[numDev] - beamRendererVisualData->half[numDev];
			beamRendererVisualData->half[numDev] = beamRendererVisualData->half[numDev] + deltaTime * diff;
		}
	}
}

//-----------------------------------------------------------------------
void PUParticle3DBeamRender::destroyAll(void)
{
	if (!_particleSystem || !_billboardChain)
		return;

	// Remove the listener
	static_cast<PUParticleSystem3D *>(_particleSystem)->removeListener(this);

	// Delete the BillboardChain
	CC_SAFE_DELETE(_billboardChain);

	// Delete the visual data
	std::vector<PUParticle3DBeamVisualData*>::const_iterator it;
	std::vector<PUParticle3DBeamVisualData*>::const_iterator itEnd = _allVisualData.end();
	for (it = _allVisualData.begin(); it != itEnd; ++it)
	{
		delete *it;
	}

	_allVisualData.clear();
	_visualData.clear();
}

PUParticle3DBeamRender* PUParticle3DBeamRender::clone()
{
	auto br = PUParticle3DBeamRender::create(_texFile);
	copyAttributesTo(br);
	return br;
}

void PUParticle3DBeamRender::copyAttributesTo( PUParticle3DRender *render )
{
	PUParticle3DRender::copyAttributesTo(render);
	PUParticle3DBeamRender *beamRender = static_cast<PUParticle3DBeamRender*>(render);
	beamRender->setUseVertexColours(_useVertexColours);
	beamRender->setMaxChainElements(_maxChainElements);
	beamRender->setUpdateInterval(_updateInterval);
	beamRender->setDeviation(_deviation);
	beamRender->setNumberOfSegments(_numberOfSegments);
	beamRender->setJump(_jump);
	beamRender->setTexCoordDirection(_texCoordDirection);
}

NS_CC_END
