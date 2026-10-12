#include "estoque.h"

void menu_estoque(void)
{
    int opcao;

    do
    {
        limpar_tela();

        printf("=== SISTEMA DE ESTOQUE ===\n\n");

        printf("1 - Cadastrar Filamento\n");
        printf("2 - Consultar Filamento\n");
        printf("3 - Registrar Entrada\n");
        printf("4 - Registrar Saida\n");
        printf("5 - Alterar Localizacao\n");
        printf("6 - Excluir Filamento\n");
        printf("7 - Historico de Movimentacoes\n");
        printf("0 - Sair\n");

        opcao = ler_num_inteiro("\nDigite uma opcao: ");

        switch(opcao)
        {
            case 1:
                cadastrar_filamento();
                break;

            case 2:
                consultar_filamento();
                break;

            case 3:
                registrar_entrada();
                break;

            case 4:
                registrar_saida();
                break;

            case 5:
                alterar_localizacao_filamento();
                break;

            case 6:
                excluir_filamento();
                break;

            case 7:
                listar_movimentacoes();
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
                aguardar_enter();
                break;
        }

    } while(opcao != 0);
}

void cadastrar_filamento(void)
{
    Filamento filamento;
    FILE *arquivo;
    Filamento existente;

    limpar_tela();

    printf("\n1 - Cadastrar filamento\n");

    filamento.id_filamento = prox_id_filamento;

    if(!ler_texto_obrigatorio(
        "Digite um codigo de filamento: ",
        filamento.codigo,
        sizeof(filamento.codigo)))
        return;

    /*
     * Verifica se o codigo ja existe
     */
    arquivo = abrirArquivo("filamentos.dat", "rb");

    if(arquivo != NULL)
    {
        while(fread(&existente, sizeof(Filamento), 1, arquivo))
        {
            if(existente.estado_filamento == 1 &&
               strcmp(existente.codigo, filamento.codigo) == 0)
            {
                fclose(arquivo);

                printf("\nEsse codigo ja existe!\n");
                aguardar_enter();
                return;
            }
        }

        fclose(arquivo);
    }

    if(!ler_texto_obrigatorio(
        "Digite o tipo de filamento: ",
        filamento.tipo,
        sizeof(filamento.tipo)))
        return;

    if(!ler_texto_obrigatorio(
        "Digite o nome da marca: ",
        filamento.marca,
        sizeof(filamento.marca)))
        return;

    if(!ler_texto_obrigatorio(
        "Digite a cor do filamento: ",
        filamento.cor,
        sizeof(filamento.cor)))
        return;

    filamento.estoque_atual =
        ler_num_real("Digite o estoque atual: ");

    if(filamento.estoque_atual < 0)
    {
        printf("\nO estoque nao pode ser negativo.\n");
        aguardar_enter();
        return;
    }

    filamento.estoque_minimo =
        ler_num_real("Digite o estoque minimo do filamento: ");

    if(filamento.estoque_minimo < 0)
    {
        printf("\nO estoque minimo nao pode ser negativo.\n");
        aguardar_enter();
        return;
    }

    filamento.custo_atual =
        ler_num_real("Digite o custo medio do filamento: ");

    if(filamento.custo_atual < 0)
    {
        printf("\nO custo nao pode ser negativo.\n");
        aguardar_enter();
        return;
    }

    if(!ler_texto_obrigatorio(
        "Digite a localizacao do filamento: ",
        filamento.localizacao,
        sizeof(filamento.localizacao)))
        return;

    /*
     * Mantem o comportamento antigo de deixar
     * a localizacao em letras maiusculas.
     */
    {
        int i;

        for(i = 0; filamento.localizacao[i] != '\0'; i++)
        {
            filamento.localizacao[i] =
                toupper((unsigned char)filamento.localizacao[i]);
        }
    }

    filamento.estado_filamento = 1;

    arquivo = abrirArquivo("filamentos.dat", "ab");

    if(arquivo == NULL)
    {
        printf("\nErro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }

    if(fwrite(&filamento, sizeof(Filamento), 1, arquivo) != 1)
    {
        fclose(arquivo);

        printf("\nErro ao salvar filamento.\n");
        aguardar_enter();
        return;
    }

    fclose(arquivo);

    prox_id_filamento++;

    printf("\nFilamento cadastrado com sucesso!\n");
    printf("ID do filamento: %d\n", filamento.id_filamento);

    aguardar_enter();
}
//-------------------------------------------

int listar_filamentos(void)
{
    FILE *arquivo;
    Filamento filamento;
    int encontrou = 0;

    printf("\n=== FILAMENTOS CADASTRADOS ===\n\n");

    arquivo = abrirArquivo("filamentos.dat", "rb");

    if(arquivo == NULL)
        return 0;

    while(fread(
        &filamento,
        sizeof(Filamento),
        1,
        arquivo))
    {
        if(filamento.estado_filamento == 1)
        {
            encontrou = 1;

            printf("[ID: %d]\n",
                   filamento.id_filamento);

            printf("Codigo: %s\n",
                   filamento.codigo);

            printf("Tipo: %s\n",
                   filamento.tipo);

            printf("Quantidade: %.2f g\n",
                   filamento.estoque_atual);

            printf("Cor: %s\n",
                   filamento.cor);

            printf("Localizacao: %s\n\n",
                   filamento.localizacao);
        }
    }

    fclose(arquivo);

    return encontrou;
}

void consultar_filamento(void)
{
    Filamento filamento;
    long posicao;

    do
    {
        limpar_tela();

        printf("\n=== CONSULTAR FILAMENTO ===\n");

        if(!selecionar_item(
            "filamento",
            "consultar",
            &filamento,
            &posicao,
            0))
            return;

        limpar_tela();

        printf("\n=== DADOS DO FILAMENTO ===\n\n");

        printf("ID: %d\n",
               filamento.id_filamento);

        printf("Codigo: %s\n",
               filamento.codigo);

        printf("Tipo: %s\n",
               filamento.tipo);

        printf("Marca: %s\n",
               filamento.marca);

        printf("Cor: %s\n",
               filamento.cor);

        printf("Estoque atual: %.2f g\n",
               filamento.estoque_atual);

        printf("Estoque minimo: %.2f g\n",
               filamento.estoque_minimo);

        printf("Custo medio: R$ %.2f\n",
               filamento.custo_atual);

        printf("Localizacao: %s\n",
               filamento.localizacao);

        aguardar_enter();

    } while(1);
}

void registrar_entrada(void)
{
    Filamento filamento;
    Movimentacao movimentacao;
    long posicao;

    FILE *arquivo_filamento;
    FILE *arquivo_movimentacao;

    float quantidade_entrada;

    do
    {
        limpar_tela();

        printf("\n=== REGISTRAR ENTRADA ===\n");

        if(!selecionar_item(
            "filamento",
            "registrar entrada",
            &filamento,
            &posicao,
            0))
            return;

        /*
         * Mostra somente os dados do filamento selecionado
         */
        limpar_tela();

        printf("\n=== REGISTRAR ENTRADA ===\n\n");

        printf("Filamento selecionado\n");
        printf("Codigo: %s\n", filamento.codigo);
        printf("Tipo: %s\n", filamento.tipo);
        printf("Marca: %s\n", filamento.marca);
        printf("Cor: %s\n", filamento.cor);
        printf("Estoque atual: %.2f g\n",
               filamento.estoque_atual);

        quantidade_entrada =
            ler_num_real("\nDigite a quantidade que entrou: ");

        if(quantidade_entrada <= 0)
        {
            printf("\nA quantidade deve ser maior que zero!\n");
            aguardar_enter();
            continue;
        }

        /*
         * Atualiza o estoque
         */
        filamento.estoque_atual += quantidade_entrada;

        /*
         * Salva o filamento alterado
         */
        arquivo_filamento =
            abrirArquivo("filamentos.dat", "rb+");

        if(arquivo_filamento == NULL)
        {
            printf("\nErro ao abrir arquivo de filamentos.\n");
            aguardar_enter();
            continue;
        }

        if(!salvar_registro(
            arquivo_filamento,
            &filamento,
            sizeof(Filamento),
            posicao))
        {
            fclose(arquivo_filamento);

            printf("\nErro ao atualizar estoque.\n");
            aguardar_enter();
            continue;
        }

        fclose(arquivo_filamento);

        /*
         * Cria a movimentacao
         */
        memset(&movimentacao, 0, sizeof(Movimentacao));

        movimentacao.id_movimentacao =
            prox_id_movimentacao;

        movimentacao.id_filamento =
            filamento.id_filamento;

        movimentacao.id_projeto = 0;

        movimentacao.id_usuario =
            id_usuario_logado;

        strcpy(movimentacao.tipo, "ENTRADA");

        limpar_tela();

        printf("\n=== REGISTRAR ENTRADA ===\n\n");

        printf("Filamento: %s\n",
               filamento.codigo);

        printf("Quantidade: %.2f g\n",
               quantidade_entrada);

        if(!ler_texto_obrigatorio(
            "\nDigite o motivo da entrada: ",
            movimentacao.motivo_movimentacao,
            sizeof(movimentacao.motivo_movimentacao)))
        {
            continue;
        }

        movimentacao.quantidade =
            quantidade_entrada;

        movimentacao.data_movimentacao =
            time(NULL);

        movimentacao.estado_movimentacao = 1;

        /*
         * Salva a movimentacao
         */
        arquivo_movimentacao =
            abrirArquivo("movimentacoes.dat", "ab");

        if(arquivo_movimentacao == NULL)
        {
            printf(
                "\nEstoque atualizado, mas ocorreu um erro ao registrar a movimentacao.\n"
            );

            aguardar_enter();
            continue;
        }

        if(fwrite(
            &movimentacao,
            sizeof(Movimentacao),
            1,
            arquivo_movimentacao) != 1)
        {
            fclose(arquivo_movimentacao);

            printf(
                "\nEstoque atualizado, mas ocorreu um erro ao registrar a movimentacao.\n"
            );

            aguardar_enter();
            continue;
        }

        fclose(arquivo_movimentacao);

        prox_id_movimentacao++;

        limpar_tela();

        printf("\n=== ENTRADA REALIZADA ===\n\n");

        printf("Filamento: %s\n",
               filamento.codigo);

        printf("Quantidade: %.2f g\n",
               quantidade_entrada);

        printf("Estoque atual: %.2f g\n",
               filamento.estoque_atual);

        printf("ID da movimentacao: %d\n",
               movimentacao.id_movimentacao);

        if(filamento.estoque_atual <=
           filamento.estoque_minimo)
        {
            printf(
                "\nATENCAO: estoque abaixo ou igual ao minimo!\n"
            );
        }

        aguardar_enter();

    } while(1);
}

void registrar_saida(void)
{
    Filamento filamento;
    Projeto projeto;
    Movimentacao movimentacao;

    long posicao_filamento;
    long posicao_projeto;

    int opcao;
    int filamento_selecionado = 0;
    int projeto_selecionado = 0;
    int quantidade_informada = 0;

    FILE *arquivo_filamento;
    FILE *arquivo_movimentacao;

    limpar_tela();

    memset(&movimentacao, 0, sizeof(Movimentacao));

    movimentacao.id_movimentacao =
        prox_id_movimentacao;

    movimentacao.id_usuario =
        id_usuario_logado;

    movimentacao.data_movimentacao =
        time(NULL);

    strcpy(movimentacao.tipo, "SAIDA");

    movimentacao.estado_movimentacao = 1;

    limpar_tela();

    printf("\n=== REGISTRAR SAIDA ===\n");

    /*
     * Seleciona o filamento
     */
    if(!selecionar_item(
        "filamento",
        "registrar saida",
        &filamento,
        &posicao_filamento,
        0))
        return;

    filamento_selecionado = 1;

    limpar_tela();

    printf("\n=== REGISTRAR SAIDA ===\n\n");
    printf("Filamento: %s\n",
           filamento.codigo);

    if(!selecionar_item(
        "projeto",
        "registrar saida",
        &projeto,
        &posicao_projeto,
        0))
        return;

    projeto_selecionado = 1;

    limpar_tela();

    printf("\n=== REGISTRAR SAIDA ===\n\n");
    printf("Filamento: %s\n",
           filamento.codigo);
    printf("Projeto: %s\n",
           projeto.nome_do_projeto);

    movimentacao.quantidade =
        ler_num_real("\nDigite a quantidade que saiu: ");

    if(movimentacao.quantidade <= 0)
    {
        printf("\nA quantidade deve ser maior que zero!\n");
        aguardar_enter();
        return;
    }

    quantidade_informada = 1;

    /*
     * O peso real da peca corresponde
     * a quantidade informada na saida.
     */
    movimentacao.peso_real =
        movimentacao.quantidade;

    movimentacao.tem_peso_real = 1;

    movimentacao.id_filamento =
        filamento.id_filamento;

    movimentacao.id_projeto =
        projeto.id_projeto;

    if(movimentacao.quantidade >
       filamento.estoque_atual)
    {
        printf(
            "\nQuantidade de saida maior que o estoque disponivel!\n"
        );

        printf(
            "Estoque disponivel: %.2f g\n",
            filamento.estoque_atual
        );

        aguardar_enter();
        return;
    }

    /*
     * Menu de edicao da saida
     */
    do
    {
        calcular_detalhamento_saida(
            &movimentacao,
            &filamento
        );

        limpar_tela();

        printf("\n=== REGISTRAR SAIDA ===\n\n");

        printf("Filamento: %s\n",
               filamento.codigo);

        printf("Projeto: %s\n",
               projeto.nome_do_projeto);

        printf("Quantidade: %.2f g\n",
               movimentacao.quantidade);

        if(movimentacao.tem_perda ||
           movimentacao.tem_testes ||
           movimentacao.tem_suportes)
        {
            printf("\nDetalhamento:\n");

            if(movimentacao.tem_perda)
            {
                printf("Perda: %.2f g\n",
                       movimentacao.perda);
            }

            if(movimentacao.tem_testes)
            {
                printf("Testes: %.2f g\n",
                       movimentacao.testes);
            }

            if(movimentacao.tem_suportes)
            {
                printf("Suportes: %.2f g\n",
                       movimentacao.suportes);
            }

            /*
             * Mostra o desperdicio somente quando
             * houver mais de um tipo de desperdicio.
             */
            if((movimentacao.tem_perda +
                movimentacao.tem_testes +
                movimentacao.tem_suportes) > 1)
            {
                printf("Desperdicio total: %.2f g\n",
                       movimentacao.desperdicio);
            }

            printf("Custo extra: R$ %.2f\n",
                   movimentacao.custo_extra);
        }

        printf("\n");
        printf("1 - Alterar filamento\n");
        printf("2 - Alterar projeto\n");
        printf("3 - Alterar quantidade\n");
        printf("4 - Detalhar saida\n");
        printf("5 - Salvar saida\n");
        printf("0 - Cancelar\n");

        opcao = ler_num_inteiro("\nDigite uma opcao: ");

        switch(opcao)
        {
            case 1:
                if(selecionar_item(
                    "filamento",
                    "alterar filamento",
                    &filamento,
                    &posicao_filamento,
                    0))
                {
                    movimentacao.id_filamento =
                        filamento.id_filamento;

                    filamento_selecionado = 1;
                }
                break;
            case 2:

                if(selecionar_item(
                    "projeto",
                    "alterar projeto",
                    &projeto,
                    &posicao_projeto,
                    0))
                {
                    movimentacao.id_projeto =
                        projeto.id_projeto;

                    projeto_selecionado = 1;
                }
                break;
            case 3:
            {
                float nova_quantidade;

                nova_quantidade =
                    ler_num_real(
                        "\nDigite a nova quantidade: "
                    );

                if(nova_quantidade <= 0)
                {
                    printf(
                        "\nA quantidade deve ser maior que zero!\n"
                    );

                    aguardar_enter();
                }
                else
                {
                    movimentacao.quantidade =
                        nova_quantidade;

                    /*
                     * O peso real acompanha a quantidade da saida.
                     */
                    movimentacao.peso_real =
                        nova_quantidade;

                    movimentacao.tem_peso_real = 1;

                    quantidade_informada = 1;
                }

                break;
            }
            case 4:
                detalhar_saida(&movimentacao);
                break;
            case 5:

                if(!filamento_selecionado ||
                   !projeto_selecionado ||
                   !quantidade_informada)
                {
                    printf(
                        "\nFilamento, projeto e quantidade sao obrigatorios.\n"
                    );

                    aguardar_enter();
                    break;
                }

                if(movimentacao.quantidade >
                   filamento.estoque_atual)
                {
                    printf(
                        "\nQuantidade de saida maior que o estoque disponivel!\n"
                    );

                    printf(
                        "Estoque disponivel: %.2f g\n",
                        filamento.estoque_atual
                    );

                    aguardar_enter();
                    break;
                }

                limpar_tela();

                printf("\n=== REGISTRAR SAIDA ===\n\n");

                printf("Filamento: %s\n",
                       filamento.codigo);

                printf("Projeto: %s\n",
                       projeto.nome_do_projeto);

                printf("Quantidade: %.2f g\n",
                       movimentacao.quantidade);

                if(movimentacao.tem_perda)
                {
                    printf("Perda: %.2f g\n",
                           movimentacao.perda);
                }

                if(movimentacao.tem_testes)
                {
                    printf("Testes: %.2f g\n",
                           movimentacao.testes);
                }

                if(movimentacao.tem_suportes)
                {
                    printf("Suportes: %.2f g\n",
                           movimentacao.suportes);
                }

                if(movimentacao.tem_perda ||
                   movimentacao.tem_testes ||
                   movimentacao.tem_suportes)
                {
                    if((movimentacao.tem_perda +
                        movimentacao.tem_testes +
                        movimentacao.tem_suportes) > 1)
                    {
                        printf("Desperdicio total: %.2f g\n",
                               movimentacao.desperdicio);
                    }

                    printf("Custo extra: R$ %.2f\n",
                           movimentacao.custo_extra);
                }
                if(!ler_texto_obrigatorio(
                    "\nDigite o motivo da saida: ",
                    movimentacao.motivo_movimentacao,
                    sizeof(movimentacao.motivo_movimentacao)))
                {
                    break;
                }
                /*
                 * Atualiza os calculos antes de salvar
                 */
                calcular_detalhamento_saida(
                    &movimentacao,
                    &filamento
                );

                if(movimentacao.desperdicio >
                   movimentacao.quantidade)
                {
                    printf(
                        "\nO desperdicio nao pode ser maior que a quantidade da saida!\n"
                    );

                    aguardar_enter();
                    break;
                }
                /*
                 * Atualiza o estoque somente agora
                 */
                filamento.estoque_atual -=
                    movimentacao.quantidade;

                /*
                 * Salva o filamento alterado
                 */
                arquivo_filamento =
                    abrirArquivo(
                        "filamentos.dat",
                        "rb+"
                    );

                if(arquivo_filamento == NULL)
                {
                    printf(
                        "\nErro ao abrir arquivo de filamentos.\n"
                    );

                    aguardar_enter();
                    break;
                }

                if(!salvar_registro(
                    arquivo_filamento,
                    &filamento,
                    sizeof(Filamento),
                    posicao_filamento))
                {
                    fclose(arquivo_filamento);

                    printf(
                        "\nErro ao atualizar estoque.\n"
                    );

                    aguardar_enter();
                    break;
                }

                fclose(arquivo_filamento);

                /*
                 * Salva a movimentacao
                 */
                arquivo_movimentacao =
                    abrirArquivo(
                        "movimentacoes.dat",
                        "ab"
                    );

                if(arquivo_movimentacao == NULL)
                {
                    printf(
                        "\nEstoque atualizado, mas ocorreu um erro ao registrar a movimentacao.\n"
                    );

                    aguardar_enter();
                    break;
                }

                if(fwrite(
                    &movimentacao,
                    sizeof(Movimentacao),
                    1,
                    arquivo_movimentacao) != 1)
                {
                    fclose(arquivo_movimentacao);

                    printf(
                        "\nEstoque atualizado, mas ocorreu um erro ao registrar a movimentacao.\n"
                    );

                    aguardar_enter();
                    break;
                }

                fclose(arquivo_movimentacao);

                prox_id_movimentacao++;

                printf(
                    "\nSaida realizada com sucesso!\n"
                );

                printf(
                    "ID da movimentacao: %d\n",
                    movimentacao.id_movimentacao
                );

                printf(
                    "\nEstoque atual: %.2f g\n",
                    filamento.estoque_atual
                );

                if(filamento.estoque_atual <=
                   filamento.estoque_minimo)
                {
                    printf(
                        "\nATENCAO: estoque abaixo ou igual ao minimo!\n"
                    );
                }
                aguardar_enter();
                return;
            case 0:
                return;
            default:
                printf("\nOpcao invalida!\n");
                aguardar_enter();
                break;
        }

    } while(opcao != 0);
}

void detalhar_saida(Movimentacao *movimentacao)
{
    Movimentacao copia;
    int opcao;

    copia = *movimentacao;

    do
    {
        limpar_tela();

        printf("\n=== DETALHAR SAIDA ===\n\n");

        /*
         * O peso real e igual a quantidade da saida.
         * Ele nao pode ser alterado aqui.
         */
        printf(
            "Peso real da peca: %.2f g\n",
            copia.peso_real
        );

        if(copia.tem_perda)
            printf(
                "1 - Perda:                  %.2f g\n",
                copia.perda
            );
        else
            printf(
                "1 - Perda:                  Nao informado\n"
            );

        if(copia.tem_testes)
            printf(
                "2 - Quantidade em testes:   %.2f g\n",
                copia.testes
            );
        else
            printf(
                "2 - Quantidade em testes:   Nao informado\n"
            );

        if(copia.tem_suportes)
            printf(
                "3 - Quantidade em suportes: %.2f g\n",
                copia.suportes
            );
        else
            printf(
                "3 - Quantidade em suportes: Nao informado\n"
            );

        printf("\n");
        printf("4 - Limpar detalhamento\n");
        printf("5 - Voltar\n");
        printf("0 - Cancelar alteracoes\n");

        opcao = ler_num_inteiro(
            "\nDigite uma opcao: "
        );

        switch(opcao)
        {
            case 1:
            {
                float valor;

                valor =
                    ler_num_real(
                        "\nDigite a quantidade de perda: "
                    );

                if(valor < 0)
                {
                    printf(
                        "\nO valor nao pode ser negativo.\n"
                    );

                    aguardar_enter();
                }
                else if(valor == 0)
                {
                    copia.perda = 0;
                    copia.tem_perda = 0;
                }
                else
                {
                    copia.perda = valor;
                    copia.tem_perda = 1;
                }

                break;
            }

            case 2:
            {
                float valor;

                valor =
                    ler_num_real(
                        "\nDigite a quantidade utilizada em testes: "
                    );

                if(valor < 0)
                {
                    printf(
                        "\nO valor nao pode ser negativo.\n"
                    );

                    aguardar_enter();
                }
                else if(valor == 0)
                {
                    copia.testes = 0;
                    copia.tem_testes = 0;
                }
                else
                {
                    copia.testes = valor;
                    copia.tem_testes = 1;
                }

                break;
            }

            case 3:
            {
                float valor;

                valor =
                    ler_num_real(
                        "\nDigite a quantidade utilizada em suportes: "
                    );

                if(valor < 0)
                {
                    printf(
                        "\nO valor nao pode ser negativo.\n"
                    );

                    aguardar_enter();
                }
                else if(valor == 0)
                {
                    copia.suportes = 0;
                    copia.tem_suportes = 0;
                }
                else
                {
                    copia.suportes = valor;
                    copia.tem_suportes = 1;
                }

                break;
            }

            case 4:

                /*
                 * O peso real nao e limpo,
                 * pois corresponde a quantidade da saida.
                 */
                copia.perda = 0;
                copia.testes = 0;
                copia.suportes = 0;

                copia.tem_perda = 0;
                copia.tem_testes = 0;
                copia.tem_suportes = 0;

                copia.desperdicio = 0;
                copia.custo_extra = 0;

                printf(
                    "\nDetalhamento limpo.\n"
                );

                aguardar_enter();

                break;

            case 5:

                /*
                 * Mantem as alteracoes feitas.
                 */
                *movimentacao = copia;

                return;

            case 0:

                /*
                 * Cancela as alteracoes.
                 */
                return;

            default:

                printf("\nOpcao invalida!\n");
                aguardar_enter();

                break;
        }

    } while(opcao != 5);
}

void calcular_detalhamento_saida(
    Movimentacao *movimentacao,
    Filamento *filamento
)
{
    float desperdicio = 0;

    if(movimentacao->tem_perda)
        desperdicio += movimentacao->perda;

    if(movimentacao->tem_testes)
        desperdicio += movimentacao->testes;

    if(movimentacao->tem_suportes)
        desperdicio += movimentacao->suportes;

    if(movimentacao->tem_perda ||
       movimentacao->tem_testes ||
       movimentacao->tem_suportes)
    {
        movimentacao->desperdicio = desperdicio;

        if(filamento != NULL)
        {
            movimentacao->custo_extra =
                (desperdicio / 1000.0f) *
                filamento->custo_atual;
        }
    }
    else
    {
        movimentacao->desperdicio = 0;
        movimentacao->custo_extra = 0;
    }
}

void alterar_localizacao_filamento(void)
{
    Filamento filamento;
    long posicao;

    FILE *arquivo;

    do
    {
        limpar_tela();

        printf("\n=== ALTERAR LOCALIZACAO ===\n\n");

        if(!selecionar_item(
            "filamento",
            "alterar localizacao",
            &filamento,
            &posicao,
            0))
            return;

        limpar_tela();
        printf("\n=== ALTERAR LOCALIZACAO ===\n\n");

        printf("Localizacao atual: %s\n",
               filamento.localizacao);

        if(!ler_texto_obrigatorio(
            "\nDigite a nova localizacao (0 - Voltar): ",
            filamento.localizacao,
            sizeof(filamento.localizacao)))
        {
            continue;
        }

        {
            int i;

            for(i = 0;
                filamento.localizacao[i] != '\0';
                i++)
            {
                filamento.localizacao[i] =
                    toupper(
                        (unsigned char)
                        filamento.localizacao[i]
                    );
            }
        }

        arquivo = abrirArquivo("filamentos.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("\nErro ao abrir arquivo.\n");
            aguardar_enter();
            continue;
        }

        if(!salvar_registro(
            arquivo,
            &filamento,
            sizeof(Filamento),
            posicao))
        {
            fclose(arquivo);

            printf("\nErro ao alterar localizacao.\n");
            aguardar_enter();
            continue;
        }

        fclose(arquivo);
        printf("\nLocalizacao atualizada com sucesso!\n");
        aguardar_enter();
    } while(1);
}

void excluir_filamento(void)
{
    Filamento filamento;
    long posicao;

    FILE *arquivo;

    int confirmacao;

    do
    {
        limpar_tela();

        printf("\n=== EXCLUIR FILAMENTO ===\n");

        if(!selecionar_item(
            "filamento",
            "excluir",
            &filamento,
            &posicao,
            0))
            return;

        limpar_tela();

        printf("\n=== EXCLUIR FILAMENTO ===\n\n");

        printf("Filamento selecionado:\n");
        printf("ID: %d\n", filamento.id_filamento);
        printf("Codigo: %s\n", filamento.codigo);
        printf("Tipo: %s\n", filamento.tipo);
        printf("Marca: %s\n", filamento.marca);

        confirmacao = ler_num_inteiro(
            "\nDeseja realmente excluir? (1 - Sim / 0 - Nao): "
        );

        if(confirmacao != 1)
        {
            printf("\nExclusao cancelada.\n");
            aguardar_enter();
            continue;
        }

        filamento.estado_filamento = 0;

        arquivo = abrirArquivo("filamentos.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("\nErro ao abrir arquivo.\n");
            aguardar_enter();
            continue;
        }

        if(!salvar_registro(
            arquivo,
            &filamento,
            sizeof(Filamento),
            posicao))
        {
            fclose(arquivo);

            printf("\nErro ao excluir filamento.\n");
            aguardar_enter();
            continue;
        }

        fclose(arquivo);
        limpar_tela();
        printf("\nFilamento excluido com sucesso!\n");

        aguardar_enter();

    } while(1);
}
//===============================
void listar_movimentacoes(void)
{
    FILE *arquivo;
    FILE *arquivo_filamento;
    FILE *arquivo_usuario;
    FILE *arquivo_projeto;

    Movimentacao movimentacao;
    Filamento filamento;
    Usuario usuario;
    Projeto projeto;

    int encontrou = 0;
    int encontrou_filamento;
    int encontrou_usuario;
    int encontrou_projeto;

    struct tm *data_formatada;

    limpar_tela();

    printf("\n=== HISTORICO DE MOVIMENTACOES ===\n");

    arquivo = abrirArquivo(
        "movimentacoes.dat",
        "rb"
    );

    if(arquivo == NULL)
    {
        printf("\nNenhuma movimentacao registrada.\n");
        aguardar_enter();
        return;
    }

    while(fread(
        &movimentacao,
        sizeof(Movimentacao),
        1,
        arquivo))
    {
        if(movimentacao.estado_movimentacao != 1)
            continue;

        encontrou = 1;

        encontrou_filamento = 0;
        encontrou_usuario = 0;
        encontrou_projeto = 0;

        /*
         * Busca o filamento
         */
        arquivo_filamento =
            abrirArquivo("filamentos.dat", "rb");

        if(arquivo_filamento != NULL)
        {
            while(fread(
                &filamento,
                sizeof(Filamento),
                1,
                arquivo_filamento))
            {
                if(filamento.id_filamento ==
                       movimentacao.id_filamento &&
                   filamento.estado_filamento == 1)
                {
                    encontrou_filamento = 1;
                    break;
                }
            }

            fclose(arquivo_filamento);
        }

        /*
         * Busca o usuario
         */
        arquivo_usuario =
            abrirArquivo("usuarios.dat", "rb");

        if(arquivo_usuario != NULL)
        {
            while(fread(
                &usuario,
                sizeof(Usuario),
                1,
                arquivo_usuario))
            {
                if(usuario.id_usuario ==
                       movimentacao.id_usuario &&
                   usuario.estado_usuario == 1)
                {
                    encontrou_usuario = 1;
                    break;
                }
            }

            fclose(arquivo_usuario);
        }

        /*
         * Busca o projeto somente para saidas
         */
        if(strcmp(movimentacao.tipo, "SAIDA") == 0)
        {
            arquivo_projeto =
                abrirArquivo("projetos.dat", "rb");

            if(arquivo_projeto != NULL)
            {
                while(fread(
                    &projeto,
                    sizeof(Projeto),
                    1,
                    arquivo_projeto))
                {
                    if(projeto.id_projeto ==
                           movimentacao.id_projeto &&
                       projeto.estado_projeto == 1)
                    {
                        encontrou_projeto = 1;
                        break;
                    }
                }

                fclose(arquivo_projeto);
            }
        }

        printf("\n--------------------------------------------\n");
        printf("ID da movimentacao: %d\n",
               movimentacao.id_movimentacao);
        printf("--------------------------------------------\n");
        if(encontrou_filamento)
        {
            printf("Filamento: %s\n",
                   filamento.codigo);
        }

        printf("Tipo: %s\n",
               movimentacao.tipo);

        if(encontrou_usuario)
        {
            printf("Usuario: %s\n",
                   usuario.nome_do_usuario);
        }

        if(strcmp(movimentacao.tipo, "SAIDA") == 0 &&
           encontrou_projeto)
        {
            printf("Projeto: %s\n",
                   projeto.nome_do_projeto);
        }

        printf("Motivo: %s\n",
               movimentacao.motivo_movimentacao);

        data_formatada =
            localtime(&movimentacao.data_movimentacao);

        if(data_formatada != NULL)
        {
            printf(
                "Data: %02d/%02d/%04d | %02d:%02d:%02d\n",
                data_formatada->tm_mday,
                data_formatada->tm_mon + 1,
                data_formatada->tm_year + 1900,
                data_formatada->tm_hour,
                data_formatada->tm_min,
                data_formatada->tm_sec
            );
        }

        printf("Quantidade: %.2f g\n",
               movimentacao.quantidade);

        if(movimentacao.tem_peso_real)
        {
            printf(
                "Peso real: %.2f g\n",
                movimentacao.peso_real
            );
        }

        if(movimentacao.tem_perda)
        {
            printf(
                "Perda: %.2f g\n",
                movimentacao.perda
            );
        }

        if(movimentacao.tem_testes)
        {
            printf(
                "Testes: %.2f g\n",
                movimentacao.testes
            );
        }

        if(movimentacao.tem_suportes)
        {
            printf(
                "Suportes: %.2f g\n",
                movimentacao.suportes
            );
        }

        if(movimentacao.tem_perda ||
           movimentacao.tem_testes ||
           movimentacao.tem_suportes)
        {
            printf(
                "Desperdicio: %.2f g\n",
                movimentacao.desperdicio
            );

            printf(
                "Custo extra: R$ %.2f\n",
                movimentacao.custo_extra
            );
        }
    }

    fclose(arquivo);

    if(!encontrou)
    {
        printf(
            "\nNenhuma movimentacao registrada.\n"
        );
    }

    aguardar_enter();
}