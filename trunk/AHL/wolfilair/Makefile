.PHONY: gen_pkg npdrm_package

CELL_MK_DIR = $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk

PPU_SRCS = src/main.cpp src/lib/tools/Thread.cpp\
           src/lib/GL.cpp src/lib/shape/Shape.cpp\
           src/lib/Vertex3D.cpp src/lib/io/Pad.cpp\
           src/lib/tools/dialog/Dialog.cpp\
           src/lib/tools/dialog/ErrorDialog.cpp\
           src/lib/tools/dialog/InformationDialog.cpp\
           src/lib/io/Console.cpp\
           src/lib/io/DebugFont.cpp src/lib/network/Socket.cpp
PPU_TARGET = PS3Library.elf

PPU_INCDIRS	+= -I$(CELL_SDK)/target/ppu/include/sysutil
PPU_CXXSTDFLAGS		+= # utiliser -fno-exceptions pour linker des .a
PPU_CPPFLAGS		:= -DPSGL -D_DEBUG

PPU_LIBS		:= 
PPU_LDLIBDIR	:= -L$(PPU_PSGL_LIBDIR)


PPU_LDLIBS 	+= -lPSGL -lPSGLU -lPSGLFX -lm -lpthread\
		   -lresc_stub -lgcm_cmd -lgcm_sys_stub \
		   -lnet_stub -lio_stub -lsysutil_stub -lsysmodule_stub\
		   -ldbgfont -lnet_stub

include $(CELL_MK_DIR)/sdk.target.mk

gen_pkg: $(PPU_TARGET)
	$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)
	$(MAKE_FSELF) $(OBJS_DIR)/$(PPU_TARGET) EBOOT.BIN
	$(MAKE_FSELF_NPDRM) $(OBJS_DIR)/$(PPU_TARGET) \
	pkg/PS3_GAME/USRDIR/EBOOT.BIN
	$(MAKE_PACKAGE_NPDRM) pkg/ps3lib.conf pkg/PS3_GAME/
	mv *.pkg pkg/PS3Library.pkg # déplace le package
	
export_pkg: gen_pkg
	cp pkg/PS3Library.pkg /z/