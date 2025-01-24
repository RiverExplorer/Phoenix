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
	AdminSummary.xml \
	AuthBinaryValues.xml \
	AuthSummary.xml \
	CalendarSummary.xml \
	CapabilitySummary.xml \
	Definitions.xml \
	EMailSummary.xml \
	draft-royer-phoenix-00.xml \
	FileBinaryValues.xml \
	FolderSummary.xml \
	Introduction.xml \
	KeepAliveSummary.xml \
	PingSummary.xml \
	ProtocolBinaryValues.xml \
	ProtocolDetails.xml \
	ProtocolSummary.xml \
	SMimeSummary.xml

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

$(TARGET.pdf) : $(SOURCES.xml)
	$(CPP) $(TOPDOC.xml) > tmp.xml
	xml2rfc --pdf tmp.xml
	mv tmp.pdf $@

$(TARGET.html): $(SOURCES.xml)
	$(CPP) $(TOPDOC.xml) > tmp.xml
	xml2rfc --html tmp.xml
	mv tmp.html $@

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.txt) riverexplorer.games:~/public_html/Phoenix
clean :
	$(RM) tmp.*
	$(RM) $(TARGET.txt) $(TARGET.html) $(TARGET.pdf)
