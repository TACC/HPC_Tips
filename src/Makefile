export CPATH=/usr/include/mysql
export LIBRARY_PATH=/usr/lib64/mysql
CXXFLAGS := -g -O2

CXX := g++

install: showTip
	[ -n "$(DIR)" ] && cp $^ $(DIR) || true

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



