#include "FrameworkPCH.h"
#include "SpriteSheet.h"

namespace fw
{
    SpriteSheet::SpriteSheet(int width, int height): SheetWidth(width), SheetHeight(height)
    {

    }

    SpriteSheet::SpriteSheet(const char* _filename)
    {
        char* jsonstring = fw::LoadCompleteFile(_filename, nullptr);
        rapidjson::Document document;
        document.Parse(jsonstring);
        delete[] jsonstring;

        SheetWidth = document["Width"].GetInt();
        SheetHeight = document["Height"].GetInt();

        bool ReadSpriteSize = false;

        for (unsigned int i = 0; i < document["Sprites"].Size(); i++)
        {
            rapidjson::Value& spriteinformation = document["Sprites"][i];
            unsigned int readX = spriteinformation["X"].GetInt();
            unsigned int readY = spriteinformation["Y"].GetInt();
            unsigned int readH = spriteinformation["H"].GetInt();
            unsigned int readW = spriteinformation["W"].GetInt();
            const char* readName = spriteinformation["Name"].GetString();

            SpriteInfo* readSprite = new SpriteInfo(readX, readY, readH, readW, this);

            if (!ReadSpriteSize)
            {
                SpriteSize = IVector2(readW, readH);
                ReadSpriteSize = true;
            }

            AddMappedSprite(readName, readSprite);
        }
    }

    SpriteSheet::~SpriteSheet()
    {
        for (std::pair<std::string, SpriteInfo*> SelectedSprite : Sprites)
        {
            delete SelectedSprite.second;
        }
    }

    void SpriteSheet::AddMappedSprite(const std::string _name, SpriteInfo* _spriteinfo)
    {
        Sprites[_name] = _spriteinfo;
    }

  //void SpriteSheet::AddMappedSprite(std::map<std::string, fw::SpriteInfo*> _spriteinfo)
  //{
  //
  //}

  //  void SpriteSheet::AddSprite(SpriteInfo* _spriteinfo)
  //  {
  //      m_Sprites.push_back(_spriteinfo);
  //
  //  }

    //std::vector<SpriteInfo*> SpriteSheet::GetSpriteList()
    //{
    //    return m_Sprites;
    //}
    //
    //SpriteInfo* SpriteSheet::GetSpriteByName(const char* _name)
    //{
    //    //TODO
    //    return nullptr;
    //}


   

} // namespace fw
