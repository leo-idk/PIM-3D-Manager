#include "structs.h"

void menu_inicial(void) {
    int opcao_menu;

    do {
        limpar_tela();

        mostrar_data_atual();
        printf("Tela inicial\n\n");
        printf("1 - Projetos\n");
        printf("2 - Tarefas do dia\n");
        printf("3 - Estoque\n");
        printf("4 - Relatorios\n");
        printf("5 - Usuarios\n");
        printf("0 - Deslogar\n");

        opcao_menu = ler_num_inteiro("\nEscolha: ");

        switch (opcao_menu) {
            case 1:
            {
                if(nivel_usuario_logado == GERENTE)
                {
                    menu_projetos();
                }
                else
                {
                    int resultado = autenticar_admin();

                    if(resultado == 1)
                    {
                        menu_projetos();
                    }
                    else if(resultado == -1)
                    {
                        aguardar_enter();
                    }
                }

                break;
            }

            case 2:
                quadro_acompanhamento();
                break;

            case 3:
                menu_estoque();
                break;

            case 4:
            {
                if(nivel_usuario_logado == GERENTE)
                {
                    menu_relatorios();
                }
                else
                {
                    int resultado = autenticar_admin();

                    if(resultado == 1)
                    {
                        menu_relatorios();
                    }
                    else if(resultado == -1)
                    {
                        aguardar_enter();
                    }
                }

                break;
            }

            case 5:
            {
                char email[TAMANHO_EMAIL];
                char senha[TAMANHO_SENHA];

                limpar_tela();

                printf("Acesso ao Gerenciamento de Usuarios\n\n");

                printf("E-mail: ");
                fgets(email, sizeof(email), stdin);
                email[strcspn(email, "\n")] = '\0';

                if(strcmp(email, "0") == 0)
                    break;

                printf("Senha: ");
                ler_senha(senha, sizeof(senha));

                if(strcmp(senha, "0") == 0)
                    break;

                if(strcmp(email, EMAIL_ADMIN) == 0 &&
                   strcmp(senha, SENHA_ADMIN) == 0)
                {
                    menu_usuarios();
                }
                else
                {
                    printf("\nAcesso permitido somente ao administrador principal.\n");
                    aguardar_enter();
                }

                break;
            }
            case 0:
                if (confirmar_saida()) {
                    limpar_tela();
                    printf("Logout realizado com sucesso!\n");
                    aguardar_enter();
                    tela_de_login();
                    return;
                } else {
                    opcao_menu = -1;
                }
                break;

            default:
                limpar_tela();
                printf("Opcao invalida. Tente novamente.\n");
                aguardar_enter();
                break;
        }

    } while (opcao_menu != 0);
}

int confirmar_saida(void) {
    limpar_tela();
    printf("Deseja realmente sair do sistema? (1 - Sim / 0 - Não): ");
    int confirmacao = ler_num_inteiro("");
    return confirmacao == 1;
}

int main(void)
{
    FILE *arquivo;

    Usuario usuario;
    Projeto projeto;
    Tarefa tarefa;
    Filamento filamento;
    Movimentacao movimentacao;

    /* Verifica o maior ID dos usuarios */
    arquivo = abrirArquivo("usuarios.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&usuario, sizeof(Usuario), 1, arquivo))
        {
            if(usuario.id_usuario >= prox_id_usuario)
            {
                prox_id_usuario = usuario.id_usuario + 1;
            }
        }

        fclose(arquivo);
    }

    /* Verifica o maior ID dos projetos */
    arquivo = abrirArquivo("projetos.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&projeto, sizeof(Projeto), 1, arquivo))
        {
            if(projeto.id_projeto >= prox_id_projeto)
            {
                prox_id_projeto = projeto.id_projeto + 1;
            }
        }

        fclose(arquivo);
    }

    /* Verifica o maior ID das tarefas */
    arquivo = abrirArquivo("tarefas.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&tarefa, sizeof(Tarefa), 1, arquivo))
        {
            if(tarefa.id_tarefa >= prox_id_tarefa)
            {
                prox_id_tarefa = tarefa.id_tarefa + 1;
            }
        }

        fclose(arquivo);
    }

/* Verifica o maior ID dos filamentos */
arquivo = abrirArquivo("filamentos.dat", "rb");

if(arquivo != NULL)
{
    while(fread(&filamento, sizeof(Filamento), 1, arquivo))
    {
        if(filamento.id_filamento >= prox_id_filamento)
        {
            prox_id_filamento =
                filamento.id_filamento + 1;
        }
    }

    fclose(arquivo);
}

/* Verifica o maior ID das movimentacoes */
arquivo = abrirArquivo("movimentacoes.dat", "rb");

if(arquivo != NULL)
{
    while(fread(
        &movimentacao,
        sizeof(Movimentacao),
        1,
        arquivo))
    {
        if(movimentacao.id_movimentacao >= prox_id_movimentacao)
        {
            prox_id_movimentacao =
                movimentacao.id_movimentacao + 1;
        }
    }

    fclose(arquivo);
}

    tela_de_login();

    return 0;
}
