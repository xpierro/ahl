/*
 * PNG.cpp
 *
 *  Created on: 16 oct. 2010
 *      Author: Pierre
 */

#include "PNG.h"
#include "../2D/GL2D.h"

#include <fstream>

using namespace std;

namespace PS3 {

PNG::PNG(){}

PNG::PNG(string path) {
	initPngDec();
	loaded = false;
	glGenTextures(1, &texId);
	pngPath = path;
	loadFromDisc();
}

PNG::~PNG() {
	free(buffer);
}

void PNG::make(string path)
{
    initPngDec();
	loaded = false;
	glGenTextures(1, &texId);
	pngPath = path;
	loadFromDisc();
}

bool PNG::pngInitialized = false;

static void pngCallback(const uint64_t status, const uint64_t p, void *u) {
	(void) p;
	(void) u;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		PNG::closePngDec();
	}
}

void PNG::initPngDec() {
	if (!pngInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
		cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
		if(GL2D::libraryStarted)
		{
		    GL2D::addUserCallback(pngCallback);
		}
		else
            GL::addUserCallback(pngCallback);

		pngInitialized = true;
	}
}

void PNG::closePngDec() {
	cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
	pngInitialized = false;
}

GLuint PNG::getTextureId() {
	if (!loaded) {
		loadFromDisc();
	}
	return texId;
}

int PNG::getWidth()
{
    return width;
}
int PNG::getHeight()
{
    return height;
}

/**
 * Memory allocation callback - simple one for now
 */
void* pngAllocator(uint32_t size, void *cbCtrlMallocArg) {
	(void) cbCtrlMallocArg;
	return malloc(size);
}

int32_t pngFreer(void* ptr, void* cbCtrlFreeArg) {
	(void) cbCtrlFreeArg;
	free(ptr);
	return 0;
}


void PNG::loadFromDisc() {
	// Step one - Create the Decoder
	CellPngDecThreadInParam threadInParam;
	threadInParam.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_DISABLE; // No spu
	threadInParam.ppuThreadPriority = 0; // Max priority
	threadInParam.cbCtrlMallocFunc = pngAllocator; // Memory allocator
	threadInParam.cbCtrlMallocArg = NULL; // Do we need to count malloc calls?
	threadInParam.cbCtrlFreeFunc = pngFreer; // Frees the memory
	threadInParam.cbCtrlFreeArg = NULL;

	CellPngDecMainHandle mainHandle;
	CellPngDecThreadOutParam threadOutParam;

	cellPngDecCreate(&mainHandle, &threadInParam, &threadOutParam);

	// Step two - Open the PNG file
	CellPngDecSrc pngDecSrc;
	// We want to load a file, not a buffer
	pngDecSrc.srcSelect = CELL_PNGDEC_FILE;
	pngDecSrc.fileName = pngPath.c_str(); // Where to find the file
	pngDecSrc.fileOffset = 0; // We decode from the beginning of the png
	pngDecSrc.fileSize = 0; // Infinite: decode until EOF or error
	pngDecSrc.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_DISABLE; // No spu

	CellPngDecSubHandle subHandle;
	CellPngDecOpnInfo openInfo;

	cellPngDecOpen(mainHandle, &subHandle, &pngDecSrc, &openInfo);

	// Step three - Get info about the picture (read the header)
	CellPngDecInfo decInfo;
	cellPngDecReadHeader(mainHandle, subHandle, &decInfo);
	width = decInfo.imageWidth;
	height = decInfo.imageHeight;
	colorSpace = decInfo.colorSpace;
	bitDepth = decInfo.bitDepth;

	// Step four - Prepare the decoder
	CellPngDecInParam pngDecInParam;
	// We're not multithreading for now, no way to stop the process
	pngDecInParam.commandPtr = NULL;
	// Very hard to know for sure
	pngDecInParam.outputMode = CELL_PNGDEC_BOTTOM_TO_TOP;
	pngDecInParam.outputColorSpace = colorSpace;
	pngDecInParam.outputBitDepth = bitDepth;
	// Used only if we have less than 1byte per pixel, which is rare
	pngDecInParam.outputPackFlag = CELL_PNGDEC_1BYTE_PER_1PIXEL;
	if ( decInfo.colorSpace == CELL_PNGDEC_GRAYSCALE_ALPHA
	     || decInfo.colorSpace == CELL_PNGDEC_RGBA
	     || (decInfo.chunkInformation & 0x10)) {
			// We keep the same alpha as the original picture
	        pngDecInParam.outputAlphaSelect = CELL_PNGDEC_STREAM_ALPHA;
	} else {
		pngDecInParam.outputAlphaSelect = CELL_PNGDEC_FIX_ALPHA;
	}
	pngDecInParam.outputColorAlpha  = 0xff; // If a fixed alpha is needed

	CellPngDecOutParam pngDecOutParam;

	cellPngDecSetParameter(mainHandle, subHandle,
						   &pngDecInParam, &pngDecOutParam);

	// Here we should verify that enough memory is available for the decoder
	// with testing pngDecOutParam.useMemorySpace for allocation, useless for
	// small fish.

	// Step five - Build a buffer
	// The buffer should have 1byte per color info + alpha, so 4byte per pixel
	// for most png pictures, the info about that is given in the pngDecOutParam
	// and is called outputBitDepth, but refering to the number of components
	// per pixel is safer. For RGBA the number is 4
	// We have width * height pixels in the png

	int bufferSize = pngDecOutParam.outputComponents
			         * pngDecOutParam.outputWidth * pngDecOutParam.outputHeight;

	// For now, rather than memalign we'll use malloc
	buffer = (uint8_t*) malloc(bufferSize);

	// Step six - Launch the Decoder and wait
	CellPngDecDataCtrlParam dataCtrlParam;
	// Other values are made to pad lines with 0s
	dataCtrlParam.outputBytesPerLine = width * pngDecOutParam.outputComponents;

	CellPngDecDataOutInfo dataOutInfo;

	cellPngDecDecodeData(mainHandle, subHandle, buffer,
			             &dataCtrlParam, &dataOutInfo);


	// Last step: clean the decoder
	cellPngDecClose(mainHandle, subHandle);
	cellPngDecDestroy(mainHandle);

	convertToTexture();
	loaded = true;
}

void PNG::convertToTexture() {
	glBindTexture(GL_TEXTURE_2D, texId);

	int texColorSpace = GL_RGBA;
	switch(colorSpace) {
	case CELL_PNGDEC_RGB: texColorSpace = GL_RGB; break;
	case CELL_PNGDEC_ARGB: texColorSpace = GL_ARGB_SCE; break;
	default: break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			     0, texColorSpace, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Removes the one-pixel line that appears sometimes around textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}



}
