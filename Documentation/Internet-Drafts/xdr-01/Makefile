#
# Build the draft
#
#
.SUFFIXES:	.txt .svg .cpp .xml

TOP			=	../..

TITLE		=	draft-royer-bits-in-xdr

VERSION	=	01

SOURCES.xml = \
	draft-01.xml

ARTWORK.src = \

ARTWORK.txt = \
		Assy.txt \
		EmailStatusSvg.txt \
		EmailStatusTxt.txt \
		ExtraWide.txt \
		ExtraWideA.txt \
		ExtraWideB.txt \
		ExtraWideC.txt

ARTWORK.svg		= $(ARTWORK.txt:.txt=.svg)

REFERENCES.xml = \
	References/reference.RFC.4506.xml

DOCDIR			= ../..

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(TITLE)-$(VERSION).txt

TARGET.html	=	$(TITLE)-$(VERSION).html

TARGET.pdf	=	$(TITLE)-$(VERSION).pdf

#
# The:  "2> >(grep -v 'Warning: Found SVG with width')" below
# stops a warning message I do not care about from beeing shown.
#
REMOVE_SVG_WARNING = 2> >(grep -v 'Warning: Found SVG with width')

CPP = $(TOP)/MyTools/SimpleCpp

XML2RFC_FLAGS = --v3 \
	--id-is-work-in-progress \
	--allow-local-file-access

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt)

pdf : $(TARGET.pdf)

html : $(TARGET.html)

$(TARGET.txt) : $(ARTWORK.svg) $(TOPDOC.xml)  $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.pdf) : $(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) --pdf -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.html):$(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) --html -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TOPDOC.xml) : draft-01.xml $(SOURCES.xml) $(REFERENCES.xml) $(INCLUDES)
	soelim $< | sed '/^\.lf*/d' > temp.xml
	xmllint --xinclude --format temp.xml > $@

$(INCLUDE.xml) : $(INCLUDE.cpp)

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.txt) riverexplorer.games:~/public_html/Phoenix

clean :
	$(RM) tmp.* temp.xml
	$(RM) $(ARTWORK.svg)
	$(RM) $(TOPDOC.xml)
	$(RM) $(GENERATED.XDR)

%.svg : %.txt
	aasvg < $< > $@ 
