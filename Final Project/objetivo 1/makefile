all:

	
	cat /dev/null>"arquivo1.txt"
	cat /dev/null>"arquivo2.txt"
	cat /dev/null>"saida.txt"
	
	./gen.py | head -50 | sort | ./split.py arquivo1.txt arquivo2.txt
	
	clear
	
	gcc buffers.c -o buffers.exe
	./buffers.exe
