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

#include "CCPUParticle3DDoPlacementParticleEventHandlerTranslator.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DDynamicAttribute.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DDynamicAttributeTranslator.h"

NS_CC_BEGIN

PUParticle3DDoPlacementParticleEventHandlerTranslator::PUParticle3DDoPlacementParticleEventHandlerTranslator()
{
}
//-------------------------------------------------------------------------
bool PUParticle3DDoPlacementParticleEventHandlerTranslator::translateChildProperty( PUScriptCompiler* compiler, PUAbstractNode *node )
{
    PUPropertyAbstractNode* prop = reinterpret_cast<PUPropertyAbstractNode*>(node);
    PUParticle3DEventHandler* evt = static_cast<PUParticle3DEventHandler *>(prop->parent->context);
    PUParticle3DDoPlacementParticleEventHandler* handler = static_cast<PUParticle3DDoPlacementParticleEventHandler*>(evt);

    if (prop->name == token[TOKEN_DOPLACE_FORCE_EMITTER])
    {
        // Property: force_emitter
        if (passValidateProperty(compiler, prop, token[TOKEN_DOPLACE_FORCE_EMITTER], VAL_STRING))
        {
            std::string val;
            if(getString(*prop->values.front(), &val))
            {
                handler->setForceEmitterName(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_DOPLACE_NUMBER_OF_PARTICLES])
    {
        // Property: number_of_particles
        if (passValidateProperty(compiler, prop, token[TOKEN_DOPLACE_NUMBER_OF_PARTICLES], VAL_UINT))
        {
            unsigned int val = 0;
            if(getUInt(*prop->values.front(), &val))
            {
                handler->setNumberOfParticles(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_POSITION])
    {
        // Property: inherit_position
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_POSITION], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritPosition(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_DIRECTION])
    {
        // Property: inherit_direction
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_DIRECTION], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritDirection(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_ORIENTATION])
    {
        // Property: inherit_orientation
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_ORIENTATION], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritOrientation(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_TIME_TO_LIVE])
    {
        // Property: inherit_time_to_live
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_TIME_TO_LIVE], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritTimeToLive(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_MASS])
    {
        // Property: inherit_mass
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_MASS], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritMass(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_TEXTURE_COORDINATE])
    {
        // Property: inherit_texture_coord
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_TEXTURE_COORDINATE], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritTextureCoordinate(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_COLOUR])
    {
        // Property: inherit_colour
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_COLOUR], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritColour(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_WIDTH])
    {
        // Property: inherit_width
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_WIDTH], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritParticleWidth(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_HEIGHT])
    {
        // Property: inherit_height
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_HEIGHT], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritParticleHeight(val);
                return true;
            }
        }
    }
    else if (prop->name == token[TOKEN_INHERIT_DEPTH])
    {
        // Property: inherit_depth
        if (passValidateProperty(compiler, prop, token[TOKEN_INHERIT_DEPTH], VAL_BOOL))
        {
            bool val;
            if(getBoolean(*prop->values.front(), &val))
            {
                handler->setInheritParticleDepth(val);
                return true;
            }
        }
    }

    return false;
}

bool PUParticle3DDoPlacementParticleEventHandlerTranslator::translateChildObject( PUScriptCompiler* compiler, PUAbstractNode *node )
{
    // No objects
    return false;
}

NS_CC_END