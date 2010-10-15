#!/usr/bin/make
# SCE CONFIDENTIAL
# PLAYSTATION(R)3 Programmer Tool Runtime Library 192.001
#                 Copyright (C) 2007 Sony Computer Entertainment Inc.
#                                                All Rights Reserved.
.PHONY: gen_pkg npdrm_package

include MakeDefs

GFX_COMMON=Common

PPU_INCDIRS		+= -Iinclude
PPU_CPPFLAGS		+= -DPSGL
PPU_CXXSTDFLAGS		+= -fno-exceptions
PPU_SRCS		:= main.cpp Common/gfxObject.cpp \
			Common/gfxCommon.cpp \
			png.cpp \

PPU_TARGET		:= hello_world.elf
PPU_LDLIBS		:= -lpngdec_stub -lsysmodule_stub $(GRAPHICS_SAMPLE_CORE_LIBS)

VPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/vs_basicSample.cg
FPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/fs_basicSample.cg

include MakeRules

gen_pkg:: $(PPU_TARGET)
	$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)
	$(MAKE_FSELF) $(OBJS_DIR)/$(PPU_TARGET) EBOOT.BIN				# to use in /app_home/PS3_GAME
	$(MAKE_FSELF_NPDRM) $(OBJS_DIR)/$(PPU_TARGET) PS3_GAME/USRDIR/EBOOT.BIN
	$(MAKE_PACKAGE_NPDRM) package.conf PS3_GAME/
