export CPATH=/usr/local/Cellar/mysql/5.6.16/include/mysql
export LIBRARY_PATH=/usr/local/lib
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



