TARGETS=$(shell ls *.vsl | sed s/\.vsl/\.tree/g)
all: ${TARGETS}
%: %.s
%.tree: %.vsl
	../src/vslc -T <$*.vsl > $*.tree
clean:
	-rm -f *.tree
purge: clean
	-rm -f ${TARGETS}
testall: all
	-for n in $$( ls *.vsl | sed s/\.vsl//g ) ; do diff $$n.tree $$n.tree.correct ; done ;
