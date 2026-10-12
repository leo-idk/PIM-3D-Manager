#include "usuarios.h"

int autenticar_admin(void)
{
    char email[TAMANHO_EMAIL];
    char senha[TAMANHO_SENHA];

    limpar_tela();

    printf("Confirme nivel de acesso.\n\n");

    printf("E-mail: ");
    fgets(email, sizeof(email), stdin);

    email[strcspn(email, "\n")] = '\0';

    if(strcmp(email, "0") == 0)
        return 0;

    printf("Senha: ");
    ler_senha(senha, sizeof(senha));

    if(strcmp(senha, "0") == 0)
        return 0;

    if(strcmp(email, EMAIL_ADMIN) == 0 &&
       strcmp(senha, SENHA_ADMIN) == 0)
    {
        return 1;
    }

    FILE *arquivo = abrirArquivo("usuarios.dat", "rb");
    Usuario usuario;

    if(arquivo != NULL)
    {
        while(fread(&usuario, sizeof(Usuario), 1, arquivo))
        {
            if(strcmp(usuario.email, email) == 0 &&
               strcmp(usuario.password, senha) == 0 &&
               usuario.estado_usuario == 1 &&
               usuario.nivel_acesso == GERENTE)
            {
                fclose(arquivo);
                return 1;
            }
        }

        fclose(arquivo);
    }

    printf("\nE-mail ou senha incorretos.\n");

    return -1;
}

void ler_senha(char *senha, int tamanho) {
    int i = 0;
    char caractere;

#ifdef _WIN32
    while (1) {
        caractere = _getch(); // Lê o caractere sem ecoar na tela no Windows

        if (caractere == '\r' || caractere == '\n') {
            break;
        }
        if (caractere == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        if (i < tamanho - 1) {
            senha[i] = caractere;
            i++;
            printf("*");
            fflush(stdout);
        }
    }
    senha[i] = '\0';
    printf("\n");

#else
    struct termios antigo;
    struct termios novo;

    tcgetattr(STDIN_FILENO, &antigo);
    novo = antigo;
    novo.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &novo);

    while (1) {
        caractere = getchar();

        if (caractere == '\n' || caractere == EOF) {
            break;
        }

        if (i < tamanho - 1) {
            senha[i] = caractere;
            i++;

            printf("*");
            fflush(stdout);
        }
    }

    senha[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
    printf("\n");
#endif
}

void tela_de_login(void)
{
    int menu_login;

    do
    {
        limpar_tela();

        printf("Login.\n\n");

        printf("1 - Entrar\n");
        printf("0 - Sair do sistema\n");

        menu_login = ler_num_inteiro("\nEscolha: ");

        switch(menu_login)
        {
            case 1:
            {
                char email[TAMANHO_EMAIL];
                char senha[TAMANHO_SENHA];
                while(1)
                {
                    limpar_tela();

                    printf("Login\n\n");

                    printf("E-mail: ");
                    fgets(email, sizeof(email), stdin);

                    if(strchr(email, '\n') == NULL)
                    {
                        while(getchar() != '\n');
                    }

                    email[strcspn(email, "\n")] = '\0';

                    if(strcmp(email, "0") == 0)
                    {
                        break;
                    }

                    printf("Senha: ");
                    ler_senha(senha, sizeof(senha));

                    if(strcmp(senha, "0") == 0)
                    {
                        break;
                    }

                    if(strcmp(email, EMAIL_ADMIN) == 0 &&
                       strcmp(senha, SENHA_ADMIN) == 0)
                    {
                        id_usuario_logado = 0;
                        nivel_usuario_logado = GERENTE;

                        limpar_tela();

                        printf("Login realizado com sucesso!\n");
                        printf("Bem-vindo, Administrador!\n");

                        menu_inicial();

                        return;
                    }

                    int encontrado = 0;

                    FILE *arquivo = abrirArquivo("usuarios.dat", "rb");

                    if(arquivo == NULL)
                    {
                        printf("\nNenhum usuario cadastrado.\n");
                        aguardar_enter();
                        break;
                    }

                    Usuario usuario;

                    while(fread(&usuario, sizeof(Usuario), 1, arquivo))
                    {
                        if(strcmp(usuario.email, email) == 0 &&
                           strcmp(usuario.password, senha) == 0 &&
                           usuario.estado_usuario == 1)
                        {
                            encontrado = 1;

                            id_usuario_logado = usuario.id_usuario;
                            nivel_usuario_logado = usuario.nivel_acesso;

                            limpar_tela();

                            printf("Login realizado com sucesso!\n");
                            printf("Bem-vindo, %s!\n",
                                   usuario.nome_do_usuario);

                            fclose(arquivo);

                            menu_inicial();

                            return;
                        }
                    }

                    fclose(arquivo);

                    if(encontrado == 0)
                    {
                        limpar_tela();

                        printf("Login\n\n");
                        printf("E-mail ou senha incorretos.\n");

                        aguardar_enter();
                }   }

                break;
            }
            case 0:
                if(confirmar_saida())
                {
                    return;
                }

                break;

            default:
                limpar_tela();
                printf("Opcao invalida.\n");
                aguardar_enter();

                break;
        }

    } while(menu_login != 0);
}

void tela_de_cadastro(void) {

    Usuario novo_usuario = {0};

    while (1) {

        limpar_tela();

        printf("Cadastro\n\n");

        if(!ler_texto_obrigatorio(
            "Nome: ",
            novo_usuario.nome_do_usuario,
            TAMANHO_NOME))
        {
            return;
        }

        while (1) {

            if(!ler_texto_obrigatorio(
                "E-mail: ",
                novo_usuario.email,
                TAMANHO_EMAIL))
            {
                return;
            }

            if (strchr(novo_usuario.email, '@') != NULL &&
                strchr(strchr(novo_usuario.email, '@') + 1, '.') != NULL) {

                break;
            }

            limpar_tela();
            printf("Cadastro\n\n");
            printf("Nome: %s\n\n", novo_usuario.nome_do_usuario);
            printf("E-mail invalido. Tente novamente.\n\n");
        }

        char senha_entrada[100];

        while (1) {

            printf("Senha: ");
            ler_senha(senha_entrada, sizeof(senha_entrada));

            if(strcmp(senha_entrada, "0") == 0)
            {
                return;
            }

            if (strlen(senha_entrada) >= 6 &&
                strlen(senha_entrada) < TAMANHO_SENHA) {

                strcpy(novo_usuario.password, senha_entrada);
                break;
            }

            limpar_tela();

            printf("Cadastro\n\n");
            printf("Nome: %s\n\n", novo_usuario.nome_do_usuario);
            printf("E-mail: %s\n\n", novo_usuario.email);
            printf("Senha deve ter pelo menos 6 caracteres e no maximo %d caracteres. Tente novamente.\n\n",
                   TAMANHO_SENHA - 1);
        }

        FILE *arquivo = abrirArquivo("usuarios.dat", "rb");
        Usuario usuario;
        int encontrado = 0;

        if(arquivo != NULL)
        {
            while(fread(&usuario, sizeof(Usuario), 1, arquivo))
            {
                if(usuario.estado_usuario == 1 &&
                    (strcmp(usuario.nome_do_usuario,
                            novo_usuario.nome_do_usuario) == 0 ||
                        strcmp(usuario.email,
                            novo_usuario.email) == 0))
                    {
                        encontrado = 1;
                        break;
                 }
            }

            fclose(arquivo);
        }

        if(encontrado)
        {
            limpar_tela();

            printf("Cadastro\n\n");
            printf("Nome ou e-mail ja cadastrado.\n");
            printf("Cadastro cancelado.\n");

            aguardar_enter();
            continue;
        }
        while(1)
        {
            printf("\nNivel de acesso:\n");
            printf("1 - Membro\n");
            printf("2 - Gerente de Projetos\n");

            int nivel = ler_num_inteiro("\nEscolha: ");

            if(nivel == MEMBRO || nivel == GERENTE)
            {
                novo_usuario.nivel_acesso = nivel;
                break;
            }

            printf("Nivel de acesso invalido. Tente novamente.\n");
        }
        novo_usuario.id_usuario = prox_id_usuario++;
        novo_usuario.estado_usuario = 1;

        arquivo = abrirArquivo("usuarios.dat", "ab");

        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            aguardar_enter();
            return;
        }

        fwrite(&novo_usuario, sizeof(Usuario), 1, arquivo);
        fclose(arquivo);
        limpar_tela();

        printf("Usuario cadastrado com sucesso! (ID: %d)\n",
               novo_usuario.id_usuario);

        aguardar_enter();

        return;
    }
}

int listar_usuarios(void)
{
    FILE *arquivo = abrirArquivo("usuarios.dat", "rb");
    Usuario usuario;
    int encontrou = 0;

    if(arquivo == NULL)
        return 0;

    printf("Usuarios disponiveis:\n\n");

    while(fread(&usuario, sizeof(Usuario), 1, arquivo))
    {
        if(usuario.estado_usuario == 1)
        {
            printf("[ID: %d] %s\n",
                   usuario.id_usuario,
                   usuario.nome_do_usuario);

            encontrou = 1;
        }
    }

    fclose(arquivo);
    return encontrou;
}

void editar_usuario(void)
{
    Usuario usuario;
    long posicao;
    int opcao;

    if(!selecionar_item("usuario", "editar", &usuario, &posicao, 0))
        return;

    FILE *arquivo = abrirArquivo("usuarios.dat", "rb+");

    if(arquivo == NULL)
        return;

    do
    {
        limpar_tela();

        printf("Edicao Usuario [ID: %d]\n\n", usuario.id_usuario);

        printf("Nome: %s\n", usuario.nome_do_usuario);
        printf("E-mail: %s\n", usuario.email);
        printf("Senha: ");

        for(int i = 0; usuario.password[i] != '\0'; i++)
        {
            printf("*");
        }

        printf("\n\n");

        printf("Selecione o campo que deseja alterar:\n");
        printf("1 - Nome\n");
        printf("2 - E-mail\n");
        printf("3 - Senha\n");
        printf("4 - Salvar\n");
        printf("0 - Cancelar\n");

        opcao = ler_num_inteiro("\nEscolha: ");

        switch(opcao)
        {
            case 1:
{
                char novo_nome[TAMANHO_NOME];

                if(ler_texto_obrigatorio(
                    "Novo Nome: ",
                    novo_nome,
                    TAMANHO_NOME))
                {
                    FILE *arquivo_nomes = abrirArquivo("usuarios.dat", "rb");
                    Usuario outro_usuario;
                    int nome_existe = 0;

                    if(arquivo_nomes != NULL)
                    {
                        while(fread(&outro_usuario,
                                    sizeof(Usuario),
                                    1,
                                    arquivo_nomes))
                        {
                            if(outro_usuario.estado_usuario == 1 &&
                            outro_usuario.id_usuario != usuario.id_usuario &&
                            strcmp(outro_usuario.nome_do_usuario,
                                    novo_nome) == 0)
                            {
                                nome_existe = 1;
                                break;
                            }
                        }

                        fclose(arquivo_nomes);
                    }

                    if(nome_existe)
                    {
                        printf("\nNome ja cadastrado.\n");
                        aguardar_enter();
                    }
                    else
                    {
                        strcpy(usuario.nome_do_usuario, novo_nome);
                    }
                }

                break;
            }
            case 2:
            {
                char novo_email[TAMANHO_EMAIL];

                if(ler_texto_obrigatorio(
                    "Novo E-mail: ",
                    novo_email,
                    TAMANHO_EMAIL))
                {
                    if(strchr(novo_email, '@') != NULL &&
                    strchr(strchr(novo_email, '@') + 1, '.') != NULL)
                    {
                        FILE *arquivo_emails = abrirArquivo("usuarios.dat", "rb");
                        Usuario outro_usuario;
                        int email_existe = 0;

                        if(arquivo_emails != NULL)
                        {
                            while(fread(&outro_usuario,
                                        sizeof(Usuario),
                                        1,
                                        arquivo_emails))
                            {
                                if(outro_usuario.estado_usuario == 1 &&
                                outro_usuario.id_usuario != usuario.id_usuario &&
                                strcmp(outro_usuario.email,
                                        novo_email) == 0)
                                {
                                    email_existe = 1;
                                    break;
                                }
                            }

                            fclose(arquivo_emails);
                        }

                        if(email_existe)
                        {
                            printf("\n\nE-mail ja cadastrado.\n\n");
                            aguardar_enter();
                        }
                        else
                        {
                            strcpy(usuario.email, novo_email);
                        }
                    }
                    else
                    {
                        printf("\nE-mail invalido.\n");
                        aguardar_enter();
                    }
                }

                break;
            }
           case 3:
            {
                char nova_senha[100];

                printf("Nova Senha: ");
                ler_senha(nova_senha, sizeof(nova_senha));

                if(strcmp(nova_senha, "0") == 0)
                {
                    break;
                }

                if(strlen(nova_senha) >= 6 &&
                strlen(nova_senha) < TAMANHO_SENHA)
                {
                    strcpy(usuario.password, nova_senha);
                }
                else
                {
                    printf(
                        "\n\nSenha deve ter pelo menos 6 caracteres "
                        "e no maximo %d caracteres.\n\n",
                        TAMANHO_SENHA - 1
                    );

                    aguardar_enter();
                }

                break;
            }
            case 4:

                salvar_registro(arquivo, &usuario, sizeof(Usuario), posicao);

                limpar_tela();

                printf("Edicao concluida com sucesso!\n");

                aguardar_enter();

                fclose(arquivo);

                return;

            case 0:

                limpar_tela();
                fclose(arquivo);

                return;

            default:

                printf("\nOpcao invalida.\n");
                aguardar_enter();

                break;
        }

    } while(1);
}

void consultar_usuario(void)
{
    Usuario usuario;

    if(!selecionar_item("usuario", "consultar", &usuario, NULL, 0))
        return;

    limpar_tela();

    printf("Consulta Usuario [ID: %d]\n\n", usuario.id_usuario);

    printf("Nome: %s\n", usuario.nome_do_usuario);
    printf("E-mail: %s\n", usuario.email);
    printf("Senha: ");

    for(int i = 0; usuario.password[i] != '\0'; i++)
    {
        printf("*");
    }

    printf("\n");

    aguardar_enter();
}

void excluir_usuario(void)
{
    Usuario usuario;
    long posicao;

    while(1)
    {
        //limpar_tela();

        if(!selecionar_item("usuario", "excluir", &usuario, &posicao, 0))
            return;

        limpar_tela();

        printf("Excluir Usuario\n\n");

        printf("ID: %d\n", usuario.id_usuario);
        printf("Nome: %s\n", usuario.nome_do_usuario);
        printf("E-mail: %s\n\n", usuario.email);

        int confirmacao = ler_num_inteiro(
            "Deseja realmente excluir este usuario? (1 - Sim / 0 - Nao): "
        );

        if(confirmacao != 1)
        {
            return;
        }

        FILE *arquivo_projetos = abrirArquivo("projetos.dat", "rb");

        if(arquivo_projetos != NULL)
        {
            Projeto projeto;
            int usuario_vinculado = 0;

            while(fread(&projeto,
                        sizeof(Projeto),
                        1,
                        arquivo_projetos))
            {
                if(projeto.estado_projeto == 1)
                {
                    for(int i = 0;
                        i < projeto.quantidade_usuarios;
                        i++)
                    {
                        if(projeto.id_usuarios[i] == usuario.id_usuario)
                        {
                            usuario_vinculado = 1;
                            break;
                        }
                    }
                }

                if(usuario_vinculado)
                    break;
            }

            fclose(arquivo_projetos);

            if(usuario_vinculado)
            {
                limpar_tela();

                printf("Nao e possivel excluir este usuario.\n");
                printf("O usuario esta vinculado a um projeto ativo.\n");

                aguardar_enter();
                return;
            }
        }

        usuario.estado_usuario = 0;

        FILE *arquivo = abrirArquivo("usuarios.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("\nErro ao abrir arquivo.\n");
            aguardar_enter();
            return;
        }

        salvar_registro(
            arquivo,
            &usuario,
            sizeof(Usuario),
            posicao
        );

        fclose(arquivo);

    }
}

void menu_usuarios(void)
{
    int opcao;

    do
    {
        limpar_tela();

        printf("Gerenciamento de Usuarios\n\n");

        printf("1 - Cadastrar Usuario\n");
        printf("2 - Consultar Usuario\n");
        printf("3 - Editar Usuario\n");
        printf("4 - Excluir Usuario\n");
        printf("0 - Voltar\n");

        opcao = ler_num_inteiro("\nEscolha: ");

        switch(opcao)
        {
            case 1:
                tela_de_cadastro();
                break;

            case 2:
                consultar_usuario();
                break;

            case 3:
                editar_usuario();
                break;

            case 4:
                excluir_usuario();
                break;

            case 0:
                break;

            default:
                printf("\nOpcao invalida.\n");
                aguardar_enter();
                break;
        }

    } while(opcao != 0);
}