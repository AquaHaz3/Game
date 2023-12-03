#include "SceneFile.h"

#include <fstream>
#include "ByteReader.h"

#include "../model/Background.h"
#include "../model/ItemEntity.h"
#include "../model/Wall.h"
#include "../model/Block.h"

#define MAGIC_NUMBER 0xA98A4A2E

#define __READ_4_SHORTS br.ReadInt16(), br.ReadInt16(), br.ReadInt16(), br.ReadInt16()
#define __READ_2_SHORTS br.ReadInt16(), br.ReadInt16()
#define __READ_4_BYTES br.ReadByte(), br.ReadByte(), br.ReadByte(), br.ReadByte()

union _dword {
    uint32_t dword;
    uint16_t word[2];
    uint8_t bytes[4];
    char io[4];
};

union _word {
    uint16_t word;
    uint8_t bytes[2];
    char io[2];
};

static GameObject* defaultGameObjectFactory(SceneObjectType type, UtilsIO::ByteReader& br)
{
    switch (type)
    {
    case SceneObjectType::BACKGROUND:
        return new Background(__READ_4_SHORTS, (BlockID) br.ReadInt32());
    case SceneObjectType::WALL:
        return new Wall(__READ_4_SHORTS, (BlockID)br.ReadInt32());
    case SceneObjectType::BLOCK:
        return new Block(__READ_4_SHORTS, (BlockID)br.ReadInt32());
    case SceneObjectType::ITEM_ENTITIY:
        return new ItemEntity(__READ_4_SHORTS, (ItemID) br.ReadInt32(), {__READ_4_BYTES});
    case SceneObjectType::PLAYER:
        return new Player(__READ_2_SHORTS);
    default:
        throw std::runtime_error("[SceneFile] unknown SceneObjectType: "+std::to_string((int)type));
        break;
    }
}

void SceneFile::SaveScene(std::string name, int width, int height)
{
    if (objects.size() == 0) return;
    using namespace UtilsIO;
    std::ofstream output("data\\"+path+SCENE_FILE_EXTENSION, std::ios::binary);


    _dword magic; magic.dword = MAGIC_NUMBER; output.write(magic.io, 4);
    _word _width; _width.word = width; output.write(_width.io, 2);
    _word _height; _height.word = height; output.write(_height.io, 2);
    _word _c; _c.word = objects.size(); output.write(_c.io, 2);

    char proto96[96] = { 0 };
    char proto128[128] = { 0 };

    for (const auto& kv : objects) {
        PrototypeGameObject* p = kv.second;
        output.write((char*) &p->type, 1);
        switch ((SceneObjectType)p->type)
        {
        case SceneObjectType::BACKGROUND:
        case SceneObjectType::WALL:
        case SceneObjectType::BLOCK:
            memcpy(proto96, (uint8_t*)&p->x, 96);
            output.write(proto96, 96);
            break;
        case SceneObjectType::ITEM_ENTITIY:
            memcpy(proto128, (uint8_t*)&p->x, 128);
            output.write(proto128, 128);
            break;
        default:
            break;
        }
    }

    if (isHavePlayer) {
        uint8_t pType = (int)SceneObjectType::PLAYER;
        output.write((char*) &pType, 1);
        _dword pX; pX.dword = player_x;
        _dword pY; pY.dword = player_y;
        output.write((char*) pX.io, 4);
        output.write((char*) pY.io, 4);
    }

    output.close();

}

Scene* SceneFile::BuildScene()
{
    if (path == "") return nullptr;
    using namespace UtilsIO;
    std::ifstream input(path, std::ios::binary);
    int len = input.tellg(); 
    uint8_t* buffer = new uint8_t[len]; input.read((char*)buffer, len);

    ByteReader reader = ByteReader(buffer);
    uint32_t magic = reader.ReadInt32();
    if (magic != MAGIC_NUMBER) {
        throw std::runtime_error("[SceneFile] Bad magic word!");
        return 0;
    }
    int width = reader.ReadInt16();
    int height = reader.ReadInt16();
    Scene* scene = new Scene(width, height);
    int objectCount = reader.ReadInt16();
    for (int i = 0; i < objectCount; i++) {
        SceneObjectType type = (SceneObjectType) reader.ReadByte();
        scene->addObjectToScene(defaultGameObjectFactory(type, reader));
    }
    delete[] buffer;
    input.close();
    return scene;
}