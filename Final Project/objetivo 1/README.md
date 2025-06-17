# Gerador de dados para o trabalho de Sistemas Operativos 23/24

## Programas

O programa ./gen.py gera uma sequência de registos de tamanho 16 para o stdout.
O tamanho dos registos pode ser alterado com dois argumentos que indicam o
tamanho máximo e minimo.

O programa ./split.py separa os registos lidos do stdin aleatoriamente por cada
um dos ficheiros indicados como argumentos, preservando a sua ordenação (se
existir).

## Exemplos

Gerar ficheiros para o Obj. #1 com um total de 1000 linhas de 16 carateres:

    $ ./gen.py | head -1000 | sort | ./split.py f1.txt f2.txt

Gerar ficheiros para os Obj. #2 e #3:

    $ ./gen.py | head -1000 | sort | ./split.py f1.txt f2.txt f3.txt f4.txt

Gerar ficheiros para a Val. do Obj. #2:

    $ ./gen.py | head -1000 | sort | ./split.py f1.txt f2.txt f3.txt f4.txt f5.txt f6.txt f7.txt f8.txt

Gerar um ficheiro para a Val. 1 do Obj. #3:

    $ ./gen.py | head -1000 > f.txt

Gerar ficheiros para a Val. 2 do Obj. #3 (registos de 5 a 20 carateres):

    $ ./gen.py 20 5 | head -1000 | sort | ./split.py f1.txt f2.txt f3.txt f4.txt

Gerar um ficheiro para as Val. 2+3 do Obj. #3:

    $ ./gen.py 20 5 | head -1000 > f.txt

## Verificação

Pode verificar se o seu programa está a funcionar corretamente se, depois de
guardar o resultado num ficheiro r.txt, o seguinte comando:

    $ cat f*.txt | sort | cmp r.txt

não apresentar qualquer erro.
