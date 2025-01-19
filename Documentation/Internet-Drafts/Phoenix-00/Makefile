#
# Build the draft
#
#
TOP			=	../..

TITLE		=	draft-royer-phoenix

VERSION	=	00

SOURCES.xml = \
	Abstract.xml \
	AdminBinaryValues.xml \
	AdminOverview.xml \
	AuthBinaryValues.xml \
	AuthOverview.xml \
	draft-royer-phoenix-00.xml \
	FileBinaryValues.xml \
	FileOverview.xml \
	Introduction.xml \
	Definitions.xml \
	ProtocolBinaryValues.xml \
	ProtocolDetails.xml \
	ProtocolOverview.xml

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(TITLE)-$(VERSION).txt

TARGET.html	=	$(TITLE)-$(VERSION).html

TARGET.pdf	=	$(TITLE)-$(VERSION).pdf

CPP = $(TOP)/MyTools/SimpleCpp

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt)

pdf : $(TARGET.pdf)

html : $(TARGET.html)

$(TARGET.txt) : $(SOURCES.xml)
	$(CPP) $(TOPDOC.xml) > tmp.xml
	xml2rfc tmp.xml
	mv tmp.txt $@
	scp $@ riverexplorer.games:~/public_html/Phoenix

$(TARGET.pdf) : $(SOURCES.xml)
	$(CPP) $(TOPDOC.xml) > tmp.xml
	xml2rfc --pdf tmp.xml
	mv tmp.pdf $@
	scp $@ riverexplorer.games:~/public_html/Phoenix

$(TARGET.html): $(SOURCES.xml)
	$(CPP) $(TOPDOC.xml) > tmp.xml
	xml2rfc --html tmp.xml
	mv tmp.html $@
	scp $@ riverexplorer.games:~/public_html/Phoenix

clean :
	$(RM) tmp.*
	$(RM) $(TARGET.txt) $(TARGET.html) $(TARGET.pdf)
