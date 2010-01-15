all:
	g++ main.cpp backward_chaining.cpp -o es -W -Wall -Werror -g

fclean:
	rm -f *~ *.o es

re:	fclean all

.PHONY:	all fclean
