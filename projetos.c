#include "projetos.h"

int listar_projetos(void)
{
    FILE *arquivo = abrirArquivo("projetos.dat", "rb");
    Projeto projeto;
    int encontrou = 0;

    if (arquivo == NULL)
        return 0;

    printf("Projetos cadastrados:\n\n");

    while(fread(&projeto, sizeof(Projeto), 1, arquivo))
    {
        if(projeto.estado_projeto == 1)
        {
            printf("[ID: %d] %s\n\n",
                   projeto.id_projeto,
                   projeto.nome_do_projeto);

            encontrou = 1;
        }
    }

    fclose(arquivo);

    return encontrou;
}

void cadastrar_projeto(void) {
    limpar_tela();

    printf("Cadastro. (Digite 0 para voltar).\n");

    Projeto novo_projeto = {0};

    if (!ler_texto_obrigatorio("\nNome do projeto: ", novo_projeto.nome_do_projeto, TAMANHO_NOME))
        return;

    if (!ler_texto_obrigatorio("Descricao: ", novo_projeto.descricao_proj, TAMANHO_TEXTO))
        return;

    /* USUARIOS DO PROJETO */
    novo_projeto.quantidade_usuarios = 0;

    while(1)
    {
        Usuario usuario;
        int usuario_encontrado = 0;

        limpar_tela();

        printf("Cadastro de Projeto\n\n");
        printf("Nome: %s\n", novo_projeto.nome_do_projeto);
        printf("Descricao: %s\n\n", novo_projeto.descricao_proj);

        printf("Usuarios selecionados:\n\n");

        for(int i = 0;
            i < novo_projeto.quantidade_usuarios;
            i++)
        {
            FILE *arquivo_usuarios =
                abrirArquivo("usuarios.dat", "rb");

            if(arquivo_usuarios != NULL)
            {
                Usuario usuario_lista;

                while(fread(&usuario_lista,
                            sizeof(Usuario),
                            1,
                            arquivo_usuarios))
                {
                    if(usuario_lista.id_usuario ==
                    novo_projeto.id_usuarios[i] &&
                    usuario_lista.estado_usuario == 1)
                    {
                        printf("[ID: %d] %s\n",
                            usuario_lista.id_usuario,
                            usuario_lista.nome_do_usuario);

                        break;
                    }
                }

                fclose(arquivo_usuarios);
            }
        }

        printf("\nUsuarios disponiveis:\n\n");

        FILE *arquivo_disponiveis =
            abrirArquivo("usuarios.dat", "rb");

        if(arquivo_disponiveis != NULL)
        {
            Usuario usuario_lista;

            while(fread(&usuario_lista,
                        sizeof(Usuario),
                        1,
                        arquivo_disponiveis))
            {
                int selecionado = 0;

                for(int i = 0;
                    i < novo_projeto.quantidade_usuarios;
                    i++)
                {
                    if(novo_projeto.id_usuarios[i] ==
                    usuario_lista.id_usuario)
                    {
                        selecionado = 1;
                        break;
                    }
                }

                if(usuario_lista.estado_usuario == 1 &&
                !selecionado)
                {
                    printf("[ID: %d] %s\n",
                        usuario_lista.id_usuario,
                        usuario_lista.nome_do_usuario);
                }
            }

            fclose(arquivo_disponiveis);
        }

        int id_usuario =
            ler_num_inteiro(
                "\nDigite o ID do usuario (0 - Voltar): "
            );

        if(id_usuario == 0)
        {
            if(novo_projeto.quantidade_usuarios == 0)
            {
                return;
            }

            break;
        }

        FILE *arquivo_usuarios =
            abrirArquivo("usuarios.dat", "rb");

        if(arquivo_usuarios != NULL)
        {
            while(fread(&usuario,
                        sizeof(Usuario),
                        1,
                        arquivo_usuarios))
            {
                if(usuario.id_usuario == id_usuario &&
                usuario.estado_usuario == 1)
                {
                    usuario_encontrado = 1;
                    break;
                }
            }

            fclose(arquivo_usuarios);
        }

        if(!usuario_encontrado)
        {
            printf("\nUsuario nao encontrado. Tente novamente.\n");
            aguardar_enter();
            continue;
        }

        int usuario_repetido = 0;

        for(int i = 0;
            i < novo_projeto.quantidade_usuarios;
            i++)
        {
            if(novo_projeto.id_usuarios[i] ==
            usuario.id_usuario)
            {
                usuario_repetido = 1;
                break;
            }
        }

        if(usuario_repetido)
        {
            printf("\nEste usuario ja pertence ao projeto.\n");
            aguardar_enter();
            continue;
        }

        /* ADICIONAR USUARIO */
        novo_projeto.id_usuarios[
            novo_projeto.quantidade_usuarios
        ] = usuario.id_usuario;

        novo_projeto.quantidade_usuarios++;

        limpar_tela();

        printf("Cadastro de Projeto\n\n");
        printf("Nome: %s\n", novo_projeto.nome_do_projeto);
        printf("Descricao: %s\n\n", novo_projeto.descricao_proj);

        printf("Usuarios selecionados:\n");

        for(int i = 0;
            i < novo_projeto.quantidade_usuarios;
            i++)
        {
            FILE *arquivo_usuarios =
                abrirArquivo("usuarios.dat", "rb");

            if(arquivo_usuarios != NULL)
            {
                Usuario usuario_lista;

                while(fread(&usuario_lista,
                            sizeof(Usuario),
                            1,
                            arquivo_usuarios))
                {
                    if(usuario_lista.id_usuario ==
                    novo_projeto.id_usuarios[i] &&
                    usuario_lista.estado_usuario == 1)
                    {
                        printf("[ID: %d] %s\n",
                            usuario_lista.id_usuario,
                            usuario_lista.nome_do_usuario);

                        break;
                    }
                }

                fclose(arquivo_usuarios);
            }
        }

        printf("\n");

        /* LIMITE */
        if(novo_projeto.quantidade_usuarios >=
           MAX_USUARIOS_PROJETO)
        {
            printf("\nLimite de usuarios atingido.\n");
            aguardar_enter();
            break;
        }
    }

    limpar_tela();

    printf("Cadastro de Projeto\n\n");

    printf("Nome: %s\n", novo_projeto.nome_do_projeto);
    printf("Descricao: %s\n\n", novo_projeto.descricao_proj);

    printf("Usuarios selecionados:\n\n");

    for(int i = 0;
        i < novo_projeto.quantidade_usuarios;
        i++)
    {
        FILE *arquivo_usuarios =
            abrirArquivo("usuarios.dat", "rb");

        if(arquivo_usuarios != NULL)
        {
            Usuario usuario_lista;

            while(fread(&usuario_lista,
                        sizeof(Usuario),
                        1,
                        arquivo_usuarios))
            {
                if(usuario_lista.id_usuario ==
                   novo_projeto.id_usuarios[i] &&
                   usuario_lista.estado_usuario == 1)
                {
                    printf("[ID: %d] %s\n",
                           usuario_lista.id_usuario,
                           usuario_lista.nome_do_usuario);

                    break;
                }
            }

            fclose(arquivo_usuarios);
        }
    }

    printf("\n");

    do {
        novo_projeto.estimado = ler_num_inteiro("Valor estimado: ");

        if (novo_projeto.estimado <= 0) {
            printf("O valor estimado deve ser maior que zero.\n");
        }

    } while (novo_projeto.estimado <= 0);


    printf("\n");

    if(!ler_prazo(novo_projeto.prazo))
        return;

    novo_projeto.concluido = 0;
    novo_projeto.restante = novo_projeto.estimado;
    strcpy(novo_projeto.status_proj, MAPEAMENTO_STATUS_PROJ[0]);

    novo_projeto.data_criacao = time(NULL);

    novo_projeto.id_projeto = prox_id_projeto++;
    novo_projeto.estado_projeto = 1;

    FILE *arquivo = abrirArquivo("projetos.dat", "ab");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        aguardar_enter();
        return;
    }

    fwrite(&novo_projeto, sizeof(Projeto), 1, arquivo);

    fclose(arquivo);

    limpar_tela();
    printf("Projeto cadastrado com sucesso! (ID: %d)\n",
           novo_projeto.id_projeto);

    aguardar_enter();
}

void consultar_projeto(void)
{
    Projeto projeto;
    long posicao;

    if(!selecionar_item("projeto", "consultar", &projeto, &posicao, 0))
        return;

    while(1)
    {
        limpar_tela();

        printf("Consulta Projeto [ID: %d]\n", projeto.id_projeto);
        printf("Nome: %s\n", projeto.nome_do_projeto);
        printf("Descricao: %s\n\n", projeto.descricao_proj);

        printf("Estimado: %d\n", projeto.estimado);
        printf("Concluido: %d\n", projeto.concluido);
        printf("Restante: %d\n", projeto.restante);
        printf("Status: %s\n", projeto.status_proj);
        printf("Prazo: %s\n", projeto.prazo);

        printf("\n1 - Gerenciar Tarefas\n");
        printf("2 - Listar Usuarios\n");
        printf("3 - Atualizar progresso\n");
        printf("0 - Voltar\n");

        int opcao = ler_num_inteiro("\nEscolha: ");

        if(opcao == 0)
            return;

        if(opcao == 1)
        {
            menu_tarefas(projeto.id_projeto);
            continue;
        }

        if(opcao == 2)
        {

            printf("Usuarios do Projeto [ID: %d]\n\n",
                   projeto.id_projeto);

            if(projeto.quantidade_usuarios == 0)
            {
                printf("Nenhum usuario vinculado.\n");
            }
            else
            {
                for(int i = 0;
                    i < projeto.quantidade_usuarios;
                    i++)
                {
                    FILE *arquivo_usuarios =
                        abrirArquivo("usuarios.dat", "rb");

                    if(arquivo_usuarios != NULL)
                    {
                        Usuario usuario;

                        while(fread(&usuario,
                                    sizeof(Usuario),
                                    1,
                                    arquivo_usuarios))
                        {
                            if(usuario.id_usuario ==
                               projeto.id_usuarios[i] &&
                               usuario.estado_usuario == 1)
                            {
                                printf("[ID: %d] %s\n",
                                       usuario.id_usuario,
                                       usuario.nome_do_usuario);

                                break;
                            }
                        }

                        fclose(arquivo_usuarios);
                    }
                }
            }

            aguardar_enter();
            continue;
        }

        if(opcao == 3)
        {
            atualizar_progresso_projeto(&projeto, posicao);
            continue;
        }
    }
}

void editar_projeto(void) {

    Projeto projeto;
    long posicao;

        if(!selecionar_item("projeto", "editar", &projeto, &posicao, 0))
            return;

    FILE *arquivo = abrirArquivo("projetos.dat", "rb+");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }

    int menu_edicao;

    do {
        limpar_tela();

        printf("Edicao [ID: %d]\n", projeto.id_projeto);
        printf("Nome: %s\n", projeto.nome_do_projeto);
        printf("Descricao: %s\n", projeto.descricao_proj);
        printf("Prazo: %s\n", projeto.prazo);

        printf("\nSelecione o campo que deseja alterar:\n");
        printf("1 - Nome\n");
        printf("2 - Descricao\n");
        printf("3 - Editar Usuarios\n");
        printf("4 - Valor Estimado\n");
        printf("5 - Prazo\n");
        printf("6 - Salvar\n");
        printf("0 - Cancelar\n");

        menu_edicao = ler_num_inteiro("\nEscolha: ");

        switch (menu_edicao) {

            case 1:
                ler_texto_obrigatorio(
                    "Novo Nome: ",
                    projeto.nome_do_projeto, TAMANHO_NOME
                );
                break;

            case 2:
                ler_texto_obrigatorio(
                    "Nova Descricao: ",
                    projeto.descricao_proj, TAMANHO_TEXTO
                );
                break;
            case 3:
            {
                int opcao_usuario;

                do
                {
                    limpar_tela();

                    printf("Usuarios do Projeto [ID: %d]\n\n",
                        projeto.id_projeto);

                    printf("Usuarios atuais:\n\n");

                    for(int i = 0;
                        i < projeto.quantidade_usuarios;
                        i++)
                    {
                        FILE *arquivo_usuarios =
                            abrirArquivo("usuarios.dat", "rb");

                        if(arquivo_usuarios != NULL)
                        {
                            Usuario usuario;

                            while(fread(&usuario,
                                        sizeof(Usuario),
                                        1,
                                        arquivo_usuarios))
                            {
                                if(usuario.id_usuario ==
                                projeto.id_usuarios[i] &&
                                usuario.estado_usuario == 1)
                                {
                                    printf("[ID: %d] %s\n",
                                        usuario.id_usuario,
                                        usuario.nome_do_usuario);

                                    break;
                                }
                            }

                            fclose(arquivo_usuarios);
                        }
                    }

                    printf("\n1 - Adicionar Usuario\n");
                    printf("2 - Remover Usuario\n");
                    printf("0 - Voltar\n");

                    opcao_usuario =
                        ler_num_inteiro("\nEscolha: ");

                    if(opcao_usuario == 1)
                    {
                        int ids_disponiveis[MAX_USUARIOS_PROJETO];
                        int quantidade_disponiveis = 0;

                        if(projeto.quantidade_usuarios >= MAX_USUARIOS_PROJETO)
                        {
                            printf("\nLimite de usuarios atingido.\n");
                            aguardar_enter();
                            continue;
                        }

                        // Monta lista de usuarios que ainda nao pertencem ao projeto
                        FILE *arquivo_usuarios =
                            abrirArquivo("usuarios.dat", "rb");


                        if(arquivo_usuarios != NULL)
                        {
                            Usuario usuario;


                            while(fread(&usuario,
                                        sizeof(Usuario),
                                        1,
                                        arquivo_usuarios))
                            {
                                if(usuario.estado_usuario == 1)
                                {
                                    int ja_pertence = 0;


                                    for(int i = 0;
                                        i < projeto.quantidade_usuarios;
                                        i++)
                                    {
                                        if(projeto.id_usuarios[i] ==
                                        usuario.id_usuario)
                                        {
                                            ja_pertence = 1;
                                            break;
                                        }
                                    }


                                    if(!ja_pertence)
                                    {
                                        ids_disponiveis[quantidade_disponiveis] =
                                            usuario.id_usuario;

                                        quantidade_disponiveis++;
                                    }
                                }
                            }


                            fclose(arquivo_usuarios);
                        }


                        if(quantidade_disponiveis == 0)
                        {
                            printf("\nNao existem usuarios disponiveis para adicionar.\n");
                            aguardar_enter();
                            continue;
                        }


                        printf("\nUsuarios disponiveis:\n\n");


                        arquivo_usuarios =
                            abrirArquivo("usuarios.dat", "rb");


                        if(arquivo_usuarios != NULL)
                        {
                            Usuario usuario;


                            while(fread(&usuario,
                                        sizeof(Usuario),
                                        1,
                                        arquivo_usuarios))
                            {
                                for(int i = 0;
                                    i < quantidade_disponiveis;
                                    i++)
                                {
                                    if(usuario.id_usuario ==
                                    ids_disponiveis[i])
                                    {
                                        printf("[ID: %d] %s\n",
                                            usuario.id_usuario,
                                            usuario.nome_do_usuario);

                                        break;
                                    }
                                }
                            }


                            fclose(arquivo_usuarios);
                        }


                        int id_usuario =
                            ler_num_inteiro("\nDigite o ID do usuario: ");


                        int usuario_valido = 0;


                        for(int i = 0;
                            i < quantidade_disponiveis;
                            i++)
                        {
                            if(ids_disponiveis[i] == id_usuario)
                            {
                                usuario_valido = 1;
                                break;
                            }
                        }


                        if(!usuario_valido)
                        {
                            printf("\nUsuario invalido ou ja pertence ao projeto.\n");
                            aguardar_enter();
                            continue;
                        }


                        projeto.id_usuarios[
                            projeto.quantidade_usuarios
                        ] = id_usuario;


                        projeto.quantidade_usuarios++;


                        printf("\nUsuario adicionado ao projeto!\n");
                        aguardar_enter();
                    }
                    else if(opcao_usuario == 2)
                    {
                        int id_usuario =
                            ler_num_inteiro(
                                "\nDigite o ID do usuario a remover (0 - Voltar): "
                            );

                        if(id_usuario == 0)
                            continue;

                        int encontrado = 0;

                        for(int i = 0;
                            i < projeto.quantidade_usuarios;
                            i++)
                        {
                            if(projeto.id_usuarios[i] == id_usuario)
                            {
                                for(int j = i;
                                    j < projeto.quantidade_usuarios - 1;
                                    j++)
                                {
                                    projeto.id_usuarios[j] =
                                        projeto.id_usuarios[j + 1];
                                }

                                projeto.quantidade_usuarios--;
                                encontrado = 1;

                                break;
                            }
                        }

                        if(!encontrado)
                        {
                            printf("\nUsuario nao pertence ao projeto.\n");
                            aguardar_enter();
                        }
                    }

                } while(opcao_usuario != 0);

                break;
            }

			case 4: {
				int novo_estimado;

				do {
					novo_estimado =
						ler_num_inteiro("Novo valor estimado: ");

					if (novo_estimado <= 0) {
						printf("O valor estimado deve ser maior que zero.\n");
					}
					else if (novo_estimado < projeto.concluido) {
						printf("O valor estimado nao pode ser menor que "
							   "o valor concluido (%d).\n",
							   projeto.concluido);
					}

				} while (novo_estimado <= 0 || novo_estimado < projeto.concluido);

				projeto.estimado = novo_estimado;

				projeto.restante = projeto.estimado - projeto.concluido;

				if(projeto.concluido == 0)
                    strcpy(projeto.status_proj, MAPEAMENTO_STATUS_PROJ[0]);
                else if(projeto.concluido < projeto.estimado)
                    strcpy(projeto.status_proj, MAPEAMENTO_STATUS_PROJ[1]);
                else
                    strcpy(projeto.status_proj, MAPEAMENTO_STATUS_PROJ[2]);

				break;
			}

			case 5:
            {
                printf("\n");

                ler_prazo(projeto.prazo);

                break;
            }
            case 6:
                salvar_registro(arquivo, &projeto, sizeof(Projeto), posicao);
                fclose(arquivo);

                limpar_tela();
                printf("Edicao concluida com sucesso!\n");
                aguardar_enter();
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

    } while (menu_edicao != 0);

    fclose(arquivo);
}

void excluir_projeto(void) {

    Projeto projeto;
    long posicao;

    //limpar_tela();
    if(!selecionar_item("projeto", "excluir", &projeto, &posicao, 0))
        return;

    limpar_tela();
    printf("\nTem certeza que deseja excluir '[ID: %d] %s'? (1 - Sim / 0 - Não): ",
        projeto.id_projeto,
        projeto.nome_do_projeto);

    int confirmacao = ler_num_inteiro("");

    if(confirmacao != 1)
    {
        limpar_tela();
        return;
    }

    FILE *arquivo_tarefas = abrirArquivo("tarefas.dat", "rb");

    if(arquivo_tarefas != NULL)
    {
        Tarefa tarefa;
        int possui_tarefas = 0;

        while(fread(&tarefa, sizeof(Tarefa), 1, arquivo_tarefas))
        {
            if(tarefa.estado_tarefa == 1 &&
               tarefa.id_projeto == projeto.id_projeto)
            {
                possui_tarefas = 1;
                break;
            }
        }

        fclose(arquivo_tarefas);

        if(possui_tarefas)
        {
            limpar_tela();

            printf("Nao e possivel excluir este projeto. ");
            printf("Existem tarefas vinculadas.\n");

            aguardar_enter();
            return;
        }
    }

    projeto.estado_projeto = 0;

    FILE *arquivo = abrirArquivo("projetos.dat", "rb+");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }

    salvar_registro(arquivo, &projeto, sizeof(Projeto), posicao);

    fclose(arquivo);

    limpar_tela();

    printf("Projeto excluido com sucesso!\n");
    aguardar_enter();
}

void atualizar_progresso_projeto(Projeto *projeto, long posicao)
{
    limpar_tela();

    printf("Atualizar Progresso [ID: %d]\n", projeto->id_projeto);
    printf("Estimado: %d\n", projeto->estimado);
    printf("Concluido atual: %d\n", projeto->concluido);
    printf("Restante atual: %d\n", projeto->restante);

    do {
        projeto->concluido = ler_num_inteiro(
            "\nDigite o novo valor concluido: "
        );

        if (projeto->concluido < 0 ||
            projeto->concluido > projeto->estimado)
        {
            printf("Valor invalido.\n");
        }

    } while (projeto->concluido < 0 ||
             projeto->concluido > projeto->estimado);

    projeto->restante =
        projeto->estimado - projeto->concluido;

    if(projeto->concluido == 0)
        strcpy(projeto->status_proj, MAPEAMENTO_STATUS_PROJ[0]);
    else if(projeto->concluido < projeto->estimado)
        strcpy(projeto->status_proj, MAPEAMENTO_STATUS_PROJ[1]);
    else
        strcpy(projeto->status_proj, MAPEAMENTO_STATUS_PROJ[2]);

    FILE *arquivo = abrirArquivo("projetos.dat", "rb+");

    if(arquivo == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        aguardar_enter();
        return;
    }

    salvar_registro(
        arquivo,
        projeto,
        sizeof(Projeto),
        posicao
    );

    fclose(arquivo);

    limpar_tela();

    printf("Dados atualizados:\n");
    printf("Estimado: %d\n", projeto->estimado);
    printf("Concluido: %d\n", projeto->concluido);
    printf("Restante: %d\n", projeto->restante);

    aguardar_enter();
}

void menu_projetos(void) {
    int opcao_menu;

    do {
        limpar_tela();

        printf("Gerenciar Projetos\n\n");

        printf("1 - Cadastrar Projeto\n");
        printf("2 - Adicionar Tarefa\n");
        printf("3 - Consultar Projeto\n");
        printf("4 - Editar Projeto\n");
        printf("5 - Excluir Projeto\n");
        printf("0 - Voltar\n");

        opcao_menu = ler_num_inteiro("\nEscolha: ");

        switch (opcao_menu) {
            case 1:
                cadastrar_projeto();
                break;
            case 2:
            {
                Projeto projeto;

                limpar_tela();

                if(selecionar_item(
                    "projeto",
                    "adicionar tarefa",
                    &projeto,
                    NULL,
                    0))
                {
                    cadastrar_tarefa(projeto.id_projeto);
                }

                break;
            }
            case 3:
                consultar_projeto();
                break;

            case 4:
                editar_projeto();
                break;

            case 5:
                excluir_projeto();
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
