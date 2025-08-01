#ifndef __JAVASTREAM_H__
#define __JAVASTREAM_H__

#include <stdint.h>
#include <stdio.h>
#define LT_RESOURCE 5
#define LT_FILE		6
#define LT_SOUND_RESOURCE	7 // [GEC]
static constexpr const char* dir = "Doom2rpg.app";

class Applet;

// ------------------
// InputStream Class
// ------------------

class InputStream
{
private:
    
    int field_0x0;
    int field_0x4;
    
    FILE* file;
    int fileSize;
    int field_0x28;
    
public:
    uint32_t cursor;
    static constexpr int LOADTYPE_RESOURCE = 5;
	static constexpr int LOADTYPE_FILE = 6;
    uint8_t* data;
    
	// Constructor
	InputStream();
	// Destructor
	~InputStream();
    uint8_t* getTop();
    const uint8_t* getData();
    int getFileSize();
    void offsetCursor(int offset);
	bool startup();
	bool loadResource(const char* fileName);
	bool loadFile(const char* fileName, int loadType);
	void close();
	int readInt();
	int readShort();
	bool readBoolean();
	uint8_t readByte();
	uint8_t readUnsignedByte();
	int readSignedByte();
	void read(uint8_t* dest, int off, int size);
};

// -------------------
// OutputStream Class
// -------------------

class OutputStream
{
private:
    uint8_t* getTop();
    bool canWrite(int typeSizeof);
    bool isOpen;
    FILE* file;
    uint8_t* buffer;
    uint32_t written;
    uint8_t* writeBuff;
    int field_0x24_;
    int fileSize;
    int flushCount;
    bool noWrite;
    Applet* App;
    
public:
	// Constructor
	OutputStream();
	// Destructor
	~OutputStream();

	int openFile(const char* fileName, int openMode);
	void close();
	int flush();
	void writeInt(int i);
	void writeShort(int16_t i);
	void writeByte(uint8_t i);
	void writeBoolean(bool b);
	void write(uint8_t* buff, int off, int size);
};

#endif
