daas: $(wildcard *.c *.h)
	${CC} -o $@ -m32 $(filter %.c,$^)
