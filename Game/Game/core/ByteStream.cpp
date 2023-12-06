#include "ByteStream.h"

union qword {
	uint64_t qword;
	uint32_t dword;
	uint16_t word;
	uint8_t  byte;
};

ByteInStream::ByteInStream(std::string path)
	: std::ifstream(path, std::ios::binary)
{

}

uint32_t ByteInStream::readInt32()
{
	qword q;
	read((char*)&q, sizeof(uint32_t));
	return q.dword;
}

uint64_t ByteInStream::readInt64()
{
	qword q;
	read((char*)&q, sizeof(uint64_t));
	return q.qword;
}

uint16_t ByteInStream::readShort()
{
	qword q;
	read((char*)&q, sizeof(uint16_t));
	return q.word;
}

uint8_t ByteInStream::readByte()
{
	uint8_t c[1] = { 0 };
	read((char*)&c, sizeof(uint8_t));
	return c[0];
}

std::string ByteInStream::readUTFString()
{
	int length = readShort();
	char* buffer = new char[length];
	read(buffer, length);
	auto str = std::string(buffer);
	delete[] buffer;
	return str;
}

ByteOutStream::ByteOutStream(std::string path)
	: std::ofstream(path, std::ios::binary)
{

}

void ByteOutStream::writeInt32(uint32_t data)
{
	write((char*) & data, sizeof(uint32_t));
}

void ByteOutStream::writeInt64(uint64_t data)
{
	write((char*)&data, sizeof(uint64_t));
}

void ByteOutStream::writeShort(uint16_t data)
{
	write((char*)&data, sizeof(uint16_t));
}

void ByteOutStream::writeByte(uint8_t data)
{
	write((char*)&data, sizeof(uint8_t));
}

void ByteOutStream::writeUTFString(std::string str)
{
	writeShort(str.size());
	write(str.c_str(), str.size());
}
