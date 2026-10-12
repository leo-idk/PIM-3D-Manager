# PIM 3D Manager

Sistema simples de controle de projetos e estoque para operações de impressão 3D, programado em linguagem C. O software centraliza:

* **Controle de Acesso:** Permissões por níveis de usuário (Membro e Gerente).
* **Gestão de Projetos e Tarefas:** Acompanhamento de fluxo de trabalho.
* **Controle de Estoque:** Registro de entradas, saídas e rastreio de perdas de matéria-prima.
* **Relatórios e Histórico:** Relatórios gerenciais e diário de bordo.

## Para Compilar o projeto:

## 1. Copie e execute este comando:

#### No Linux:

```bash
gcc -std=c99 -Wall -Wextra -o main main.c globals.c utils.c usuarios.c projetos.c tarefas.c estoque.c relatorios.c
./main

```
#### No Windows (GCC instalado):

```cmd
gcc -std=c99 -Wall -Wextra -o main.exe main.c globals.c utils.c usuarios.c projetos.c tarefas.c estoque.c relatorios.c
main.exe

```
#### No Windows (usando o Code::Blocks):

Basta abrir o projeto `.cbp` no Code::Blocks e apertar **F9**.

Esse comando reúne os arquivos do sistema e gera o executável chamado main.

## 2. Abra o sistema

Se a compilação terminar sem erros, execute:
./main
O programa deverá iniciar no terminal.

## 3. Se aparecer um erro

Se o terminal informar que gcc não foi encontrado, instale o compilador com:

```
sudo dnf install gcc
```
