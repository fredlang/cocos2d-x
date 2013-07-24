/****************************************************************************
 Copyright (c) 2013      Zynga Inc.
 
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

#include "CCFontAtlasCache.h"
#include "CCFontAtlasFactory.h"


NS_CC_BEGIN

std::map<std::string, FontAtlas *> FontAtlasCache::_atlasMap;


FontAtlas * FontAtlasCache::getFontAtlasTTF(const char *fontFileName, int size, GlyphCollection glyphs)
{
    std::string atlasName = generateFontName(fontFileName, size);
    FontAtlas *tempAtlas = _atlasMap[atlasName];
    
    if ( !tempAtlas )
    {
        tempAtlas = FontAtlasFactory::createAtlasFromTTF(fontFileName, size, glyphs);
        
        if (tempAtlas)
        {
            _atlasMap[atlasName] = tempAtlas;
        }
        else
        {
            return 0;
        }
    }
    
    return tempAtlas;
}

FontAtlas * FontAtlasCache::getFontAtlasFNT(const char *fontFileName)
{
    std::string atlasName = generateFontName(fontFileName, 0);
    FontAtlas *tempAtlas = _atlasMap[atlasName];
    
    if ( !tempAtlas )
    {
        tempAtlas = FontAtlasFactory::createAtlasFromFNT(fontFileName);
        
        if (tempAtlas)
        {
            _atlasMap[atlasName] = tempAtlas;
        }
        else
        {
            return 0;
        }
    }
    
    return tempAtlas;
}

std::string FontAtlasCache::generateFontName(const char *fontFileName, int size)
{
    std::string tempName(fontFileName);
    return tempName + std::to_string(size);
}

bool FontAtlasCache::releaseFontAtlas(FontAtlas *atlas)
{
    if (atlas)
    {
        for( auto &item: _atlasMap)
        {
            if ( item.second == atlas )
            {
                bool removeFromList = false;
                if(item.second->isSingleReference())
                    removeFromList = true;
                
                item.second->release();
                
                if (removeFromList)
                    _atlasMap.erase(item.first);
                
                return true;
            }
        }
    }
    
    return false;
}

NS_CC_END