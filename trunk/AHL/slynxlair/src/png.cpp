#include <sys/process.h>
#include <sys/spu_initialize.h>
#include <cell/codec/pngdec.h>
#include <cell/sysmodule.h>
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>
#include <stdlib.h>
#include <GLES/gl.h>
#include "png.h"

CellPngDecMainHandle pngHandleM;
CellPngDecSubHandle pngHandleS;
CellPngDecThreadInParam pngThreadInParam;
CellPngDecThreadOutParam pngThreadOutParam;

CellPngDecSrc pngSrc;
CellPngDecOpnInfo pngOpenInfo;
CellPngDecInfo pngInfo;
CellPngDecDataOutInfo pngDataOutInfo;

CellPngDecInParam pngInParam;
CellPngDecOutParam pngOutParam;
CellPngDecDataCtrlParam pngDataCtrlParam;
PngMallocArg pngMallocArg;
PngFreeArg pngFreeArg;




/*CellPngDecCbControlMalloc*/
static void *pngMalloc(uint32_t size, void *cbCtrlMallocArg){
    PngMallocArg *arg;
    arg = (PngMallocArg *)cbCtrlMallocArg;
    arg->mallocCallCounts++;
	return malloc(size);
}

/*CellPngDecCbControlFree*/
static int32_t pngFree(void *ptr, void *cbCtrlFreeArg){
    PngFreeArg *arg;
    arg = (PngFreeArg *)cbCtrlFreeArg;
    arg->freeCallCounts++;
	free(ptr);
	return 0;
}

int pngDecode(GLuint width, GLuint height,GLuint &pngText, char *path)
{
    int32_t error;

    error = cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
    if(error != CELL_OK)
    {
        return 1;
    }

    error = cellSysmoduleLoadModule(CELL_SYSMODULE_PNGDEC);
    if(error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        return 2;
    }

    //volatile ThreadStat_t threadState;
    //uint8_t *data = (uint8_t*)(height * width * 4);
    //char* buffer = (char*) malloc(sizeof(char) * height * width * 4);

    //sys_spu_initialize(2, 0);

    pngMallocArg.mallocCallCounts = 0;
	pngFreeArg.freeCallCounts = 0;
    pngThreadInParam.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_DISABLE;
    pngThreadInParam.ppuThreadPriority = 0;
    pngThreadInParam.spuThreadPriority = 16;
    pngThreadInParam.cbCtrlMallocFunc = pngMalloc;
    pngThreadInParam.cbCtrlMallocArg = &pngMallocArg;
    pngThreadInParam.cbCtrlFreeFunc = pngFree;
    pngThreadInParam.cbCtrlFreeArg = &pngFreeArg;

    error = cellPngDecCreate(&pngHandleM, &pngThreadInParam, &pngThreadOutParam);
    if (error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
        return 3;
    }

    //threadState = THREAD_INIT;
/////////////////////////////////////////////
    pngSrc.srcSelect = CELL_PNGDEC_FILE;
    pngSrc.fileName = path;
    pngSrc.fileOffset = 0;
    pngSrc.fileSize = 0;
    pngSrc.streamPtr = NULL;
    pngSrc.streamSize = 0;
    pngSrc.spuThreadEnable = CELL_PNGDEC_SPU_THREAD_DISABLE;

    error = cellPngDecOpen(pngHandleM, &pngHandleS, &pngSrc, &pngOpenInfo);
    if (error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
        return 4;
    }
    error = cellPngDecReadHeader(pngHandleM, pngHandleS, &pngInfo);
    if (error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
        return 5;
    }

    pngInParam.commandPtr        = NULL;
    pngInParam.outputMode        = CELL_PNGDEC_TOP_TO_BOTTOM;
    pngInParam.outputColorSpace  = pngInfo.colorSpace;
    pngInParam.outputBitDepth    = pngInfo.bitDepth;
    pngInParam.outputPackFlag    = CELL_PNGDEC_1BYTE_PER_1PIXEL;
    if((pngInfo.colorSpace == CELL_PNGDEC_GRAYSCALE_ALPHA)
     ||(pngInfo.colorSpace == CELL_PNGDEC_RGBA)
     ||(pngInfo.chunkInformation & 0x10))
        {pngInParam.outputAlphaSelect = CELL_PNGDEC_STREAM_ALPHA;}
    else{
         pngInParam.outputAlphaSelect = CELL_PNGDEC_FIX_ALPHA;
        }
    pngInParam.outputColorAlpha  = 0xff;

    error = cellPngDecSetParameter(pngHandleM, pngHandleS, &pngInParam, &pngOutParam);
    if (error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
        return 6;
    }

    size_t buff_size = pngOutParam.outputWidth * pngOutParam.outputComponents * pngOutParam.outputHeight;
    buff_size = (buff_size + 0xfffff) & (~0xfffff);
    uint8_t *buffer = (uint8_t*)memalign(0x100000, buff_size);
    pngDataCtrlParam.outputBytesPerLine = (pngOutParam.outputWidth * pngOutParam.outputComponents);
    error = cellPngDecDecodeData(pngHandleM, pngHandleS, buffer, &pngDataCtrlParam, &pngDataOutInfo);
    if (error != CELL_OK)
    {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
        cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);
        return 7;
    }

    /*glGenTextures(1, &pngText);
    glBindTexture(GL_TEXTURE_2D, pngText);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer );*/

    cellPngDecClose(pngHandleM, pngHandleS);
    cellPngDecDestroy(pngHandleM);

    cellSysmoduleUnloadModule(CELL_SYSMODULE_FS);
    cellSysmoduleUnloadModule(CELL_SYSMODULE_PNGDEC);

    free( buffer );
    return 0;
}

void pngDestroy()
{
    cellPngDecClose(pngHandleM, pngHandleS);

	cellPngDecDestroy(pngHandleM);
}
