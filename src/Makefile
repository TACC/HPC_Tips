export CPATH=/usr/include/mysql
export LIBRARY_PATH=/usr/lib64/mysql
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



