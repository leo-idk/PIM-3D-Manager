# Sistema PIM

Sistema desenvolvido em linguagem C para gerenciamento de usuários, projetos, tarefas, estoque de filamentos e relatórios.

## Compilação

Abra `PrjSistemaPIM.cbp` no Code::Blocks e compile o projeto. Os arquivos `.dat` são criados localmente durante o uso.

## Organização

- `main.c`: inicialização e fluxo principal.
- `structs.h`: estruturas, constantes e protótipos compartilhados.
- `globals.c`: contadores e mapeamentos globais.
- `utils.c`: funções auxiliares.
- `usuarios.c`: autenticação e gerenciamento de usuários.
- `projetos.c`: gerenciamento de projetos.
- `tarefas.c`: gerenciamento de tarefas e quadro.
- `estoque.c`: filamentos e movimentações.
- `relatorios.c`: relatórios e diário de bordo.
