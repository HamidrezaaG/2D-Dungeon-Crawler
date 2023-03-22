#pragma once
#include "../Utility/Helpers.h"
#include "Math/Vector.h"

namespace fw {


    class SpriteSheet {

    public:

        struct SpriteInfo {

            SpriteInfo() {
            };

            SpriteInfo(int _x, int _y, int _h, int _w, SpriteSheet* _sheet) : m_pSheet(_sheet) {
                UV_Offset = Vector2((float)_x / m_pSheet->SheetWidth, (float)_y / m_pSheet->SheetHeight);
                UV_Scale = Vector2((float)_w / m_pSheet->SheetWidth, (float)_h / m_pSheet->SheetHeight);
            };

            SpriteSheet* m_pSheet = nullptr;
            Vector2 UV_Scale;
            Vector2 UV_Offset;
        };

        SpriteSheet(int width, int height);
        SpriteSheet(const char* _filename);
        ~SpriteSheet();

        static SpriteSheet* Load(const char* _filename)
        {
            char* jsonstring = fw::LoadCompleteFile(_filename, nullptr);
            rapidjson::Document document;
            document.Parse(jsonstring);
            delete[] jsonstring;

            unsigned int sheetWidth = document["Width"].GetInt();
            unsigned int sheetHeight = document["Height"].GetInt();

            SpriteSheet* pSpriteSheetToReturn = new SpriteSheet(sheetWidth, sheetHeight);
            bool ReadSpriteSize = false;

            for (unsigned int i = 0; i < document["Sprites"].Size(); i++)
            {
                rapidjson::Value& spriteinformation = document["Sprites"][i];
                unsigned int readX = spriteinformation["X"].GetInt();
                unsigned int readY = spriteinformation["Y"].GetInt();
                unsigned int readH = spriteinformation["H"].GetInt();
                unsigned int readW = spriteinformation["W"].GetInt();
                const char* readName = spriteinformation["Name"].GetString();

                SpriteInfo* readSprite = new SpriteInfo(readX, readY, readH, readW, pSpriteSheetToReturn);
                
                if (!ReadSpriteSize)
                {
                    pSpriteSheetToReturn->SpriteSize = IVector2(readW, readH);
                    ReadSpriteSize = true;
                }

                pSpriteSheetToReturn->AddMappedSprite(readName, readSprite);
            }
            return pSpriteSheetToReturn;
        }

        //void AddSprite(SpriteInfo* _spriteinfo);

        void AddMappedSprite(std::string _name, SpriteInfo* _spriteinfo);

        //std::vector<SpriteInfo*> GetSpriteList() const;
        
        //SpriteInfo* GetSpriteInfoByName(const char* _name) const;

        unsigned int SheetHeight = 0, SheetWidth = 0;
        IVector2 SpriteSize;
        std::map<std::string, SpriteInfo*> Sprites;

    };
} // namespace fw


