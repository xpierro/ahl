#include <cell/codec/pngdec.h>
#include <stdlib.h>

/*cbCtrlMallocArg*/
typedef struct PngMallocArg{
	uint32_t mallocCallCounts;
} PngMallocArg;

/*cbCtrlFreeArg*/
typedef struct PngFreeArg{
	uint32_t freeCallCounts;
} PngFreeArg;

static void *pngMalloc(uint32_t size, void *cbCtrlMallocArg);
static int32_t pngFree(void *ptr, void *cbCtrlFreeArg);
void pngDecode(char *path);
void pngDestroy(void);
