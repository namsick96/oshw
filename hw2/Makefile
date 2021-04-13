all : minishell program1 program2 program3

minishell : minishell.cpp
	g++ -std=c+11 minishell.cpp -o minishell
program1 : program1.cpp
	g++ -std=c+11 program1.cpp -o program1
program2 : program2.cpp
	g++ -std=c+11 program2.cpp -o program2
program3 : program3.cpp
	g++ -std=c+11 program3.cpp -lpthread -o program3

clean : 
	rm -rf minishell
	rm -rf program1
	rm -rf program2
	rm -rf program3