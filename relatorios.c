#include "relatorios.h"

void menu_relatorios(void)
{
    int opcao;

    do
    {
        limpar_tela();

        printf("============== RELATORIOS ==============\n\n");
        printf("1 - Relatorio Geral\n");
        printf("2 - Diario de Bordo\n");
        printf("0 - Voltar\n");

        opcao = ler_num_inteiro("\nEscolha: ");

        switch(opcao)
        {
            case 1:
                relatorio_geral();
                break;

            case 2:
                diario_de_bordo();
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

void diario_de_bordo(void)
{
    FILE *arquivo;
    FILE *arquivo_usuario;

    Projeto projeto;
    Tarefa tarefa;
    Movimentacao movimentacao;
    Usuario usuario;

    struct tm *data_formatada;

    int encontrou;

    limpar_tela();

    printf("================ DIARIO DE BORDO ================\n\n");


    /* ================= PROJETOS ================= */

    printf("---------------- PROJETOS ----------------\n\n");

    arquivo = abrirArquivo("projetos.dat", "rb");

    encontrou = 0;

    if(arquivo != NULL)
    {
        while(fread(&projeto,
                    sizeof(Projeto),
                    1,
                    arquivo))
        {
            if(projeto.estado_projeto != 1)
                continue;

            encontrou = 1;

            printf("[ID: %d] %s\n",
                   projeto.id_projeto,
                   projeto.nome_do_projeto);

            data_formatada =
                localtime(&projeto.data_criacao);

            if(data_formatada != NULL)
            {
                printf("Criado em: %02d/%02d/%04d %02d:%02d\n",
                       data_formatada->tm_mday,
                       data_formatada->tm_mon + 1,
                       data_formatada->tm_year + 1900,
                       data_formatada->tm_hour,
                       data_formatada->tm_min);
            }

            printf("Status: %s\n",
                   projeto.status_proj);

            printf("\n");
        }

        fclose(arquivo);
    }

    if(!encontrou)
        printf("Nenhum projeto cadastrado.\n");


    /* ================= TAREFAS ================= */

    printf("\n---------------- TAREFAS ----------------\n\n");

    arquivo = abrirArquivo("tarefas.dat", "rb");

    encontrou = 0;

    if(arquivo != NULL)
    {
        while(fread(&tarefa,
                    sizeof(Tarefa),
                    1,
                    arquivo))
        {
            if(tarefa.estado_tarefa != 1)
                continue;

            encontrou = 1;

            printf("[ID: %d] %s\n",
                   tarefa.id_tarefa,
                   tarefa.nome);

            printf("Projeto: %d\n",
                   tarefa.id_projeto);

            arquivo_usuario =
                abrirArquivo("usuarios.dat", "rb");

            if(arquivo_usuario != NULL)
            {
                int encontrou_usuario = 0;

                while(fread(&usuario,
                            sizeof(Usuario),
                            1,
                            arquivo_usuario))
                {
                    if(usuario.id_usuario ==
                           tarefa.id_usuario_responsavel &&
                       usuario.estado_usuario == 1)
                    {
                        printf("Responsavel: %s\n",
                               usuario.nome_do_usuario);

                        encontrou_usuario = 1;
                        break;
                    }
                }

                fclose(arquivo_usuario);

                if(!encontrou_usuario)
                    printf("Responsavel: Nao encontrado\n");
            }
            else
            {
                printf("Responsavel: Nao encontrado\n");
            }

            data_formatada =
                localtime(&tarefa.data_criacao);

            if(data_formatada != NULL)
            {
                printf("Criada em: %02d/%02d/%04d %02d:%02d\n",
                       data_formatada->tm_mday,
                       data_formatada->tm_mon + 1,
                       data_formatada->tm_year + 1900,
                       data_formatada->tm_hour,
                       data_formatada->tm_min);
            }

            printf("Status: %s\n",
                   tarefa.status);

            printf("\n");
        }

        fclose(arquivo);
    }

    if(!encontrou)
        printf("Nenhuma tarefa cadastrada.\n");


    /* ================= MOVIMENTACOES ================= */

    printf("\n------------- ATIVIDADES DE ESTOQUE -------------\n\n");

    arquivo = abrirArquivo("movimentacoes.dat", "rb");

    encontrou = 0;

    if(arquivo != NULL)
    {
        while(fread(&movimentacao,
                    sizeof(Movimentacao),
                    1,
                    arquivo))
        {
            if(movimentacao.estado_movimentacao != 1)
                continue;

            encontrou = 1;

            data_formatada =
                localtime(&movimentacao.data_movimentacao);

            if(data_formatada != NULL)
            {
                printf("%02d/%02d/%04d | %02d:%02d - ",
                       data_formatada->tm_mday,
                       data_formatada->tm_mon + 1,
                       data_formatada->tm_year + 1900,
                       data_formatada->tm_hour,
                       data_formatada->tm_min);
            }

            arquivo_usuario =
                abrirArquivo("usuarios.dat", "rb");

            if(arquivo_usuario != NULL)
            {
                while(fread(&usuario,
                            sizeof(Usuario),
                            1,
                            arquivo_usuario))
                {
                    if(usuario.id_usuario ==
                           movimentacao.id_usuario &&
                       usuario.estado_usuario == 1)
                    {
                        printf("%s - ",
                               usuario.nome_do_usuario);
                        break;
                    }
                }

                fclose(arquivo_usuario);
            }

            printf("%s",
                   movimentacao.tipo);

            if(movimentacao.id_projeto != 0)
            {
                printf(" - Projeto %d",
                       movimentacao.id_projeto);
            }

            printf("\n");
        }

        fclose(arquivo);
    }

    if(!encontrou)
        printf("Nenhuma atividade de estoque registrada.\n");

    printf("\n");

    aguardar_enter();
}

void relatorio_geral(void)
{
    FILE *arquivo;

    Projeto projeto;
    Tarefa tarefa;
    Movimentacao movimentacao;

    int total_projetos = 0;
    int projetos_concluidos = 0;
    int projetos_andamento = 0;
    int projetos_nao_iniciados = 0;

    int total_tarefas = 0;
    int tarefas_a_fazer = 0;
    int tarefas_fazendo = 0;
    int tarefas_feitas = 0;
    int tarefas_impedidas = 0;

    int total_entradas = 0;
    int total_saidas = 0;

    float filamento_consumido = 0;
    float desperdicio = 0;
    float custo_extra = 0;

    limpar_tela();

    printf("================ RELATORIO GERAL ================\n\n");

    /* PROJETOS */

    arquivo = abrirArquivo("projetos.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&projeto, sizeof(Projeto), 1, arquivo))
        {
            if(projeto.estado_projeto != 1)
                continue;

            total_projetos++;

            if(strcmp(projeto.status_proj,
                      MAPEAMENTO_STATUS_PROJ[0]) == 0)
            {
                projetos_nao_iniciados++;
            }
            else if(strcmp(projeto.status_proj,
                           MAPEAMENTO_STATUS_PROJ[1]) == 0)
            {
                projetos_andamento++;
            }
            else if(strcmp(projeto.status_proj,
                           MAPEAMENTO_STATUS_PROJ[2]) == 0)
            {
                projetos_concluidos++;
            }
        }

        fclose(arquivo);
    }

    printf("PROJETOS\n");
    printf("Total: %d\n", total_projetos);
    printf("Nao iniciados: %d\n", projetos_nao_iniciados);
    printf("Em desenvolvimento: %d\n", projetos_andamento);
    printf("Concluidos: %d\n\n", projetos_concluidos);
    printf("--------------------------------------------\n");


    /* TAREFAS */

    arquivo = abrirArquivo("tarefas.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&tarefa, sizeof(Tarefa), 1, arquivo))
        {
            if(tarefa.estado_tarefa != 1)
                continue;

            total_tarefas++;

            if(strcmp(tarefa.status,
                      MAPEAMENTO_STATUS[0]) == 0)
            {
                tarefas_a_fazer++;
            }
            else if(strcmp(tarefa.status,
                           MAPEAMENTO_STATUS[1]) == 0)
            {
                tarefas_fazendo++;
            }
            else if(strcmp(tarefa.status,
                           MAPEAMENTO_STATUS[2]) == 0)
            {
                tarefas_feitas++;
            }
            else if(strcmp(tarefa.status,
                           MAPEAMENTO_STATUS[3]) == 0)
            {
                tarefas_impedidas++;
            }
        }

        fclose(arquivo);
    }

    printf("TAREFAS\n");
    printf("Total: %d\n", total_tarefas);
    printf("A Fazer: %d\n", tarefas_a_fazer);
    printf("Fazendo: %d\n", tarefas_fazendo);
    printf("Feitas: %d\n", tarefas_feitas);
    printf("Impedidas: %d\n\n", tarefas_impedidas);
    printf("--------------------------------------------\n");

    /* MOVIMENTACOES */

    arquivo = abrirArquivo("movimentacoes.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(
            &movimentacao,
            sizeof(Movimentacao),
            1,
            arquivo))
        {
            if(movimentacao.estado_movimentacao != 1)
                continue;

            if(strcmp(movimentacao.tipo, "ENTRADA") == 0)
            {
                total_entradas++;
            }
            else if(strcmp(movimentacao.tipo, "SAIDA") == 0)
            {
                total_saidas++;

                filamento_consumido +=
                    movimentacao.quantidade;

                desperdicio +=
                    movimentacao.desperdicio;

                custo_extra +=
                    movimentacao.custo_extra;
            }
        }

        fclose(arquivo);
    }

    printf("ESTOQUE\n");
    printf("Entradas: %d\n", total_entradas);
    printf("Saidas: %d\n", total_saidas);
    printf("Filamento consumido: %.2f g\n",
           filamento_consumido);
    printf("Desperdicio: %.2f g\n",
           desperdicio);
    printf("Custo extra: R$ %.2f\n",
           custo_extra);
    printf("--------------------------------------------\n");

    aguardar_enter();
}