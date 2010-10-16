#include <cell/codec/pngdec.h>
#include <stdlib.h>
#include <PSGL/psgl.h>

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
int pngDecode(GLuint width, GLuint height, GLuint& texture, char *path);
void pngDestroy(void);
