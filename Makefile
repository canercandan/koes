all:
	g++ main.cpp -o es

fclean:
	rm -f *~ *.o es

.PHONY:	all fclean
