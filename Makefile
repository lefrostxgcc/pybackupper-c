INCLUDE_DIRS := .
CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRS))
HEADERS := zipbackup.h
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

