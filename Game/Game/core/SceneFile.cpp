#include "SceneFile.h"

#include <fstream>
#include "ByteStream.h"

#include "../model/Background.h"
#include "../model/ItemEntity.h"
#include "../model/Wall.h"
#include "../model/Block.h"
#include "../model/entity/Mob.h"

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
    case SceneObjectType::MOB:
        return Entity::EntityFactory((EntityID)brush->ord, brush->x, brush->y, 32, 32, brush->ord);
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
        case SceneObjectType::MOB:
            return Entity::EntityFactory((EntityID)ord, x, y, 32, 32, ord);
        }
    }
    else {
        short x = br.readShort();
        short y = br.readShort();
        return new Player(x, y);
    }
    return 0;
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
    case SceneObjectType::MOB:
        return "Mob";
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
    case SceneObjectType::MOB:
        return (int)EntityID::Chest;
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
        return (int)Block::lastBlock;
    case SceneObjectType::ITEM_ENTITIY:
        return (int)ItemID::__lastItem;
    case SceneObjectType::PLAYER:
        return 0;
    case SceneObjectType::MOB:
        return (int)EntityID::__lastEntity;
    default:
        return 0;
        break;
    }

}

static void writeObject(PrototypeGameObject* p, ByteOutStream& output) {
    output.writeByte(p->type);
    switch ((SceneObjectType)p->type)
    {
    case SceneObjectType::BACKGROUND:
    case SceneObjectType::WALL:
    case SceneObjectType::BLOCK:
    case SceneObjectType::MOB:
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
        output.writeInt32(((int*)&p->color)[0]);
        break;
    default:
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
    output.writeShort((int)objects.size() + (int)backgroundObjects.size());

    char proto96[96] = { 0 };
    char proto128[128] = { 0 };

    for (const auto& kv : backgroundObjects) {
        writeObject(kv.second, output);
    }
    for (const auto& kv : objects) {
        writeObject(kv.second, output);
    }

    if (isHavePlayer) {
        uint8_t pType = (int)SceneObjectType::PLAYER;
        output.writeByte(pType);
        output.writeShort(player_x);
        output.writeShort(player_y);
    }

    output.close();

}

#include <functional>

static void LoadAndIterateObjects(std::string path, std::function<void(SceneObjectType, ByteInStream&)> iterate)
{
    if (path == "") return;

    ByteInStream reader = ByteInStream(path);
    if (!reader.is_open()) return;
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
        iterate(type, reader);
    }
    reader.close();
}

void SceneFile::LoadSceneForEditor(Scene* scene, SceneFile* context)
{
    LoadAndIterateObjects(path, [scene, context](SceneObjectType type, ByteInStream& br) {
        auto pos = br.tellg();
        short x = br.readShort();
        short y = br.readShort();
        short w = br.readShort();
        short h = br.readShort();
        int ord = br.readShort();
        br.seekg(pos);
        //printf("Load object: {%s, (%d, %d, %d, %d), %d}\n", SceneFile::getTypeName(type).c_str(), x, y, w, h, ord);
        if (type == SceneObjectType::PLAYER) {
            context->setPlayer(x, y);
        }
        else {
            context->addObject(new PrototypeGameObject(x, y, w, h, ord, (int)type, {0,0,0,0})); // TODO: Color ?
            GameObject* obj = defaultGameObjectFactory(type, br);   
            if (type == SceneObjectType::BACKGROUND) obj->flags |= SOLID_OBJECT;
            scene->addObjectToScene(obj);
        }
    });
}

void SceneFile::InitScene(Scene* scene)
{
    LoadAndIterateObjects(path, [scene](SceneObjectType type, ByteInStream& reader){
        if (type != SceneObjectType::PLAYER) {
            scene->addObjectToScene(defaultGameObjectFactory(type, reader));
        }
        else {
            scene->addPlayerToScene((Player*)defaultGameObjectFactory(type, reader));
        }
    });
}

void SceneFile::removeObjectInBox(AABB* box)
{
    auto toEraseI = std::vector<int>();
    for (const auto& kv : objects) {
        PrototypeGameObject* obj = kv.second;
        AABB objBox = {
            (float)(obj->x), (float)(obj->y),
            (float)(obj->x + obj->w), (float)(obj->y + obj->h)
        };
        if (UtilAABB::isOverlap(&objBox, box)) {
            toEraseI.push_back(kv.first);
        };
    }
    for (const auto& kv : backgroundObjects) {
        PrototypeGameObject* obj = kv.second;
        AABB oBox = {
            (float)(obj->x), (float)(obj->y),
            (float)(obj->w), (float)(obj->h)
        };
        printf("select: {%f, %f -> %f, %f}, block {%f, %f -> %f, %f}\n",
            box->min.x, box->min.y, box->max.x, box->max.y,
            oBox.min.x, oBox.min.y, oBox.max.x, oBox.max.y);
        if (UtilAABB::isOverlap(&oBox, box)) {
            toEraseI.push_back(kv.first);
        };
    }
    for (int i = 0; i < toEraseI.size(); i++) {
        removeObject(toEraseI[i]);
    }
}