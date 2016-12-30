CC=gcc
CFLAGS=-g
LDFLAGS=-lavutil -lavformat -lavcodec -lchromaprint
SRCDIR=src
BUILDDIR=obj
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

mtag: $(OBJS)
	$(CC) -o $@ $< $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)