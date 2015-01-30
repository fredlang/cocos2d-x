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

#include "CCPUParticle3DDoEnableComponentEventHandlerTranslator.h"
#include "3dparticle/ParticleUniverse/CCPUParticleSystem3D.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DDynamicAttribute.h"
#include "3dparticle/ParticleUniverse/CCPUParticle3DDynamicAttributeTranslator.h"

NS_CC_BEGIN

PUParticle3DDoEnableComponentEventHandlerTranslator::PUParticle3DDoEnableComponentEventHandlerTranslator()
{
}
//-------------------------------------------------------------------------
bool PUParticle3DDoEnableComponentEventHandlerTranslator::translateChildProperty( PUScriptCompiler* compiler, PUAbstractNode *node )
{
    PUPropertyAbstractNode* prop = reinterpret_cast<PUPropertyAbstractNode*>(node);
    PUParticle3DEventHandler* evt = static_cast<PUParticle3DEventHandler *>(prop->parent->context);
    PUParticle3DDoEnableComponentEventHandler* handler = static_cast<PUParticle3DDoEnableComponentEventHandler*>(evt);

    if (prop->name == token[TOKEN_DOENABLE_COMPONENT])
    {
        // Property: enable_component
        if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_DOENABLE_COMPONENT], 3))
        {
            std::string componentType;
            std::string name;
            bool enabled = true;
            PUAbstractNodeList::const_iterator i = prop->values.begin();
            if(getString(**i, &componentType))
            {
                if (componentType == token[TOKEN_DOENABLE_EMITTER_COMPONENT])
                {
                    handler->setComponentType(CT_EMITTER);
                }
                else if (componentType == token[TOKEN_DOENABLE_AFFECTOR_COMPONENT])
                {
                    handler->setComponentType(CT_AFFECTOR);
                }
                else if (componentType == token[TOKEN_DOENABLE_OBSERVER_COMPONENT])
                {
                    handler->setComponentType(CT_OBSERVER);
                }
                else if (componentType == token[TOKEN_DOENABLE_TECHNIQUE_COMPONENT])
                {
                    handler->setComponentType(CT_TECHNIQUE);
                }
                ++i;
                if(getString(**i, &name))
                {
                    handler->setComponentName(name);
                    ++i;
                    if (getBoolean(**i, &enabled))
                    {
                        handler->setComponentEnabled(enabled);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool PUParticle3DDoEnableComponentEventHandlerTranslator::translateChildObject( PUScriptCompiler* compiler, PUAbstractNode *node )
{
    // No objects
    return false;
}

NS_CC_END