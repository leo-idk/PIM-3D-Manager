#include "structs.h"

int prox_id_usuario = 1;
int id_usuario_logado = 0;
int nivel_usuario_logado = 0;
int prox_id_projeto = 1;
int prox_id_tarefa = 1;
int prox_id_filamento = 1;
int prox_id_movimentacao = 1;

const char *MAPEAMENTO_STATUS_PROJ[] = {
    "Nao iniciado", "Em desenvolvimento", "Concluido"
};
const char *MAPEAMENTO_STATUS[] = {
    "A Fazer", "Fazendo", "Feito", "Impedida"
};
const char *MAPEAMENTO_TIPO[] = {
    "Modelagem", "Impressao", "Acabamento", "Montagem", "Reparo"
};
const char *MAPEAMENTO_PRIORIDADE[3] = {
    "Baixa", "Media", "Alta"
};
