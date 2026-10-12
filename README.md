# PIM 3D Manager

Desenvolvido para o trabalho de final de bimestre, o Projeto Integrador Multidisciplinar (PIM) do curso Analise e Desenvolvimento de Sistemas. O software centraliza:

- **Controle de Acesso:** Permissões por níveis de usuário (Membro e Gerente).
- **Gestão de Projetos e Tarefas:** Acompanhamento de fluxo de trabalho.
- **Controle de Estoque:** Registro de entradas, saídas e rastreio de perdas de matéria-prima.
- **Relatórios e Histórico:** Relatórios gerenciais e diário de bordo.

### 1. Compile o projeto

Copie e execute este comando:

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

Esse comando reúne os arquivos do sistema e gera o executável chamado main.

### 2. Abra o sistema
Se a compilação terminar sem erros, execute:
./main

O programa deverá iniciar no terminal.

### 3. Se aparecer um erro
Se o terminal informar que gcc não foi encontrado, instale o compilador com:

#### Windows:

```cmd
winget install w64devkit

```

#### Ubuntu/Debian:

```bash
sudo apt update && sudo apt install build-essential

```

#### Redhat/Fedora:
```bash
sudo dnf install gcc

```

#### Usando o Code::Blocks:

Basta abrir o projeto `.cbp` no Code::Blocks e apertar **F9**.  

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
