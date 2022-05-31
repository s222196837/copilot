SUBDIRS = altimu10 battery gps pilot proximity

default: $(SUBDIRS)
	for d in `echo $^ `; do \
            if test -d "$$d"; then \
	    	pushd $$d; \
                echo === $$d ===; \
		qmake; make; \
		./test$$d; \
		popd; \
            fi; \
        done

clean: $(SUBDIRS)
	for d in `echo $^ `; do \
            if test -d "$$d"; then \
	    	pushd $$d; \
                echo === $$d ===; \
		qmake; make clean; \
		rm ./test$$d; \
		popd; \
            fi; \
        done
