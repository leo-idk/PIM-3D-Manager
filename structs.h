#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#define TAMANHO_NOME 60
#define TAMANHO_EMAIL 51
#define TAMANHO_SENHA 7
#define ADMIN 1
#define EMAIL_ADMIN "admin@sistema.com"
#define SENHA_ADMIN "12345"
#define MEMBRO 1
#define GERENTE 2

#define TAMANHO_TEXTO 200
#define MAX_USUARIOS_PROJETO 10
#define MAX_ARQUIVOS 10
#define QUANTIDADE_PRIORIDADES 3

typedef struct {
    int id_usuario;
    char nome_do_usuario[TAMANHO_NOME];
    char email[TAMANHO_EMAIL];
    char password[TAMANHO_SENHA];
    int estado_usuario;
    int nivel_acesso;

} Usuario;

// PROJETO STRUCT
typedef struct {
    int id_projeto;
    //int id_usuario;
    char nome_do_projeto[TAMANHO_NOME];
    char descricao_proj[TAMANHO_TEXTO];

    int estimado;
    int concluido;
    int restante;
    char prazo[11];

    int id_usuarios[MAX_USUARIOS_PROJETO];
    int quantidade_usuarios;

    char status_proj[30];
    int quantidade_arquivos;

    int estado_projeto;
    time_t data_criacao;

} Projeto;

//TAREFA STRUCT
typedef struct {
    int id_tarefa;
    int id_projeto;
    //id_usuario;

    char nome[TAMANHO_NOME];
    char descricao_tf[TAMANHO_TEXTO];
    int id_usuario_responsavel;
    char tipo[50];
    char criterio_aceitacao[TAMANHO_TEXTO];
    char status[50];

    int id_tarefa_dependencia;

    int prioridade;

    int estado_tarefa;
    time_t data_criacao;

} Tarefa;

//ESTOQUE STRUCT
typedef struct {
    int id_filamento;
    char codigo[15];
    char tipo[30];
    char marca[30];
    char cor[20];
    float estoque_atual;
    float estoque_minimo;
    float custo_atual;
    char localizacao[50];

    int estado_filamento;

} Filamento;

typedef struct
{
    int id_movimentacao;
    int id_filamento;
    int id_projeto;

    char tipo[10];
    float quantidade;

    float peso_real;
    float perda;
    float testes;
    float suportes;

    int tem_peso_real;
    int tem_perda;
    int tem_testes;
    int tem_suportes;

    float desperdicio;
    float custo_extra;

    int id_usuario;
    char motivo_movimentacao[100];
    time_t data_movimentacao;

    int estado_movimentacao;
} Movimentacao;

extern int prox_id_usuario, id_usuario_logado, nivel_usuario_logado;
extern int prox_id_projeto, prox_id_tarefa, prox_id_filamento, prox_id_movimentacao;
extern const char *MAPEAMENTO_STATUS_PROJ[];
extern const char *MAPEAMENTO_STATUS[];
extern const char *MAPEAMENTO_TIPO[];
extern const char *MAPEAMENTO_PRIORIDADE[3];

/* Prototipos compartilhados */
void limpar_tela(void);
void aguardar_enter(void);
int ler_num_inteiro(const char *msg_prompt);
float ler_num_real(const char *msg_prompt);
int ler_texto_obrigatorio(const char *msg_prompt, char *texto_destino, int tamanho);
int selecionar_item(const char *tipo, const char *acao, void *item, long *posicao, int id_projeto);
int ler_prazo(char *prazo);
int autenticar_admin(void);
void ler_senha(char *senha, int tamanho);
FILE* abrirArquivo(const char *nomeArquivo, const char *modo);
int salvar_registro(FILE *arquivo, void *registro, size_t tamanho, long posicao);

//TELA DE LOGIN E MENU INICIAL
void tela_de_login(void);
void tela_de_cadastro(void);
void mostrar_data_atual(void);
void menu_inicial(void);

//CADASTRO DE USUARIOS
int listar_usuarios(void);
void editar_usuario(void);
void consultar_usuario(void);
void excluir_usuario(void);
void menu_usuarios(void);

//QUADRO ACOMPANHAMENTO
void quadro_acompanhamento(void);

//PROJETOS
int listar_projetos(void);
void cadastrar_projeto(void);
void consultar_projeto(void);
void editar_projeto(void);
void excluir_projeto(void);
void atualizar_progresso_projeto(Projeto *projeto, long posicao);
void menu_projetos(void);

//TAREFAS
int dependencia_concluida(int id_dependencia);
int listar_tarefas(int id_projeto);
void cadastrar_tarefa(int id_projeto);
void consultar_tarefa(int id_projeto);
void alterar_status(int id_projeto);
void alterar_prioridade(int id_projeto);
int dependencia_gera_ciclo(int id_tarefa, int id_dependencia);
void editar_campos(int id_projeto);
void excluir_tarefa(int id_projeto);
void alterar_impedimento(int id_projeto);
void menu_tarefas(int id_projeto);

//ESTOQUE
void menu_estoque(void);
void cadastrar_filamento(void);
int listar_filamentos(void);
void consultar_filamento(void);
void registrar_entrada(void);
void registrar_saida(void);
void detalhar_saida(Movimentacao *movimentacao);
void calcular_detalhamento_saida(
    Movimentacao *movimentacao,
    Filamento *filamento
);
void alterar_localizacao_filamento(void);
void excluir_filamento(void);
void listar_movimentacoes(void);
void menu_relatorios(void);
void diario_de_bordo(void);
void relatorio_geral(void);
int confirmar_saida(void);

#endif
