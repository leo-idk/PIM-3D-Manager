# PIM 3D Manager

Sistema de gerenciamento integrado desenvolvido em linguagem C para controle de projetos, tarefas e estoque de filamentos voltado a operações de impressão 3D. 

O _`software_` consolida o controle de acesso por níveis de usuário (Membro e Gerente), a gestão do fluxo de trabalho por tarefas com dependências, o registro de entradas e saídas de matéria-prima, o rastreio de perdas, além de relatórios gerenciais e diário de bordo integrados.

```markdown
## Para Compilar o projeto:

## 1. Copie e execute este comando:

```bash
gcc -std=c99 -Wall -Wextra -o main main.c globals.c utils.c usuarios.c projetos.c tarefas.c estoque.c relatorios.c

```

Esse comando reúne os arquivos do sistema e gera o executável chamado main.

## 2. Abra o sistema

Se a compilação terminar sem erros, execute:
./main
O programa deverá iniciar no terminal.

## 3. Se aparecer um erro

Se o terminal informar que gcc não foi encontrado, instale o compilador com:
sudo dnf install gcc

```

```
