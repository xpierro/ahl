#include "Font.h"

#include "cell/sysmodule.h"

namespace PS3 {

bool Font::fontInitialized = false;
const CellFontLibrary* Font::library = NULL;
int Font::uniqueId = 1;

Font::Font() {
	initFont();
	fontId = uniqueId;
	uniqueId += 1;
}

Font::~Font() { }

static void fontCallback(const uint64_t status, const uint64_t p, void *u) {
	(void) p;
	(void) u;
	if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
		Font::closeFonts();
	}
}

static void* fontMalloc(void* object, unsigned int size) {
	(void) object;
	return malloc(size);
}

static void fontFree(void* object, void* ptr) {
	(void) object;
	free(ptr);
}

static void* fontRealloc(void* object, void* ptr, unsigned int newSize) {
	(void) object;
	return realloc(ptr, newSize);
}
static void* fontCalloc(void* object, unsigned int elementNumber,
									  unsigned int elementSize ) {
	(void) object;
	return calloc(elementNumber, elementSize);
}

void Font::initFont() {
	if (!fontInitialized) {
		cellSysmoduleLoadModule(CELL_SYSMODULE_FS);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONT);
		cellSysmoduleLoadModule(CELL_SYSMODULE_FREETYPE );
		cellSysmoduleLoadModule(CELL_SYSMODULE_FONTFT);

		// 4MB cache used to speed up Font operations
		int cacheSize = 4 * 1024 * 1024;
		void* fontCache = malloc(cacheSize);
		// Array of font description
		CellFontEntry* userFontEntrys =
			(CellFontEntry*) malloc(sizeof(CellFontEntry) * MAX_FONTS_LOADABLE);
		CellFontConfig config;
		config.FileCache.buffer = (unsigned int*) fontCache;
		config.FileCache.size = cacheSize;
		config.userFontEntrys = userFontEntrys;
		config.userFontEntryMax = MAX_FONTS_LOADABLE;
		cellFontInit(&config);

		// Now we configure the freetype part.
		CellFontLibraryConfigFT freetypeConfig;
		CellFontLibraryConfigFT_initialize(&freetypeConfig);
		freetypeConfig.MemoryIF.Object  = NULL; // First arg of the functions
		freetypeConfig.MemoryIF.Malloc  = fontMalloc;
		freetypeConfig.MemoryIF.Free    = fontFree;
		freetypeConfig.MemoryIF.Realloc = fontRealloc;
		freetypeConfig.MemoryIF.Calloc  = fontCalloc;
		cellFontInitLibraryFreeType(&freetypeConfig, &library);

		GL::addUserCallback(fontCallback);
		fontInitialized = true;
	}
}

void Font::closeFonts() { }

const CellFontLibrary* Font::getLibrary() const {
	return library;
}

int Font::getFontId() const {
	return fontId;
}

CellFont* Font::getFontDescriptor() {
	return &font;
}

}
