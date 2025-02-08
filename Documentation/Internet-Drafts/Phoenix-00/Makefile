#
# Build the draft
#
#
.SUFFIXES:	.txt .svg

TOP			=	../..

TITLE		=	draft-royer-phoenix

VERSION	=	00

SOURCES.xml = \
	Abstract.xml \
	AdminBinaryValues.xml \
	AdminSummary.xml \
	AuthBinaryValues.xml \
	AuthSummary.xml \
	CalendarSummary.xml \
	CapabilitySummary.xml \
	Commands.xml \
	Definitions.xml \
	EMailSummary.xml \
	draft-00.xml \
	FileBinaryValues.xml \
	Folder.xml \
	Index.xml \
	Introduction.xml \
	KeepAliveSummary.xml \
	MetaData.xml \
	PingSummary.xml \
	ProtocolBinaryValues.xml \
	ProtocolDetails.xml \
	ProtocolSummary.xml \
	RequirementsLanguage.xml \
	SMimeSummary.xml

ARTWORK.txt = \
	ABNF-00.txt \
	IndexList-01.txt \
	IndexList-02.txt \
	IndexList-03.txt \
	IndexList-04.txt \
	IndexList-05.txt \
	IndexList-06.txt \
	IndexList-07.txt \
	IndexList-08.txt \
	IndexList-09.txt \
	IndexList-10.txt \
	MDN-00.txt \
	Packet-00.txt \
	PhoenixString-00.txt

GENERATED_ARTWORK.svg = $(ARTWORK.txt:.txt=.svg)

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(TITLE)-$(VERSION).txt

TARGET.html	=	$(TITLE)-$(VERSION).html

TARGET.pdf	=	$(TITLE)-$(VERSION).pdf

CPP = $(TOP)/MyTools/SimpleCpp

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt) $(ARTWORK.txt)

pdf : $(TARGET.pdf) $(GENERATED_ARTWORK.svg)

html : $(TARGET.html) (ARGWORK.svg)

$(TARGET.txt) : $(ARTWORK.txt) $(TOPDOC.xml)
	xml2rfc $(TOPDOC.xml)

$(TARGET.pdf) : $(GENERATED_ARTWORK.svg) $(TOPDOC.xml)
	xml2rfc --pdf $(TOPDOC.xml)

$(TARGET.html): $(GENERATED_ARTWORK.svg) $(TOPDOC.xml)
	xml2rfc --html $(TOPDOC.xml)

$(TOPDOC.xml) : draft-00.xml $(SOURCES.xml)
	xmllint --xinclude --format $< > $@

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.txt) riverexplorer.games:~/public_html/Phoenix

clean :
	$(RM) tmp.*
	$(RM) $(TARGET.txt) $(TARGET.html) $(TARGET.pdf)
	$(RM) $(GENERATED_ARTWORK.svg)
	$(RM) $(TOPDOC.xml)

%.svg : %.txt
	aasvg < $< > $@
