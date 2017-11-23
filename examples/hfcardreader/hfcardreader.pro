TARGET = HfCardReader
TEMPLATE = app
CONFIG += windows embed_manifest_exe release

INCLUDEPATH += ./drivers/

HEADERS = HfCardReader.h \ 
          HfCardReaderWidget.h
SOURCES = main.cpp \ 
          HfCardReader.cpp \
          HfCardReaderWidget.cpp