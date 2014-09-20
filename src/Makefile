export CPATH=/usr/include/mysql
export LIBRARY_PATH=/usr/lib64/mysql
CFLAGS := -g -O2

CXX := g++

install: showTip
	cp $^ $(DIR)

showTip: showTip.o
	$(LINK.C) -o $@ $^ -lmysqlclient

showTip.o: showTip.C version.h

neat:  neat_only

neat_only:
	$(RM) *~

clean: neat clean_only

clean_only:
	$(RM) *.o

clobber: clean clobber_only

clobber_only:
	$(RM) showTip



