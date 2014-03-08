export CPATH=/usr/include/mysql
CFLAGS := -g -O0

CC := gcc

showTip: showTip.o
	$(LINK.c) -o $@ $^ -lmysqlclient

showTip.o: showTip.c

neat:
	$(RM) *~

clean: neat
	$(RM) *.o

clobber: clean
	$(RM) showTip



