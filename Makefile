all:
	g++ main.cpp -o es

fclean:
	rm -f *~ *.o es

re:	fclean all

.PHONY:	all fclean
