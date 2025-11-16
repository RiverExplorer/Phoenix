#
# Project: Phoenix / Protocol
#
# @file Makefile
# @author Douglas Mark Royer
#
# @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
#
# Licensed under the MIT License. See LICENSE file
# or https://opensource.org/licenses/MIT for details.
#
# RiverExplorer is a trademark of Douglas Mark Royer
#
.SUFFIXES: .txt .svg

TARGET = Generated/Protocol.html Generated.Protocol.css

SOURCES.tex = \
	Protocol.tex \
	Building.tex \
	ClientImplementation.tex \
	Directives.tex \
	Introduction.tex \
	LegalMumboJumbo.tex \
	ServerImplementation.tex \
	TestCoverage.tex \
	TlsIOIn.tex \
	TlsIOOut.tex \
	WhereToGet.tex \
	XdrDecode.tex \
	XdrEncode.tex

# Some of these files and their names match
# the ones in the internet draft, so I kept the
# names the same to know at a glance they are the
# same file and topic.
#
TXT_DRAWINGS.txt =

INCLUDE_FILES	=

GENERATED_DRAWINGS  = $(addprefix Generated/,$(TXT_DRAWINGS.txt:.txt=.svg))

$(TARGET) : Generated $(SOURCES.tex) $(GENERATED_DRAWINGS) $(INCLUDE_FILES)
	make4ht -d Generated Protocol.tex

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

