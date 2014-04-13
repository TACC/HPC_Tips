VERSION_SRC := src/version.h
NAME        := tacc_tips
all:
	cd src; make

neat:
	$(RM) *~
	cd src; make $@_only

clean:  neat
	cd src; make $@_only

clobber: clean
	cd src; make $@_only


install: $(DESTDIR)$(DIR)
	cd src; make install DIR=$(DESTDIR)$(DIR)

$(DESTDIR)$(DIR):
	mkdir -p $@

dist:
	git archive --prefix=$(NAME)-`cat .version`/ master | bzip2 > $(NAME)-`cat .version`.tar.bz2

gittag:
        ifneq ($(TAG),)
	  @git status -s > /tmp/tips$$$$;                                            \
          if [ -s /tmp/tips$$$$ ]; then                                              \
	    echo "All files not checked in => try again";                            \
	  else                                                                       \
	    echo $(TAG)                                           >  .version;       \
	    echo '#define VERSION "$(TAG)"'                       >  $(VERSION_SRC); \
            git commit -m "moving to TAG_VERSION $(TAG)"    .version $(VERSION_SRC); \
            git tag -a $(TAG) -m 'Setting TAG_VERSION to $(TAG)'                   ; \
	    git push --tags                                                        ; \
          fi;                                                                        \
          rm -f /tmp/tips$$$$
        else
	  @echo "To git tag do: make gittag TAG=?"
        endif
