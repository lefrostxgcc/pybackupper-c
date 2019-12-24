INCLUDE_DIRS := .
override CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRS)) `pkg-config json-glib-1.0 --cflags`
CFLAGS += -Wall -Wextra
override LDLIBS += -lzip `pkg-config json-glib-1.0 --libs`
HEADERS := zipbackup.h zipfile.h
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TARGET := pybackupper

.PHONY = all clean

vpath %.h $(INCLUDE_DIRS)

all : $(TARGET)

$(TARGET) : $(OBJS)

$(OBJS) : $(HEADERS)

clean :
	-rm -f $(TARGET) $(OBJS)
