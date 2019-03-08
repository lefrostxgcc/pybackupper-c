INCLUDE_DIRS := .
CC := gcc
override CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))
CFLAGS += -g -O0 -Wall -Wextra -Werror
override LIBS += -lzip
HEADERS := zipbackup.h zipfile.h
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TARGET := pybackupper

.PHONY = all clean fclean re

vpath %.h $(INCLUDE_DIRS)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJS) : $(HEADERS)

clean :
	-rm -f $(OBJS)

fclean : clean
	-rm -f $(TARGET)

re: fclean all

