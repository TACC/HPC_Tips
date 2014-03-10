VERSION_SRC := src/version.h
all:
	cd src; make

neat:
	$(RM) *~
	cd src; make $@

clean:  neat
	cd src; make $@

clobber: clean
	cd src; make $@



gittag:
        ifneq ($(TAG),)
	  @git status -s > /tmp/tips$$$$;                                            \
          if [ -s /tmp/tips$$$$ ]; then                                              \
	    echo "All files not checked in => try again";                            \
	  else                                                                       \
	    echo '#define VERSION "$(TAG)"'                       >  $(VERSION_SRC); \
            git commit -m "moving to TAG_VERSION $(TAG)"             $(VERSION_SRC); \
            git tag -a $(TAG) -m 'Setting TAG_VERSION to $(TAG)'                   ; \
	    git push --tags                                                        ; \
          fi;                                                                        \
          rm -f /tmp/tips$$$$
        else
	  @echo "To git tag do: make gittag TAG=?"
        endif
