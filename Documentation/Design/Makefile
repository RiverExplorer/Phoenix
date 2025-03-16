#
# Project: Phoenix
# Time-stamp: <2025-03-16 00:03:11 doug>
#
# @file Makefile
# @author Douglas Mark Royer
# @date 24-FEB-2025
#
# @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
#
# Licensed under the MIT License. See LICENSE file
# or https://opensource.org/licenses/MIT for details.
#
# RiverExplorer is a trademark of Douglas Mark Royer
#
.SUFFIXES: .txt .svg

TARGET = Generated/Phoenix.html Generated.Phoenix.css

SOURCES.tex = \
	Phoenix.tex \
	Introduction.tex \
	PacketDetails.tex \
	ClientMainThread.tex \
	ServerMainThread.tex \
	OutputThread.tex \
	XdrEncode.tex \
	TlsIOOut.tex \
	TlsIOIn.tex \
	XdrDecode.tex \
	DispatchThread.tex \
	ClientImplementation.tex \
	ServerImplementation.tex \
	WhereToGet.tex \
	Building.tex \
	TestCoverage.tex \
	LegalMumboJumbo.tex

# Some of these files and their names match
# the ones in the internet draft, so I kept the
# names the same to know at a glance they are the
# same file.
#
TXT_DRAWINGS.txt = \
	64bitBitField.txt \
	IO-00.txt \
	IovPacket.txt \
	IovPacket2.txt \
	MultiplePacket.txt \
	SinglePacket.txt

GENERATED_DRAWINGS  = $(addprefix Generated/,$(TXT_DRAWINGS.txt:.txt=.svg))

$(TARGET) : Generated $(SOURCES.tex) $(GENERATED_DRAWINGS)
	make4ht -d Generated Phoenix.tex

Generated :
	@mkdir Generated

Generated/%.svg : Drawings/%.txt
	aasvg < $< > $@

clean :
	$(RM) -rf Generated
	$(RM) *.4ct
	$(RM) *.4tc
	$(RM) *.aux
	$(RM) *.dvi
	$(RM) *.idv
	$(RM) *.lg
	$(RM) *.log
	$(RM) *.pdf
	$(RM) *.xref
	$(RM) *.tmp
	$(RM) Phoenix.css Phoenix.html

