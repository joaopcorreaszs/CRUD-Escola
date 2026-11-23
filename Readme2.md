# Trabalho em C - Sistema de Notas Escolares

## Objetivo do trabalho
CRUD de notas de alunos, com média e conceito:
- A = 9 a 10
- B = 6 a 9
- C = 3 a 6
- D = 0 a 3

Além do CRUD, o trabalho pede uma nova estrutura de dados (lista, fila, pilha,
árvore ou grafo) que resolva melhor o problema.

## Estrutura escolhida: FILA ENCADEADA (FIFO)
Motivos (explicados no topo do arquivo notas_escola.c):
- Alunos são atendidos por ordem de chegada.
- Não tem limite fixo de 100 alunos (usa malloc).
- Operações simples: enqueue no fim, dequeue no início, busca percorrendo.

Estruturas descartadas: pilha (só o último), árvore (complexidade
desnecessária, mas boa se a escola tiver milhares de alunos), grafo (serve
para relações, não para cadastro).

## Arquivos
- notas_escola.c - código-fonte completo
- notas_escola.exe - programa já compilado
- compilar_e_rodar.bat - duplo clique para compilar e executar

## Compilador
O computador não tinha compilador C. Instalei o GCC (WinLibs 16.1.0) em:
C:\Users\jota\AppData\Local\PackiaTools\gcc\mingw64\bin

Para compilar manualmente no PowerShell:
  $env:Path = "$env:LOCALAPPDATA\PackiaTools\gcc\mingw64\bin;$env:Path"
  gcc -std=c11 -Wall -Wextra -pedantic -o notas_escola.exe notas_escola.c

## Cuidados ao testar com entrada redirecionada
O PowerShell adiciona um caractere invisível (BOM) ao gerar arquivos de
entrada. Se for testar automático, use cmd, por exemplo:
  cmd /c "type entrada.txt | notas_escola.exe"
Digitando no teclado normalmente isso não acontece.
