re_to_nfa: main.o evaluate.o nfa.o stack.o stack.h nfa.h
	gcc -g -o re_to_nfa main.o evaluate.o nfa.o stack.o stack.h nfa.h
main.o: main.c stack.h nfa.h
	gcc -g -c main.c
evaluate.o: evaluate.c stack.h nfa.h
	gcc -g -c evaluate.c
nfa.o: nfa.c stack.h nfa.h
	gcc -g -c nfa.c
stack.o: stack.c stack.h
	gcc -g -c stack.c
clean:
	rm main.o evaluate.o nfa.o stack.o re_to_nfa
