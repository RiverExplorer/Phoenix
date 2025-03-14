#
# Project: Phoenix
# Time-stamp: <2025-03-14 13:37:10 doug>
#
# @file Makefile for RPCGEN++
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
TARGET = Generated/rpcgen.html Generated.rpcgen.css

SOURCES.tex = \
	BitFields.tex \
	Building.tex \
	CommandLine.tex \
	Comments.tex \
	Conditionals.tex \
	DataTypes.tex \
	DifferencesABNF.tex \
	DifferencesXDR.tex \
	FixedSizedArrays.tex \
	Introduction.tex \
	LegalMumboJumbo.tex \
	MemberFunctions.tex \
	OpaqueData.tex \
	PassThrough.tex \
	rpcgen.tex \
	Strings.tex \
	StructInUnion.tex \
	StructOutsideUnion.tex \
	TestCoverage.tex \
	VariableSizedArrays.tex \
	WhereToGet.tex

DRAWINGS.txt = \
	64bitBitField.txt

GENERATED_DRAWINGS.svg = \
	64bitBitField.svg

$(TARGET) : $(SOURCES.tex) $(GENERATED_DRAWINGS.svg)
	make4ht -d Generated rpcgen.tex

64bitBitField.svg : 64bitBitField.txt
	aasvg < 64bitBitField.txt > 64bitBitField.svg

clean :
	$(RM) -rf Generated
	$(RM) rpcgen.4ct
	$(RM) rpcgen.4tc
	$(RM) rpcgen.aux
	$(RM) rpcgen.dvi
	$(RM) rpcgen.idv
	$(RM) rpcgen.lg
	$(RM) rpcgen.log
	$(RM) rpcgen.pdf
	$(RM) rpcgen.tmp
	$(RM) rpcgen.xref
