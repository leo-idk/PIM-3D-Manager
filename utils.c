#include "utils.h"

// Função para limpar a tela de forma portátil
void limpar_tela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper para pausar e aguardar o usuário pressionar Enter
void aguardar_enter(void)
{
    printf("\nPressione ENTER para continuar...");

    while(getchar() != '\n');
}

// Helper para leitura segura de números inteiros (buffer para digitalização de apenas numeros)
int ler_num_inteiro(const char *msg_prompt) {
    int valor_entrada;
    printf("%s", msg_prompt);
    while (scanf("%d", &valor_entrada) != 1) {
        while (getchar() != '\n'); // Limpa buffer
        printf("%s", msg_prompt);
    }
    while (getchar() != '\n'); // Limpa buffer após leitura do inteiro
    return valor_entrada;
}

float ler_num_real(const char *msg_prompt)
{
    float valor_entrada;

    while(1)
    {
        printf("%s", msg_prompt);

        if(scanf("%f", &valor_entrada) == 1)
        {
            while(getchar() != '\n');
            return valor_entrada;
        }

        while(getchar() != '\n');
        printf("Valor invalido. Tente novamente.\n");
    }
}

// Helper para leitura de textos obrigatórios com cancelamento (0)
int ler_texto_obrigatorio(const char *msg_prompt, char *texto_destino, int tamanho)
{
    char buffer_entrada[tamanho + 1];

    while(1)
    {
        printf("%s", msg_prompt);
        if(fgets(buffer_entrada, sizeof(buffer_entrada), stdin) == NULL)
            return 0;
        size_t len = strlen(buffer_entrada);
        if(len > 0 && buffer_entrada[len - 1] == '\n')
        {
            buffer_entrada[len - 1] = '\0';
            len--;
        }
        else
        {
            int caractere;
            while((caractere = getchar()) != '\n' && caractere != EOF);
            printf("Limite de %d caracteres excedido. Tente novamente.\n", tamanho - 1);
            continue;
        }
        if(strcmp(buffer_entrada, "0") == 0)
        {
            limpar_tela();
            printf("Acao cancelada. Voltando...\n");
            return 0;
        }
        int somente_espacos = 1;
        for(size_t i = 0; i < len; i++)
        {
            if(buffer_entrada[i] != ' ' &&
               buffer_entrada[i] != '\t')
            {
                somente_espacos = 0;
                break;
            }
        }
        if(somente_espacos)
        {
            printf("O campo nao pode ficar vazio.\n");
            continue;
        }
        strcpy(texto_destino, buffer_entrada);
        return 1;
    }
}

int selecionar_item(const char *tipo, const char *acao, void *item, long *posicao, int id_projeto)
{
    FILE *arquivo;
    int id;

    if(strcmp(tipo, "usuario") == 0)
    {
        Usuario *usuario = item;

        if(!listar_usuarios())
        {
            printf("Nenhum usuario para %s.\n", acao);
            aguardar_enter();
            return 0;
        }

        while(1)
        {
            id = ler_num_inteiro(
                "\nDigite o ID do usuario (0 - Voltar): "
            );

            if(id == 0)
                return 0;

            arquivo = abrirArquivo("usuarios.dat", "rb");

            if(arquivo == NULL)
            {
                printf("\nErro ao abrir arquivo.\n");
                aguardar_enter();
                return 0;
            }

            while(fread(usuario, sizeof(Usuario), 1, arquivo))
            {
                if(usuario->id_usuario == id &&
                   usuario->estado_usuario == 1)
                {
                    if(posicao != NULL)
                    {
                        *posicao = ftell(arquivo) - sizeof(Usuario);
                    }

                    fclose(arquivo);
                    return 1;
                }
            }

            fclose(arquivo);

            printf("\nUsuario nao encontrado. Tente novamente.\n");
        }
    }

    if(strcmp(tipo, "projeto") == 0)
    {
        Projeto *projeto = item;

        while(1)
        {

            if(!listar_projetos())
            {
                printf("Nenhum projeto cadastrado.\n");
                aguardar_enter();
                return 0;
            }

            id = ler_num_inteiro(
                "\nDigite o ID do projeto (0 - Voltar): "
            );

            if(id == 0)
                return 0;

            arquivo = abrirArquivo("projetos.dat", "rb");

            if(arquivo == NULL)
            {
                printf("\nErro ao abrir arquivo.\n");
                aguardar_enter();
                return 0;
            }

            while(fread(projeto, sizeof(Projeto), 1, arquivo))
            {
                if(projeto->id_projeto == id &&
                   projeto->estado_projeto == 1)
                {
                    if(posicao != NULL)
                    {
                        *posicao = ftell(arquivo) - sizeof(Projeto);
                    }

                    fclose(arquivo);
                    return 1;
                }
            }

            fclose(arquivo);

            printf("\nProjeto nao encontrado. Tente novamente.\n");
        }
    }

    if(strcmp(tipo, "tarefa") == 0)
    {
        Tarefa *tarefa = item;

        while(1)
        {

            if(!listar_tarefas(id_projeto))
            {
                printf("Nenhuma tarefa cadastrada.\n\n");
            }

            id = ler_num_inteiro(
                "\nDigite o ID da tarefa (0 - Voltar): "
            );

            if(id == 0)
                return 0;

            arquivo = abrirArquivo("tarefas.dat", "rb");

            if(arquivo == NULL)
            {
                printf("\nErro ao abrir arquivo.\n");
                aguardar_enter();
                return 0;
            }

            while(fread(tarefa, sizeof(Tarefa), 1, arquivo))
            {
                if(tarefa->id_tarefa == id &&
                    tarefa->estado_tarefa == 1 &&
                    (id_projeto == 0 || tarefa->id_projeto == id_projeto))
                {
                    if(posicao != NULL)
                    {
                        *posicao = ftell(arquivo) - sizeof(Tarefa);
                    }

                    fclose(arquivo);
                    return 1;
                }
            }

            fclose(arquivo);

            printf("\nTarefa nao encontrada. Tente novamente.\n");
        }
    }

    if(strcmp(tipo, "filamento") == 0)
    {
        Filamento *filamento = item;

        if(!listar_filamentos())
        {
            printf("Nenhum filamento para %s.\n", acao);
            aguardar_enter();
            return 0;
        }

        while(1)
        {
            id = ler_num_inteiro(
                "\nDigite o ID do filamento (0 - Voltar): "
            );

            if(id == 0)
                return 0;

            arquivo = abrirArquivo("filamentos.dat", "rb");

            if(arquivo == NULL)
            {
                printf("\nErro ao abrir arquivo.\n");
                aguardar_enter();
                return 0;
            }

            while(fread(filamento, sizeof(Filamento), 1, arquivo))
            {
                if(filamento->id_filamento == id &&
                   filamento->estado_filamento == 1)
                {
                    if(posicao != NULL)
                    {
                        *posicao = ftell(arquivo) - sizeof(Filamento);
                    }

                    fclose(arquivo);
                    return 1;
                }
            }

            fclose(arquivo);

            printf("\nFilamento nao encontrado. Tente novamente.\n");
        }
    }

    return 0;
}

int ler_prazo(char *prazo)
{
    int dia;
    int mes;
    int ano;

    while(1)
    {
        printf("Prazo (DD/MM/AAAA): ");

        if(scanf("%d", &dia) != 1)
        {
            while(getchar() != '\n');

            printf("Data invalida. Use o formato DD/MM/AAAA.\n");
            continue;
        }

        if(dia == 0)
        {
            while(getchar() != '\n');
            return 0;
        }

        if(scanf("/%d/%d", &mes, &ano) != 2)
        {
            while(getchar() != '\n');

            printf("Data invalida. Use o formato DD/MM/AAAA.\n");
            continue;
        }

        while(getchar() != '\n');

        if(dia < 1 || dia > 31 ||
           mes < 1 || mes > 12 ||
           ano < 2026)
        {
            printf("Data invalida. Tente novamente.\n");
            continue;
        }

        sprintf(
            prazo,
            "%02d/%02d/%04d",
            dia,
            mes,
            ano
        );

        return 1;
    }
}

FILE* abrirArquivo(const char *nomeArquivo, const char *modo)
{
    return fopen(nomeArquivo, modo);
}

int salvar_registro(FILE *arquivo, void *registro, size_t tamanho, long posicao)
{
    if(fseek(arquivo, posicao, SEEK_SET) != 0)
    {
        return 0;
    }

    return fwrite(registro, tamanho, 1, arquivo) == 1;
}

void mostrar_data_atual(void) {
    time_t agora = time(NULL);
    struct tm *data_atual = localtime(&agora);

    const char *dias_semana[] = {
        "domingo",
        "segunda-feira",
        "terca-feira",
        "quarta-feira",
        "quinta-feira",
        "sexta-feira",
        "sabado"
    };

    const char *meses[] = {
        "janeiro",
        "fevereiro",
        "marco",
        "abril",
        "maio",
        "junho",
        "julho",
        "agosto",
        "setembro",
        "outubro",
        "novembro",
        "dezembro"
    };

    printf("Bem vindo! Hoje é %s, %d de %s de %d às %02d:%02d.\n\n",
           dias_semana[data_atual->tm_wday],
           data_atual->tm_mday,
           meses[data_atual->tm_mon],
           data_atual->tm_year + 1900,
           data_atual->tm_hour,
           data_atual->tm_min);
}
