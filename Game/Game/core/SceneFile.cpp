#include "SceneFile.h"

#include <fstream>
#include "ByteStream.h"

#include "../model/Background.h"
#include "../model/ItemEntity.h"
#include "../model/Wall.h"
#include "../model/Block.h"

#define MAGIC_NUMBER 0x41555141

GameObject* SceneFile::brushGameObjectFactory(PrototypeGameObject* brush)
{
    switch ((SceneObjectType) brush->type)
    {
    case SceneObjectType::BACKGROUND:
        return new Wall(brush->x, brush->y, brush->x + brush->w, brush->y + brush->h, (BlockID) brush->ord);
    case SceneObjectType::WALL:
        return new Wall(brush->x, brush->y, brush->x+ brush->w, brush->y+brush->h, (BlockID)brush->ord);
    case SceneObjectType::BLOCK:
        return new Block(brush->x, brush->y, (BlockID)brush->ord);
    case SceneObjectType::ITEM_ENTITIY:
        return new ItemEntity(brush->x, brush->y, 32, 32, (ItemID) brush->ord);
    case SceneObjectType::PLAYER:
        return new Player(brush->x, brush->y);
    default:
        throw std::runtime_error("[SceneFile] unknown SceneObjectType: " + std::to_string((int)brush->type));
        break;
    }
}

static GameObject* defaultGameObjectFactory(SceneObjectType type, ByteInStream& br)
{
    if (type != SceneObjectType::PLAYER) {
        short x = br.readShort();
        short y = br.readShort();
        short w = br.readShort();
        short h = br.readShort();
        int ord = br.readShort();
        //printf("    - {%d, %d, %d, %d} [%d]\n", x, y, w, h, ord);
        if (type == SceneObjectType::ITEM_ENTITIY) {
            int color32 = br.readInt32();
            Color p = { 0,0,0,0 }; memcpy(&p, &color32, 4);
            return new ItemEntity(x, y, w, h, (ItemID)ord, p);
        }
        switch (type)
        {
        case SceneObjectType::BACKGROUND:
            return new Background(x,y,w,h, (BlockID) ord);
        case SceneObjectType::WALL:
            return new Wall(x, y, x+w, y+h, (BlockID)ord);
        case SceneObjectType::BLOCK:
            return new Block(x, y, w, h, (BlockID)ord);
        }
    }
    else {
        short x = br.readShort();
        short y = br.readShort();
        return new Player(x, y);
    }
}

std::string SceneFile::getTypeName(SceneObjectType type)
{
    switch (type)
    {
    case SceneObjectType::BACKGROUND:
        return "Background";
    case SceneObjectType::WALL:
        return "Wall";
    case SceneObjectType::BLOCK:
        return "Block";
    case SceneObjectType::ITEM_ENTITIY:
        return "ItemEntity";
    case SceneObjectType::PLAYER:
        return "Player";
    default:
        throw std::runtime_error("[SceneFile] unknown SceneObjectType: " + std::to_string((int)type));
        break;
    }
}

int SceneFile::getObjectDefaultID(SceneObjectType type) {
    switch (type)
    {
    case SceneObjectType::BACKGROUND:
        return (int) BlockID::GRAVEL;
    case SceneObjectType::WALL:
    case SceneObjectType::BLOCK:
        return (int)BlockID::WALL;
    case SceneObjectType::ITEM_ENTITIY:
        return (int)ItemID::POTION_HEAL;
    case SceneObjectType::PLAYER:
        return 0;
    default:
        return 0;
        break;
    }
}

int SceneFile::getObjectLastID(SceneObjectType type) {

    switch (type)
    {
    case SceneObjectType::BACKGROUND:
        return (int)BlockID::__lastBackground;
    case SceneObjectType::WALL:
    case SceneObjectType::BLOCK:
        return (int)BlockID::__lastBlock;
    case SceneObjectType::ITEM_ENTITIY:
        return (int)ItemID::__lastItem;
    case SceneObjectType::PLAYER:
        return 0;
    default:
        return 0;
        break;
    }

}

void SceneFile::SaveScene(std::string name, int width, int height)
{
    if (objects.size() == 0) return;

    auto output = ByteOutStream("data\\"+name+SCENE_FILE_EXTENSION);

    output.writeInt32(MAGIC_NUMBER);
    output.writeShort(width);
    output.writeShort(height);
    output.writeShort(objects.size());

    char proto96[96] = { 0 };
    char proto128[128] = { 0 };

    for (const auto& kv : objects) {
        PrototypeGameObject* p = kv.second;
        output.writeByte(p->type);
        switch ((SceneObjectType)p->type)
        {
        case SceneObjectType::BACKGROUND:
        case SceneObjectType::WALL:
        case SceneObjectType::BLOCK:
            output.writeShort(p->x);
            output.writeShort(p->y);
            output.writeShort(p->w);
            output.writeShort(p->h);
            output.writeShort(p->ord);
            //printf("[SceneFile] Save block {%d, %d, %d, %d} with ID [%d]\n", p->x, p->y, p->w, p->h, p->ord);
            break;
        case SceneObjectType::ITEM_ENTITIY:
            output.writeShort(p->x);
            output.writeShort(p->y);
            output.writeShort(p->w);
            output.writeShort(p->h);
            output.writeShort(p->ord);
            output.writeInt32(((int*) & p->color)[0]);
            break;
        default:
            break;
        }
    }

    if (isHavePlayer) {
        uint8_t pType = (int)SceneObjectType::PLAYER;
        output.writeByte(pType);
        output.writeShort(player_x);
        output.writeShort(player_y);
    }

    output.close();

}

void SceneFile::InitScene(Scene* scene)
{
    if (path == "") return;

    ByteInStream reader = ByteInStream(path);
    uint32_t magic = reader.readInt32();
    if (magic != MAGIC_NUMBER) {
        throw std::runtime_error("[SceneFile] Bad magic word!");
        return;
    }
    int width = reader.readShort();
    int height = reader.readShort();
    int objectCount = reader.readShort();
    for (int i = 0; i < objectCount + 1; i++) {
        SceneObjectType type = (SceneObjectType)reader.readByte();
        //printf("[SceneFile] load object (%d/%d) with type = %d\n", i, objectCount, type);
        if (type != SceneObjectType::PLAYER) {
            scene->addObjectToScene(defaultGameObjectFactory(type, reader));
        }
        else {
            scene->addPlayerToScene((Player*)defaultGameObjectFactory(type, reader));
        }
    }
    reader.close();
}

void SceneFile::removeObjectInBox(AABB* box)
{
    auto toErase = std::vector<PrototypeGameObject*>();
    auto toEraseI = std::vector<int>();
    for (const auto& kv : objects) {
        PrototypeGameObject* obj = kv.second;
        AABB objBox = {
            (float)(obj->x), (float)(obj->y),
            (float)(obj->x + obj->w), (float)(obj->y + obj->h)
        };
        if (UtilAABB::isOverlap(&objBox, box)) {
            toErase.push_back(obj);
            toEraseI.push_back(kv.first);
        };
    }
    for (int i = 0; i < toErase.size(); i++) {
        PrototypeGameObject* obj = toErase[i];
        if (obj != 0) delete obj;
        objects.erase(toEraseI[i]);
    }
}