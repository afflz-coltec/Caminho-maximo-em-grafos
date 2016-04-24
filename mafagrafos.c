#include <stdio.h>
#include <stdlib.h>

//#define DEBUG // DEBUGS GERAIS NO CODIGO
//#define READER // PRINTS RELATIVOS A GRAPH READER
//#define LIBERATOR // PRINTS DO LIBERADOR
//#define PATHFINDER // PRINTS DA PATH FINDER
//#define ESTETICO // PRINTS QUE DEIXAM O PROGRAMA MAIS BONITO
//#define INCEPTION // PRINTS NO NIVEL DE RECURSÃO

int MAX_TIME = 60;
int TIME_TO_DECIDE = 1;

typedef struct data_bag
{
    struct fila_vertices * this_way;
    int crit_way[50];
    int current_elapsed_time;
    int tasks_max;
    int tasks;
    int max_elapsed_time;
} data_bag;

typedef struct vertice
{
    int code;
    int time_to_finish;
    char nome[50];
    struct fila_vertices *adjacentes;
} vertice;

typedef struct vertice_holder
{
    struct vertice *vertice_atual;
    struct vertice_holder *proximo_vertice;
} vertice_holder;

typedef struct fila_vertices
{
    struct  vertice_holder *tail;
    struct  vertice_holder *head;
} fila_vertices;

//_______FUNÇOES DE ABERTURA DE ARQUIVO______________________________________________________________________________________

FILE * abrearquivo_readonly(char *nomedoarquivo)
{
    FILE *arquivo;

    arquivo = fopen(nomedoarquivo,"r");

    if (!arquivo)
    {
        printf("Não foi\n\n");
        exit(-1);
    }
    else
    {
        return arquivo;
    }
}

FILE * abrearquivo_readwrite(char *nomedoarquivo)
{
    FILE *arquivo;

    arquivo = fopen(nomedoarquivo,"a+");
    if (!arquivo)
    {
        printf("Não foi\n\n");
        exit(-1);
    }
    else
    {
        return arquivo;
    }
}

FILE * novoarquivo_readwrite(char *nomedoarquivo)
{
    FILE *arquivo;

    arquivo = fopen(nomedoarquivo,"w+");

    if (!arquivo)
        {
        printf("Não foi\n\n");
        exit(-1);
        }
    else
    {
        return arquivo;
    }
}

//_______FUNÇÕES DE STRINGS__________________________________________________________________________________

void catastring_arquivo (char *string, FILE *arquivo)
{
    int aux=0;
    char string_aux[50];

    while (!feof(arquivo))
    {
        if ((string_aux[aux]=fgetc(arquivo)) != ' ')
        {
            string[aux]=string_aux[aux];
            aux++;
        }
        else
            break;
    }

    string[aux]='\0';
}

void catastring (char *string)
{
    int aux=0;

    while ((string[aux]=getchar()) != '\n')
        aux++;

    string[aux]='\0';
}

void botastring (char *string)
{
    int aux=0;

    while ((putchar(string[aux]))!='\0')
        aux++;

    printf("\n");
}

int contastring(char *string)
{
    int count=0;

    while (string[count]!='\0')
        count++;
    return count;
}

void igualastring(char *string_origem,char *string_destino)
{
    int aux=0;
    while (aux<contastring(string_origem))
    {
        string_destino[aux]=string_origem[aux];
        aux++;
    }
    string_destino[aux]='\0';
}

char * junta_strings(char *string_a,char *string_b)
{
    int count_a = contastring(string_a);
    int count_b = contastring(string_b);
    int i,j;
    char *string_c = (char *) malloc((count_a-1+count_b)*sizeof(char));

    for(i=0; i<count_a; i++)
    {
        string_c[i]=string_a[i];
    }
    for (j=0; i<count_b+count_a; j++, i++)
    {
        string_c[i]=string_b[j];
    }
    string_c[i] = '\0';

    return (string_c);

}

//______FUNÇÕES DE IMPRESSÃO_____________________________________________________________________________________

void imprimir_inception (int level_of_inception)
{
    int i,j;

    for (j=0; j<level_of_inception; j++)
        printf("--->");
    printf("NIVEL %d\n",level_of_inception);

}

void imprimir_caminho (vertice_holder *caminho)
{
    vertice_holder *passeio;

    printf("CAMINHO :");

    for(passeio = caminho; ; passeio = passeio->proximo_vertice)
    {
        if (passeio->vertice_atual->code != 42)
        printf("->%s",passeio->vertice_atual->nome);
        if (!(passeio->proximo_vertice)) break;
    }
    printf("\n");
}

void imprimir_caminho_final(data_bag *db,vertice_holder *pointer_rep)
{
    int i=0;
    vertice_holder *passeio;

    for(i = 0;i < db->tasks_max;i++)
    {
        for(passeio = pointer_rep; ; passeio = passeio->proximo_vertice)
        {
            if (!(passeio)) break;
            if (passeio->vertice_atual->code == db->crit_way[i])
            {
                printf("->%s",passeio->vertice_atual->nome);
                break;
            }
            if (!(passeio->proximo_vertice)) break;
        }
    }
    printf("<--\n");

}

//____FUNÇOES DE APOIO______________________________________________________________________________________

data_bag * db_cria ()
{
    data_bag *db = (data_bag*) malloc(sizeof(data_bag));
    db->current_elapsed_time=0;
    db->max_elapsed_time=0;
    db->tasks = 1;
    db->tasks_max = 1;
    db->this_way = (fila_vertices*) malloc(sizeof(fila_vertices));

    return db;
}

void encher_bolsa (data_bag *db,vertice_holder * atual)
{
vertice_holder *novo_elemento;

                novo_elemento = (vertice_holder *) malloc(sizeof(vertice_holder));
                novo_elemento->vertice_atual = atual->vertice_atual;
                novo_elemento->proximo_vertice = NULL;

                if (db->this_way->head)
                {
                    db->this_way->head->proximo_vertice = novo_elemento;
                    db->this_way->head = novo_elemento;
                    db->current_elapsed_time += atual->vertice_atual->time_to_finish;
                }
                else
                {
                    db->this_way->head = novo_elemento;
                    db->this_way->tail = novo_elemento;
                    db->current_elapsed_time += atual->vertice_atual->time_to_finish;
                }
                #ifdef DEBUG
                printf("DEBUG PRINT encheu a bolsa\n");
            #endif // DEBUG
}

void esvasiar_bolsa (data_bag *db,vertice_holder *atual)
{
vertice_holder *passeio,*novo_elemento;

            novo_elemento = db->this_way->head;
            //faz um line_pop retornando a cabeça antiga da fila
            if(db->this_way->tail != db->this_way->head)
                for(passeio = db->this_way->tail;
                        passeio->proximo_vertice != db->this_way->head;
                        passeio = passeio->proximo_vertice);
            else
                passeio = db->this_way->tail;


                #ifdef DEBUG
                    printf("DEBUG PRINT organizou a bolsa\n");
                #endif // DEBUG

                //remove o elemento atual da bag pra voltar
                db->this_way->head = passeio;
                db->this_way->head->proximo_vertice = NULL;
                db->current_elapsed_time -= atual->vertice_atual->time_to_finish;
                #ifdef DEBUG
                    printf("DEBUG PRINT esvziou a bolsa\n");
                #endif // DEBUG
    free(novo_elemento);
}

void declarar_vitoria (data_bag *db,vertice_holder *atual)
{
encher_bolsa(db,atual);
vertice_holder *passeio;
int i = 0;

                #ifdef ESTETICO
                    printf("\n");
                    printf("\t*** Novo caminho critico encontrado ***\n");
                    imprimir_caminho(db->this_way->tail);
                    printf("\n");
                #endif // DEBUG
                db->tasks_max = db->tasks;


                for(passeio = db->this_way->tail; ; passeio = passeio->proximo_vertice)
                {
                    db->crit_way[i] = passeio->vertice_atual->code;
                    i++;
                    if (!(passeio->proximo_vertice)) break;
                }

                db->max_elapsed_time = db->current_elapsed_time;
esvasiar_bolsa(db,atual);
}

//_____FUNÇOES PRINCIPAIS____________________________________________________________________________________________

//EXPLORA O GRAFO RECURSIVAMENTE
        void path_finder (vertice_holder *atual,data_bag *db,int level_of_inception)
        {
            int soma_tempo_atual;
            int checagem;
            int reincidente_check = 0;
            vertice_holder *passeio;
            vertice_holder *novo_elemento;

        #ifdef PATHFINDER
            printf("\n\t***Entrou na pathfinder ***\n\n");
        #endif // PATHFINDER

        #ifdef INCEPTION
        imprimir_inception(level_of_inception);
        #endif // INCEPTION

        #ifdef PATHFINDER
            printf("PATHFINDER PRINT chegou em %s\n",atual->vertice_atual->nome);
        #endif // PATHFINDER

            vertice_holder *tempo_decisao = (vertice_holder *) malloc (sizeof(vertice_holder));
            tempo_decisao->vertice_atual = (vertice *) malloc (sizeof(vertice));
            tempo_decisao->vertice_atual->code = 42;
            igualastring("tomada de tempo para decisao",tempo_decisao->vertice_atual->nome);
            tempo_decisao->vertice_atual->time_to_finish = TIME_TO_DECIDE;

            if (atual->vertice_atual->code == 42)
                return;

            #ifdef PATHFINDER
                printf("PATHFINDER PRINT analisando reincidencias \n");
            #endif // PATHFINDER
            //testa se este vertice já foi analisado antes
            for(passeio = db->this_way->tail; ; passeio = passeio->proximo_vertice)
            {
                if (!(passeio)) break;
                if(passeio->vertice_atual->code == atual->vertice_atual->code)
                {
                    reincidente_check = 1;
                    break;
                }
                if (!(passeio->proximo_vertice)) break;
            }

        #ifdef PATHFINDER
            printf("PATHFINDER PRINT reincidencias e outros fatores analisados\n");
        #endif // PATHFINDER

            //caso seja um vertice ainda não conhecido
            if(reincidente_check == 0)
            {
                soma_tempo_atual = db->current_elapsed_time + atual->vertice_atual->time_to_finish;

                free(tempo_decisao->vertice_atual->adjacentes);
                free(tempo_decisao->vertice_atual);
                free(tempo_decisao);

                //caso o tempo atual estoure o tempo maximo
                if (soma_tempo_atual > MAX_TIME)
                {
                    #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no > max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
                    #endif // PATHFINDER

                    //se o caminho atual for mais longo que o anterior
                    if (db->current_elapsed_time > db->max_elapsed_time && db->current_elapsed_time < MAX_TIME)
                        if (db->tasks > db->tasks_max)
                            declarar_vitoria(db,atual);
                    return;
                }
                //caso o tempo atual seja o tempo desejado, ou seja o caminho ideal
                else if(soma_tempo_atual == MAX_TIME)
                {
                    #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no == max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
                    #endif // PATHFINDER
                    if (db->tasks >= db->tasks_max)
                    {
                        declarar_vitoria(db,atual);
                    }
                    return;
                }
                //caso o tempo atual não estoure o tempo maximo
                else if (soma_tempo_atual < MAX_TIME)
                {
        #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no < max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
        #endif // PATHFINDER
                    encher_bolsa(db,atual);
                    db->tasks++;

                    if (atual->vertice_atual->adjacentes)
                        for (passeio = atual->vertice_atual->adjacentes->tail; passeio != NULL; passeio=passeio->proximo_vertice)
                        {
                            #ifdef PATHFINDER
                            printf("PATHFINDER PRINT foi fazer %s\n",passeio->vertice_atual->nome);
                            #endif // PATHFINDER
                            path_finder(passeio,db,level_of_inception+1);
                            #ifdef PATHFINDER
                            printf("PATHFINDER PRINT voltou\n");
                            #endif // PATHFINDER
                            #ifdef INCEPTION
                            imprimir_inception(level_of_inception);
                            #endif // INCEPTION

                        }
                    esvasiar_bolsa(db,atual);
                    db->tasks--;


                    return;
                }
            }
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            else if (reincidente_check == 1)
            {
                #ifdef PATHFINDER
                    printf("PATHFINDER PRINT reincidente\n");
                #endif // PATHFINDER

                 soma_tempo_atual = db->current_elapsed_time + tempo_decisao->vertice_atual->time_to_finish;

                if (soma_tempo_atual > MAX_TIME)
                {
                    #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no > max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
                    #endif // PATHFINDER

                    //se o caminho atual for mais longo que o anterior
                    if (db->current_elapsed_time > db->max_elapsed_time && db->current_elapsed_time < MAX_TIME)
                        if (db->tasks > db->tasks_max)
                            declarar_vitoria(db,tempo_decisao);
                    return;
                }
                else if(soma_tempo_atual == MAX_TIME)
                {
                    #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no == max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
                    #endif // PATHFINDER

                    if (db->tasks >= db->tasks_max)
                        declarar_vitoria(db,tempo_decisao);

                    return;
                }
                //caso o tempo atual não estoure o tempo maximo
                else if (soma_tempo_atual < MAX_TIME)
                {
                    #ifdef PATHFINDER
                    printf("PATHFINDER PRINT entrou no < max\n");
                    printf("soma_tempo_atual = %d MAX_TIME = %d\n",soma_tempo_atual,MAX_TIME);
                    #endif // PATHFINDER

                    encher_bolsa(db,tempo_decisao);

                    if (!(!atual->vertice_atual->adjacentes))
                        for (passeio = atual->vertice_atual->adjacentes->tail; passeio != NULL; passeio=passeio->proximo_vertice)
                        {
                            #ifdef PATHFINDER
                                printf("PATHFINDER PRINT foi fazer %s\n",passeio->vertice_atual->nome);
                            #endif // PATHFINDER
                            path_finder(passeio,db,level_of_inception+1);
                            #ifdef PATHFINDER
                                printf("PATHFINDER PRINT voltou\n");
                            #endif // PATHFINDER
                            #ifdef INCEPTION
                            imprimir_inception(level_of_inception);
                            #endif // INCEPTION

                        }
                    esvasiar_bolsa(db,tempo_decisao);
                }
            }

            return;
        }

//CRIA UM ARQUIVO PADRONIZADO PRA LEITURA COM AS INFORMAÇÕIES DO GRAFO
        void criador_grafos (char *nome_grafo)
        {
            int contador = 0,i,forty_two = 42;
            char check = 'o';
            vertice escrita;

            printf("Antes de definir sua sitação de bomba, anote as informações necessárias:\n");
            printf("Nome da atividade,código da atividade(Não sendo 42 ou zero),tempo pra terminar :\n");

            printf("Dê um nome ao arquivo que vai salvar suas informações :\n");
            __fpurge(stdin);
            catastring(nome_grafo);

            FILE *grafile = novoarquivo_readwrite(nome_grafo);
            FILE *adjfile = novoarquivo_readwrite(junta_strings(nome_grafo,"_adj"));

            while (check != 'n')
            {
                printf("Digite o nome da atividade : ");
                __fpurge(stdin);
                catastring (escrita.nome);
                printf("\n");

                escrita.code = contador + 1;
                printf("O cdigo gerado para essa atividade : %d\n",escrita.code);

                printf("Digite o tempo para acabar a atividade : ");
                scanf ("%d",&escrita.time_to_finish);
                printf("\n");

                printf("Atividade : %s; Codigo : %d; Duração : %d\n",escrita.nome,escrita.code,escrita.time_to_finish);
                fprintf(grafile,"%d %s %d\n",escrita.code,escrita.nome,escrita.time_to_finish);

                printf("Deseja adicionar mais atividades?(s/n)\n");
                __fpurge(stdin);
                scanf("%c",&check);

                contador++;
            }
            system("clear");
            check = 'o';
            fprintf(grafile,"%d",forty_two);
            for (i = 0;i <= contador;i++)
            {
                printf("Agora vamos ajustar as conexões entre as atividades;\n");
                printf("Começando pelo centro, que é começar a estudar\n");
                while (check != 'n')
                {
                    printf("Digite o codigo da atividade conexa a atividade %d: ",i);
                    scanf ("%d",&escrita.code);
                    printf("\n");

                    fprintf(adjfile,"%d ",escrita.code);

                    printf("Deseja adicionar mais conexões?(s/n)\n");
                    __fpurge(stdin);
                    scanf("%c",&check);
                }
                system("clear");
                check = 'o';

                fprintf(adjfile,"%d\n",forty_two);

            }

            fclose(grafile);
            fclose(adjfile);
        }

//LE E ALOCA O GRAFO A PARTIR DE UM ARQUIVO(função classificada como disparadamente mais chata)
        vertice_holder * graph_reader (char *nome_grafo)
        {
            int adj_code,aux;

            //criando as definições do vertice de partida
            vertice_holder *vertice_central = (vertice_holder *) malloc (sizeof(vertice_holder));
            vertice_central->vertice_atual = (vertice *) malloc (sizeof(vertice));
            vertice_central->vertice_atual->code = 0;
            igualastring("Começar a estudar",vertice_central->vertice_atual->nome);
            vertice_central->vertice_atual->time_to_finish = 0;
            vertice_central->vertice_atual->adjacentes = (fila_vertices *) malloc (sizeof(fila_vertices));
            vertice_central->vertice_atual->adjacentes->head = (vertice_holder *) malloc (sizeof(vertice_holder));
            vertice_central->vertice_atual->adjacentes->tail = vertice_central->vertice_atual->adjacentes->head;
            vertice_central->vertice_atual->adjacentes->tail->vertice_atual=NULL;

            #ifdef READER
            printf("READER PRINT mallocou o vertice central\n");
            #endif // READER

            vertice_holder *novo_vertice,*passeio,*passeio_local;

            FILE *grafile = abrearquivo_readonly(nome_grafo);
            FILE *adjfile = abrearquivo_readonly(junta_strings(nome_grafo,"_adj"));

            fila_vertices *leitura = (fila_vertices *) malloc (sizeof(fila_vertices));
            leitura->tail = vertice_central;
            leitura->head = vertice_central;
            leitura->head->proximo_vertice = NULL;

            #ifdef READER
            printf("READER PRINT mallocou todas as estruturas de suporte\n");
            #endif // READER

            // lendo as informações de cada vertice
            while(!feof(grafile))
            {
                novo_vertice = (vertice_holder *) malloc (sizeof(vertice_holder));
                novo_vertice->vertice_atual = (vertice *) malloc (sizeof(vertice));
                novo_vertice->vertice_atual->adjacentes = (fila_vertices *) malloc (sizeof(fila_vertices));

                fscanf(grafile,"%d ",&aux);
                if (aux != 42)
                    novo_vertice->vertice_atual->code=aux;
                else
                    break;

                catastring_arquivo(novo_vertice->vertice_atual->nome,grafile);
                fscanf(grafile,"%d",&novo_vertice->vertice_atual->time_to_finish);

                novo_vertice->vertice_atual->adjacentes->head = (vertice_holder *) malloc (sizeof(vertice_holder));
                novo_vertice->vertice_atual->adjacentes->tail = novo_vertice->vertice_atual->adjacentes->head;
                novo_vertice->vertice_atual->adjacentes->tail->vertice_atual=NULL;


                leitura->head->proximo_vertice = novo_vertice;
                leitura->head = novo_vertice;
                leitura->head->proximo_vertice = NULL;

            }
            #ifdef READER
            printf("READER PRINT Vertices lidos e aguardando processamento\n");
            #endif // READER

            // lendo as adjacencias de cada vertice
            while (!feof(adjfile))
            {
                #ifdef READER
                printf("READER PRINT lendo adjacencias\n");
                #endif // READER
                //repetindo para todos os vertices
                for (passeio = leitura->tail; passeio != NULL; passeio = passeio->proximo_vertice)
                {
                    #ifdef READER
                    printf("READER PRINT passeio pelo vertice %s \n",passeio->vertice_atual->nome);
                    #endif // READER

                    while(1)
                    {
                        //scaneando codigo do elemento
                        fscanf(adjfile,"%d",&adj_code);
                        if(adj_code == 42)
                        {
                            break;
                        }

                        //procurando elemento
                        for (passeio_local = leitura->tail; passeio_local != NULL; passeio_local = passeio_local->proximo_vertice)
                        {
                            #ifdef READER
                            printf("\tREADER PRINT passeio pela adjacencia %s\n",passeio_local->vertice_atual->nome);
                            if(passeio_local->proximo_vertice)
                                printf("\tREADER PRINT a seguir %s\n",passeio_local->proximo_vertice->vertice_atual->nome);
                            #endif // READER
                            //testando
                            if(passeio_local->vertice_atual->code == adj_code)
                            {
                                #ifdef READER
                                printf("\t\tREADER PRINT juntando as pontas de %s com %s\n",passeio->vertice_atual->nome,passeio_local->vertice_atual->nome);
                                #endif // READER
                                //posicionando
                                if(!(passeio->vertice_atual->adjacentes->tail->vertice_atual))
                                {
                                    passeio->vertice_atual->adjacentes->tail->vertice_atual = passeio_local->vertice_atual;
                                    break;
                                }
                                else
                                {
                                    novo_vertice = (vertice_holder *) malloc (sizeof(vertice_holder));
                                    novo_vertice->proximo_vertice = NULL;
                                    novo_vertice->vertice_atual=passeio_local->vertice_atual;
                                    passeio->vertice_atual->adjacentes->head->proximo_vertice = novo_vertice;
                                    passeio->vertice_atual->adjacentes->head = novo_vertice;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            free (leitura);
            fclose(grafile);
            fclose(adjfile);
            return vertice_central;
        }

//LIBERA AS ESTRUTURAS ALOCADAS PRO PROGRAMA
        void graph_liberator(vertice_holder *libera)
        {
            vertice_holder *passeio,*liberado,*liberado_local,*passeio_local;

            #ifdef LIBERATOR
            printf("LIBERATOR PRINT entrou no liberador\n");
            #endif // LIBERATOR

            liberado = libera;

            for (passeio = libera->proximo_vertice; passeio != NULL; passeio = passeio->proximo_vertice)
            {
                #ifdef LIBERATOR
                printf("LIBERATOR PRINT liberando \\o/\n");
                #endif // LIBERATOR
                liberado_local = passeio->vertice_atual->adjacentes->tail;
                for (passeio_local = passeio->vertice_atual->adjacentes->tail->proximo_vertice;passeio_local != NULL; passeio_local = passeio_local->proximo_vertice)
                {
                    free(liberado_local);
                    #ifdef LIBERATOR
                    printf("\tLIBERATOR PRINT liberou tudo \\o/\n");
                    #endif // LIBERATOR

                    liberado_local = passeio_local;
                }
                free(liberado_local);
        //--------------------------------------------------
                free(liberado->vertice_atual->adjacentes);
                #ifdef LIBERATOR
                printf("LIBERATOR PRINT liberou adjacencias \\o/\n");
                #endif // LIBERATOR
                free(liberado->vertice_atual);
                #ifdef LIBERATOR
                printf("LIBERATOR PRINT liberou vertice \\o/\n");
                #endif // LIBERATOR
                free(liberado);
                #ifdef LIBERATOR
                printf("LIBERATOR PRINT liberou tudo \\o/\n");
                #endif // LIBERATOR
                liberado = passeio;
            }
            free(liberado->vertice_atual->adjacentes);
            free(liberado->vertice_atual);
            free(liberado);

            free(libera);
        }

//____________________________________________________________________________________________

void main (void)
{
    int menu;
    char nome_grafo[50];
    vertice_holder *vertice_central;
    fila_vertices *libera;
    data_bag *db = db_cria();


    printf("   ____               _____                 \n"    );
    printf("  / __ \\          _  |  __ \\   \n"               );
    printf(" | |  | | _ __   (_) | |__) |__ _  ___  ___ \n"    );
    printf(" | |  | || '_ \\      |  ___// _` |/ __|/ __|\n"   );
    printf(" | |__| || |_) |  _  | |   | (_| |\\__ \\\\__ \\\n");
    printf("  \\____/ | .__/  (_) |_|    \\__,_||___/|___/\n"  );
    printf("         | |                                \n"    );
    printf("         |_|                                \n"    );


    while (menu != 10)
    {
        printf("Deseja criar um novo esquemático anti-bomba ou prefere abrir um já criado?\n");
        printf("1 - NOVO\n2 - JÁ CRIADO\n");
        scanf("%d",&menu);

        if(menu == 1)
        {
            criador_grafos(nome_grafo);
        }
        else if (menu == 2)
        {
            __fpurge(stdin);
            printf("Digite o nome do seu arquivo : ");
            catastring(nome_grafo);
            printf("\n");
            vertice_central = graph_reader(nome_grafo);
            printf ("Agora, o mais importante, quanto tempo lhe resta pra se desafogar?\n");
            scanf("%d",&MAX_TIME);
            printf ("E quanto tempo você demora pra decidir?\n");
            scanf("%d",&TIME_TO_DECIDE);
            menu = 10;
        }
        system("clear");
    }

    path_finder(vertice_central,db,0);

    printf("O caminho em que mais tarefas foram feitas dentro do tempo estipulado é : \n");

    imprimir_caminho_final (db,vertice_central);

    graph_liberator(vertice_central);

    free (db->this_way);
    free (db);
}

