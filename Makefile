INCLUDE_DIRS := .
override CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))
override CFLAGS += -Wall -Wextra -Werror
override LIBS += -lzip
HEADERS := zipbackup.h zipfile.h
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TARGET := pybackupper

.PHONY = all clean fclean re

vpath %.h $(INCLUDE_DIRS)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

$(OBJS) : $(HEADERS)

clean :
	-rm -f $(OBJS)

fclean : clean
	-rm -f $(TARGET)

re: fclean all

