TARGETS=$(shell ls *.vsl | sed s/\.vsl//g)
all: ${TARGETS}
%: %.vsl
    ../src/vslc -T -s < $*.vsl > $*.correct_output
clean:
    -rm -f *.correct_output
purge: clean
    -rm -f ${TARGETS}