#pragma once

#include <limits.h>
#include <stdint.h>
#include <string>
#include <fstream>

class ByteInStream : public std::ifstream {

public:

	ByteInStream(std::string path);

	uint32_t     readInt32();
	uint64_t     readInt64();
	uint16_t     readShort();
	uint8_t      readByte();
	std::string  readUTFString();

};

class ByteOutStream : public std::ofstream {

public:

	ByteOutStream(std::string path);

	void writeInt32(uint32_t);
	void writeInt64(uint64_t);
	void writeShort(uint16_t);
	void writeByte(uint8_t);
	void writeUTFString(std::string);

};
