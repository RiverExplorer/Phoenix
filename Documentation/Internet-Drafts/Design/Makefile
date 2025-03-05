#
# Build the draft
#
#
.SUFFIXES:	.txt .svg .cpp .xml

TOP			=	../..

TITLE		=	Phoenix-Implementation-Design

VERSION	=	00

SOURCES.xml = \
	TOP.xml \
	Abstract.xml \
	Definitions.xml \
	DispatchThread.xml \
	FromNetworkThread.xml \
	InitialConnectionToServer.xml \
	Introduction.xml \
	IOModel.xml \
	MainThread.xml \
	SecurityConsiderations.xml \
	ToNetworkThread.xml \
	ToXDRThread.xml

ARTWORK.src = \
	IO-00.txt

ARTWORK.txt = $(addprefix Drawings/,$(ARTWORK.src))
SVG					= $(ARTWORK.src:.txt=.svg)
ARTWORK.svg = $(addprefix Generated/,$(SVG))

REFERENCES.xml =

# These files get processed by:
#
# cpp -P -nostdinc -C -undef <InFileName> > <OutFileName>
#
# to producte the INCLUDE.xml files.
#
INCLUDE.cpp = \
	Include/00-00-TOP-XDR.cpp

INCLUDE.xml = $(INCLUDE.cpp:.cpp=.xml)

INCLUDES1 := $(shell /bin/ls Include)
INCLUDES	= $(addprefix Include/,$(INCLUDES1))

# The XDR.cpp files are preprocessed with
# "cpp -P -nostdinc -C -undef"
# to produce GENERATED.XDR
#
# The .cpp files include data from the source code.
# this is done so that the source code and the documentation
# are in sync.
#
XDR.cpp 			:= $(shell /bin/ls Include/*-XDR.cpp)
GENERATED.XDR = $(XDR.cpp:.cpp=.xml)

DOCDIR			= .

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(DOCDIR)/$(TITLE).txt

TARGET.html	=	$(DOCDIR)/$(TITLE).html

TARGET.pdf	=	$(DOCDIR)/$(TITLE).pdf

#
# The:  "2> >(grep -v 'Warning: Found SVG with width')" below
# stops a warning message I do not care about from beeing shown.
#
REMOVE_SVG_WARNING = 2> >(grep -v 'Warning: Found SVG with width')

CPP = $(TOP)/MyTools/SimpleCpp

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt)

pdf : $(TARGET.pdf)

html : $(TARGET.html)

$(TARGET.txt) : ALL-XDR.xml $(ARTWORK.svg) $(ARTWORK.txt) $(TOPDOC.xml)  $(REFERENCES.xml) $(INCLUDES)
	xml2rfc -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.pdf) : ALL-XDR.xml $(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc --pdf -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.html): ALL-XDR.xml $(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc --html -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TOPDOC.xml) : TOP.xml ALL-XDR.xml $(SOURCES.xml) $(REFERENCES.xml) $(INCLUDES)
	xmllint --xinclude --format $< > $@

ALL-XDR.xml : $(INCLUDE.xml) $(GENERATED.XDR)
	echo "<section anchor='all-xdr'>" > $@
	echo "<name>Complete XDR</name>" >> $@
	cat $(GENERATED.XDR) >> $@
	echo "</section>" >> $@

$(INCLUDE.xml) : $(INCLUDE.cpp)

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.txt) riverexplorer.games:~/public_html/Phoenix

clean :
	$(RM) tmp.*
	$(RM) $(ARTWORK.svg)
	$(RM) $(TOPDOC.xml)
	$(RM) ALL-XDR.xml
	$(RM) $(GENERATED.XDR)

Generated/%.svg : Drawings/%.txt
	aasvg < $< > $@ 

Include/%.xml : Include/%.cpp
	cpp -P -nostdinc -C -undef $< > $@
