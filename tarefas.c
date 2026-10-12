#include "tarefas.h"

void quadro_acompanhamento(void)
{
    int opcao;

    do
    {
        limpar_tela();

        printf("Quadro de Acompanhamento\n");

        //listar_tarefas(0);

        printf("\n1 - Consultar tarefa\n");
        printf("2 - Alterar status\n");
        printf("3 - Alterar prioridade\n");
        printf("0 - Voltar\n");

        opcao = ler_num_inteiro("\nEscolha: ");

        switch(opcao)
        {
            case 1:
                consultar_tarefa(0);
                break;
            case 2:
                alterar_status(0);
                break;
            case 3:
                alterar_prioridade(0);
                break;
            case 0:
                break;

            default:
                printf("Opcao invalida.\n");
                aguardar_enter();
        }

    }while(opcao != 0);
}

int dependencia_concluida(int id_dependencia)
{
    FILE *arquivo;
    Tarefa tarefa;

    if(id_dependencia == 0)
        return 1;

    arquivo = abrirArquivo("tarefas.dat", "rb");

    if(arquivo == NULL)
        return 0;

    while(fread(&tarefa, sizeof(Tarefa), 1, arquivo))
    {
        if(tarefa.id_tarefa == id_dependencia &&
        tarefa.estado_tarefa == 1)
        {
            fclose(arquivo);

            if(strcmp(tarefa.status, MAPEAMENTO_STATUS[2]) == 0)
                return 1;

            return 0;
        }
    }

    fclose(arquivo);
    return 0;
}

//-----------------LISTAGEM------------------------

int listar_tarefas(int id_projeto)
{
    FILE *arquivo = abrirArquivo("tarefas.dat", "rb");
    Tarefa tarefa;
    Projeto projeto;
    Usuario usuario;

    int encontrou = 0;

    if(arquivo == NULL)
        return 0;

    printf("Tarefas cadastradas:\n\n");
    printf("-----------------------------\n");

    while(fread(&tarefa, sizeof(Tarefa), 1, arquivo))
    {
        if(tarefa.estado_tarefa == 1 &&
            (id_projeto == 0 || tarefa.id_projeto == id_projeto))
        {
            char nome_usuario[TAMANHO_NOME] = "Nao encontrado";

            // Busca o usuario responsavel
            FILE *arquivo_usuarios = abrirArquivo("usuarios.dat", "rb");

            if(arquivo_usuarios != NULL)
            {
                while(fread(&usuario, sizeof(Usuario), 1, arquivo_usuarios))
                {
                    if(usuario.id_usuario == tarefa.id_usuario_responsavel &&
                       usuario.estado_usuario == 1)
                    {
                        strcpy(nome_usuario, usuario.nome_do_usuario);
                        break;
                    }
                }

                fclose(arquivo_usuarios);
            }

            // Busca o projeto
            FILE *arquivo_projetos = abrirArquivo("projetos.dat", "rb");

            if(arquivo_projetos != NULL)
            {
                while(fread(&projeto, sizeof(Projeto), 1, arquivo_projetos))
                {
                    if(projeto.id_projeto == tarefa.id_projeto &&
                       projeto.estado_projeto == 1)
                    {
                        printf("Projeto: %s\n", projeto.nome_do_projeto);
                        printf("[ID: %d] %s\n", tarefa.id_tarefa, tarefa.nome);
                        printf("Responsavel: %s | Status: %s\n",
                            nome_usuario, tarefa.status);

                        if(tarefa.prioridade == -1)
                        {
                            printf("Prioridade: Nao definida\n");
                        }
                        else
                        {
                            printf("Prioridade: %s\n",
                                MAPEAMENTO_PRIORIDADE[tarefa.prioridade]);
                        }

                        printf("\n");
                        break; // O break fica aqui dentro, logo após achar o projeto
                    }
                }

                fclose(arquivo_projetos);
            }

            encontrou = 1;
        }
    }

    fclose(arquivo);

    return encontrou;
}

void cadastrar_tarefa(int id_projeto) {
    limpar_tela();

    printf("Cadastro. (Digite 0 para voltar)\n");
    Tarefa nova_tarefa = {0};

    nova_tarefa.id_projeto = id_projeto;

    if (!ler_texto_obrigatorio("\nNome: ", nova_tarefa.nome, TAMANHO_NOME)) return;
    if (!ler_texto_obrigatorio("Descricao: ", nova_tarefa.descricao_tf, TAMANHO_TEXTO)) return;

    Usuario usuario_responsavel;

    if(!selecionar_item("usuario", "definir responsavel", &usuario_responsavel, NULL, id_projeto))
    {
        return;
    }

    nova_tarefa.id_usuario_responsavel = usuario_responsavel.id_usuario;
    limpar_tela();

    printf("Cadastro de Tarefa\n\n");
    printf("Nome: %s\n", nova_tarefa.nome);
    printf("Descricao: %s\n", nova_tarefa.descricao_tf);
    printf("Usuario responsavel: %s\n", usuario_responsavel.nome_do_usuario);
    printf("\nTipo:\n");
    printf("1 - Modelagem\n");
    printf("2 - Impressao\n");
    printf("3 - Acabamento\n");
    printf("4 - Manutencao\n");

    int opcao_tipo;

    do {
        opcao_tipo = ler_num_inteiro("Escolha o tipo: ");

        if(opcao_tipo == 0)
        return;

        if (opcao_tipo < 1 || opcao_tipo > 5) {
            printf("Tipo invalido. Tente novamente.\n");
        }

    } while (opcao_tipo < 1 || opcao_tipo > 5);
    strcpy(nova_tarefa.tipo, MAPEAMENTO_TIPO[opcao_tipo - 1]);

    int opcao;

    nova_tarefa.id_tarefa_dependencia = 0;

    do
    {
        limpar_tela();

        printf("Cadastro. (Digite 0 para voltar).\n\n");
        printf("Nome: %s\n", nova_tarefa.nome);
        printf("Descricao: %s\n", nova_tarefa.descricao_tf);
        printf("Usuario responsavel: %s\n", usuario_responsavel.nome_do_usuario);
        printf("Tipo: %s\n\n", nova_tarefa.tipo);

        printf("1 - Escolher Tarefa Dependente\n");
        printf("2 - Criterio de aceitacao\n");
        printf("3 - Salvar\n");

        opcao = ler_num_inteiro("\nEscolha: ");

        switch(opcao)
        {
            case 1:
            {
                Tarefa dependencia;

                if(selecionar_item(
                    "tarefa",
                    "definir dependencia",
                    &dependencia,
                    NULL, id_projeto))
                {
                    nova_tarefa.id_tarefa_dependencia =
                        dependencia.id_tarefa;
                }

                break;
            }

            case 2:
                printf("\nCriterio de aceitacao: ");

                fgets(
                    nova_tarefa.criterio_aceitacao,
                    TAMANHO_TEXTO,
                    stdin
                );

                nova_tarefa.criterio_aceitacao[
                    strcspn(
                        nova_tarefa.criterio_aceitacao,
                        "\n"
                    )
                ] = '\0';

                break;

            case 3:
                break;

            case 0:
                return;

            default:
                printf("\nOpcao invalida.\n");
                aguardar_enter();
                break;
        }

   } while(opcao != 3);

    // Define status inicial padrão
    strcpy(nova_tarefa.status, MAPEAMENTO_STATUS[0]);
    nova_tarefa.prioridade = -1;
    nova_tarefa.id_tarefa = prox_id_tarefa++;

    nova_tarefa.estado_tarefa = 1;

    FILE *arquivo = abrirArquivo("tarefas.dat", "ab");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        aguardar_enter();
        return;
    }
    nova_tarefa.data_criacao = time(NULL);

    fwrite(&nova_tarefa, sizeof(Tarefa), 1, arquivo);
    fclose(arquivo);

    limpar_tela();
    printf("Tarefa cadastrada com sucesso! (ID: %d)\n", nova_tarefa.id_tarefa);
    aguardar_enter();
}

void consultar_tarefa(int id_projeto) {

    Tarefa tarefa;

    while(1)
    {
        //limpar_tela();

        if(!selecionar_item("tarefa", "consultar", &tarefa, NULL, id_projeto))
            return;

        limpar_tela();

        printf("Consulta Tarefa [ID: %d] \n", tarefa.id_tarefa);
        printf("Nome: %s\n", tarefa.nome);
        printf("Descricao: %s\n", tarefa.descricao_tf);

        FILE *arquivo_usuarios;
        Usuario usuario;

        arquivo_usuarios = abrirArquivo("usuarios.dat", "rb");

        if(arquivo_usuarios != NULL)
        {
            while(fread(&usuario,
                        sizeof(Usuario),
                        1,
                        arquivo_usuarios))
            {
                if(usuario.id_usuario == tarefa.id_usuario_responsavel &&
                usuario.estado_usuario == 1)
                {
                    printf("Usuario responsavel: %s\n", usuario.nome_do_usuario);
                    break;
                }
            }

            fclose(arquivo_usuarios);
        }
        printf("Status: %s\n", tarefa.status);

        if(tarefa.prioridade == -1)
        {
            printf("Prioridade: Nao definida\n");
        }
        else
        {
            printf("Prioridade: %s\n",
                   MAPEAMENTO_PRIORIDADE[tarefa.prioridade]);
        }
        printf("Tipo: %s\n", tarefa.tipo);
        printf("Criterio de aceitacao: %s\n", tarefa.criterio_aceitacao);

        aguardar_enter();
        return;
    }
}

//---------------------------------------------

void alterar_status(int id_projeto){
    //limpar_tela();

    Tarefa tarefa;
    long posicao;

    if(!selecionar_item("tarefa", "alterar status", &tarefa, &posicao, id_projeto))
        return;

    int opcao_status;

    limpar_tela();
    printf("Alterar Status [ID: %d]\n", tarefa.id_tarefa);
    printf("Status atual: %s\n", tarefa.status);

    do {
        printf("\nOpcoes de Status:\n");
        printf("1 - A Fazer\n");
        printf("2 - Fazendo\n");
        printf("3 - Concluido\n");
        printf("0 - Cancelar\n");

        opcao_status = ler_num_inteiro("Escolha o novo status: ");

        if(opcao_status == 1)
        {
            strcpy(tarefa.status, MAPEAMENTO_STATUS[0]);
        }

        else if(opcao_status == 2)
        {
            if(strcmp(tarefa.status, MAPEAMENTO_STATUS[3]) == 0)
            {
                limpar_tela();

                printf("A tarefa esta impedida.\n");
                printf("Use o menu Impedimento para retomar a tarefa.\n");

                aguardar_enter();
                return;
            }
            if(!dependencia_concluida(tarefa.id_tarefa_dependencia))
            {
                limpar_tela();

                printf("Tarefa bloqueada.\n");
                printf("A tarefa da qual ela depende ainda nao foi concluida.\n");

                aguardar_enter();
                return;
            }

            strcpy(tarefa.status, MAPEAMENTO_STATUS[1]);
        }

        else if(opcao_status == 3)
        {
            if(strcmp(tarefa.status, MAPEAMENTO_STATUS[3]) == 0)
            {
                limpar_tela();

                printf("A tarefa esta impedida.\n");
                printf("Use o menu Impedimento para retomar a tarefa antes de concluir.\n");

                aguardar_enter();
                return;
            }
            if(!dependencia_concluida(tarefa.id_tarefa_dependencia))
            {
                limpar_tela();

                printf("Nao e possivel concluir a tarefa.\n");
                printf("A tarefa da qual ela depende ainda nao foi concluida.\n");

                aguardar_enter();
                return;
            }

            if(tarefa.criterio_aceitacao[0] == '\0')
            {
                limpar_tela();

                printf("Nao e possivel concluir a tarefa.\n");
                printf("O criterio de aceitacao esta vazio.\n");

                aguardar_enter();
                return;
            }

            strcpy(tarefa.status, MAPEAMENTO_STATUS[2]);
        }

        else if(opcao_status == 0)
        {
            limpar_tela();
            printf("Alteracao de status cancelada.\n");
            aguardar_enter();
            return;
        }

        else
        {
            limpar_tela();
            printf("Opcao de status invalida! Tente novamente.\n");
            aguardar_enter();
            continue;
        }

        FILE *arquivo = abrirArquivo("tarefas.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("Erro ao abrir arquivo.\n");
            aguardar_enter();
            return;
        }

        salvar_registro(arquivo, &tarefa, sizeof(Tarefa), posicao);

        fclose(arquivo);

        limpar_tela();
        printf("Status atualizado.\n");
        aguardar_enter();

        return;

    } while(1);
}
//---------------------------------

void alterar_prioridade(int id_projeto)
{
    Tarefa tarefa;
    long posicao;

    if(!selecionar_item(
        "tarefa",
        "alterar prioridade",
        &tarefa,
        &posicao,
        id_projeto))
        return;

    limpar_tela();

    printf("Alterar Prioridade [ID: %d]\n", tarefa.id_tarefa);

    if(tarefa.prioridade == -1)
        printf("Prioridade atual: Nao definida\n");
    else
        printf("Prioridade atual: %s\n",
               MAPEAMENTO_PRIORIDADE[tarefa.prioridade]);

    printf("\n1 - Baixa");
    printf("\n2 - Media");
    printf("\n3 - Alta");
    printf("\n4 - Remover prioridade");
    printf("\n0 - Cancelar\n");


    int opcao = ler_num_inteiro("\nEscolha: ");


    if(opcao >= 1 && opcao <= 3)
    {
        tarefa.prioridade = opcao - 1;
    }
    else if(opcao == 4)
    {
        tarefa.prioridade = -1;
    }
    else if(opcao == 0)
    {
        return;
    }
    else
    {
        printf("\nOpcao invalida.\n");
        aguardar_enter();
        return;
    }


    FILE *arquivo = abrirArquivo("tarefas.dat", "rb+");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }


    salvar_registro(
        arquivo,
        &tarefa,
        sizeof(Tarefa),
        posicao
    );

    fclose(arquivo);


    printf("\nPrioridade atualizada com sucesso!\n");
    aguardar_enter();
}

int dependencia_gera_ciclo(int id_tarefa, int id_dependencia)
{
    FILE *arquivo;
    Tarefa tarefa;
    int atual = id_dependencia;

    while(atual != 0)
    {
        if(atual == id_tarefa)
            return 1;

        arquivo = abrirArquivo("tarefas.dat", "rb");

        if(arquivo == NULL)
            return 0;

        int encontrou = 0;

        while(fread(&tarefa, sizeof(Tarefa), 1, arquivo))
        {
            if(tarefa.id_tarefa == atual &&
               tarefa.estado_tarefa == 1)
            {
                atual = tarefa.id_tarefa_dependencia;
                encontrou = 1;
                break;
            }
        }

        fclose(arquivo);

        if(!encontrou)
            return 0;
    }

    return 0;
}

void editar_campos(int id_projeto) {

   Tarefa tarefa;
   long posicao;

    if(!selecionar_item("tarefa", "editar", &tarefa, &posicao, id_projeto))
        return;

    FILE *arquivo = abrirArquivo("tarefas.dat", "rb+");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }

    int menu_edicao_tarefa;

    do {
        limpar_tela();

        printf("Edição [ID: %d]\n", tarefa.id_tarefa);
        printf("Nome: %s\n", tarefa.nome);
        printf("Descricao: %s\n", tarefa.descricao_tf);
        Usuario usuario_responsavel;

        FILE *arquivo_usuarios =
            abrirArquivo("usuarios.dat", "rb");

        if(arquivo_usuarios != NULL)
        {
            while(fread(&usuario_responsavel,
                        sizeof(Usuario),
                        1,
                        arquivo_usuarios))
            {
                if(usuario_responsavel.id_usuario ==
                tarefa.id_usuario_responsavel &&
                usuario_responsavel.estado_usuario == 1)
                {
                    printf("Usuario responsavel: %s\n",
                        usuario_responsavel.nome_do_usuario);
                    break;
                }
            }

            fclose(arquivo_usuarios);
        }

        printf("Selecione o campo que deseja alterar:\n");
        printf("1 - Nome\n");
        printf("2 - Descricao\n");
        printf("3 - Usuario Responsavel\n");
        printf("4 - Tipo\n");
        printf("5 - Criterio de Aceitacao\n");
        printf("6 - Dependencia\n");
        printf("7 - Salvar\n");
        printf("0 - Cancelar\n");

        menu_edicao_tarefa = ler_num_inteiro("\nEscolha: ");

        switch (menu_edicao_tarefa) {
            case 1:
                ler_texto_obrigatorio("Novo Nome: ", tarefa.nome, TAMANHO_NOME);
                break;
            case 2:
                ler_texto_obrigatorio("Nova Descricao: ", tarefa.descricao_tf, TAMANHO_TEXTO);
                break;
            case 3:
            {
                Usuario usuario_responsavel;

                if(selecionar_item(
                    "usuario",
                    "alterar responsavel",
                    &usuario_responsavel,
                    NULL,
                    id_projeto))
                {
                    tarefa.id_usuario_responsavel =
                        usuario_responsavel.id_usuario;
                }

                break;
            }
            case 4:
            {
                printf("\nTipo:\n");
                printf("1 - Modelagem\n");
                printf("2 - Impressao\n");
                printf("3 - Acabamento\n");
                printf("4 - Manutencao\n");

                int opcao_tipo = ler_num_inteiro("Escolha o tipo: ");

                if (opcao_tipo >= 1 && opcao_tipo <= 5) {
                    strcpy(tarefa.tipo,
                        MAPEAMENTO_TIPO[opcao_tipo - 1]);
                } else {
                    printf("Tipo invalido.\n");
                    aguardar_enter();
                }
                break;
            }
            case 5:
                ler_texto_obrigatorio(
                    "Novo Criterio de Aceitacao: ",
                    tarefa.criterio_aceitacao, TAMANHO_TEXTO
                );
                break;
            case 6:
            {
                int opcao_dependencia;

                printf("\nDependencia atual: ");

                if(tarefa.id_tarefa_dependencia == 0)
                    printf("Nenhuma\n");
                else
                    printf("ID %d\n", tarefa.id_tarefa_dependencia);

                printf("\n1 - Escolher Tarefa Dependente\n");
                printf("2 - Remover Dependencia\n");
                printf("0 - Cancelar\n");

                opcao_dependencia =
                    ler_num_inteiro("\nEscolha: ");

                if(opcao_dependencia == 1)
                {
                    Tarefa dependencia;

                    if(selecionar_item(
                        "tarefa",
                        "definir dependencia",
                        &dependencia,
                        NULL, id_projeto))
                    {
                        if(dependencia.id_tarefa == tarefa.id_tarefa)
                        {
                            printf("\nUma tarefa nao pode depender dela mesma.\n");
                            aguardar_enter();
                        }
                        else if(dependencia_gera_ciclo(
                                    tarefa.id_tarefa,
                                    dependencia.id_tarefa))
                        {
                            printf("\nEssa dependencia criaria um ciclo entre as tarefas.\n");
                            aguardar_enter();
                        }
                        else
                        {
                            tarefa.id_tarefa_dependencia =
                                dependencia.id_tarefa;
                        }
                    }
                }
                else if(opcao_dependencia == 2)
                {
                    tarefa.id_tarefa_dependencia = 0;
                }

                break;
            }
            case 7:
                salvar_registro(
                    arquivo,
                    &tarefa,
                    sizeof(Tarefa),
                    posicao
                );

                limpar_tela();
                printf("Edicao concluida com sucesso!\n");
                aguardar_enter();

                fclose(arquivo);
                return;
            case 0:
                limpar_tela();
                break;

            default:
                limpar_tela();
                printf("Opcao invalida.\n");
                aguardar_enter();
                break;
        }
    } while (menu_edicao_tarefa != 0);

    fclose(arquivo);
}

void excluir_tarefa(int id_projeto) {

    Tarefa tarefa;
    long posicao;

    while(1)
    {
        //limpar_tela();
        if(!selecionar_item("tarefa", "excluir", &tarefa, &posicao, id_projeto))
            return;

        limpar_tela();

        printf("\nTem certeza que deseja excluir '[ID: %d] %s'? (1 - Sim / 0 - Não): ", tarefa.id_tarefa, tarefa.nome);

        int confirmacao = ler_num_inteiro("");
        if (confirmacao != 1) {
            limpar_tela();
            continue;
        }

        // Verifica se alguma tarefa depende desta
        FILE *arquivo = abrirArquivo("tarefas.dat", "rb");

        if(arquivo == NULL)
        {
            printf("Erro ao abrir arquivo.\n");
            aguardar_enter();
            return;
        }
        Tarefa outra_tarefa;
        int possui_dependencia = 0;

        if(tarefa.id_tarefa_dependencia != 0)
        {
            possui_dependencia = 1;
        }

        while(fread(&outra_tarefa, sizeof(Tarefa), 1, arquivo))
        {
            if(outra_tarefa.estado_tarefa == 1 &&
            outra_tarefa.id_tarefa_dependencia == tarefa.id_tarefa)
            {
                possui_dependencia = 1;
                break;
            }
        }

        fclose(arquivo);

        if(possui_dependencia)
        {
            limpar_tela();
            printf("Nao e possivel excluir esta tarefa. Existe uma dependencia\n");
            aguardar_enter();
            continue;
        }

        tarefa.estado_tarefa = 0;

        arquivo = abrirArquivo("tarefas.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("Erro ao abrir arquivo.\n");
            aguardar_enter();
            return;
        }

        salvar_registro(arquivo, &tarefa, sizeof(Tarefa), posicao);
        fclose(arquivo);

        limpar_tela();

        printf("Tarefa excluida com sucesso!\n");
        aguardar_enter();
        return;
    }
}

void alterar_impedimento(int id_projeto)
{
    Tarefa tarefa;
    long posicao;

    while(1)
    {
        //limpar_tela();

        if(!selecionar_item("tarefa", "registrar Impedimento", &tarefa, &posicao, id_projeto))
            return;

        if(strcmp(tarefa.status, MAPEAMENTO_STATUS[1]) == 0)
        {
            limpar_tela();

            printf("Registrar Impedimento [ID: %d]\n", tarefa.id_tarefa);
            printf("Nome: %s\n", tarefa.nome);
            printf("Status atual: %s\n", tarefa.status);

            printf("\n1 - Registrar Impedimento\n");
            printf("0 - Cancelar\n");

            int opcao = ler_num_inteiro("\nEscolha: ");

            if(opcao == 1)
            {
                strcpy(tarefa.status, MAPEAMENTO_STATUS[3]);
            }
            else
            {
                printf("Acao cancelada.\n");
                aguardar_enter();
                return;
            }
        }

        /*
        Se a tarefa já estiver impedida,
        o Andon pode ser encerrado.
        */
        else if(strcmp(tarefa.status, MAPEAMENTO_STATUS[3]) == 0)
        {
            limpar_tela();

            printf("Tarefa Impedida [ID: %d]\n", tarefa.id_tarefa);
            printf("Nome: %s\n", tarefa.nome);

            printf("\n1 - Retomar tarefa\n");
            printf("0 - Cancelar\n");

            int opcao = ler_num_inteiro("\nEscolha: ");

            if(opcao == 1)
            {
                strcpy(tarefa.status, MAPEAMENTO_STATUS[1]);
            }
            else
            {
                printf("Acao cancelada.\n");
                aguardar_enter();
                return;
            }
        }

        else
        {
            limpar_tela();

            printf("Nao pode ser Impedida.\n");
            printf("A tarefa precisa estar em 'Fazendo'.\n");

            aguardar_enter();
            return;
        }

        FILE *arquivo = abrirArquivo("tarefas.dat", "rb+");

        if(arquivo == NULL)
        {
            printf("Erro ao abrir arquivo.\n");
            aguardar_enter();
            return;
        }

        salvar_registro(
            arquivo,
            &tarefa,
            sizeof(Tarefa),
            posicao
        );

        fclose(arquivo);

        limpar_tela();

        if(strcmp(tarefa.status, MAPEAMENTO_STATUS[3]) == 0)
            printf("Tarefa Impedida.\n");
        else
            printf("Tarefa Retomada.\n");

        aguardar_enter();
    }
}

void menu_tarefas(int id_projeto) {
    int opcao_menu;

    do {
        limpar_tela();

        printf("Gerenciar Tarefas\n\n");

        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Alterar Status\n");
        printf("5 - Impedimento\n");
        printf("6 - Excluir\n");
        printf("0 - Voltar\n");

        opcao_menu = ler_num_inteiro("\nEscolha: ");

        switch (opcao_menu) {
            case 1:
                cadastrar_tarefa(id_projeto);
                break;

            case 2:
                consultar_tarefa(id_projeto);
                break;

            case 3:
                editar_campos(id_projeto);
                break;

            case 4:
                alterar_status(id_projeto);
                break;

            case 5:
                alterar_impedimento(id_projeto);
                break;

            case 6:
                excluir_tarefa(id_projeto);
                break;
            case 0:
                break;

            default:
                limpar_tela();
                printf("Opcao invalida. Tente novamente.\n");
                aguardar_enter();
                break;
        }

    } while (opcao_menu != 0);
}