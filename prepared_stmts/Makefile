export CPATH=/usr/include/mysql
export LIBRARY_PATH=/usr/lib64/mysql
CXXFLAGS := -g -O0

CXX := g++

load_db: load_db.o
	$(LINK.C) -o $@ $^ -lmysqlclient

load_db.o: load_db.C

neat:
	$(RM) *~
clean: neat
	$(RM) *.o
clobber: clean
	$(RM) load_db

