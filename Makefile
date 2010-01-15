all:
	g++ main.cpp -o es -W -Wall -Werror

fclean:
	rm -f *~ *.o es

re:	fclean all

.PHONY:	all fclean
