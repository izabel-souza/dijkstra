#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //pra usar o bool
#include <limits.h> // pra usar o INT_MAX

typedef struct vizinho {
	int id_vizinho;
	int peso_aresta;
	struct vizinho *prox;
}TVizinho;

typedef struct grafo{
	int id_vertice;
	TVizinho *prim_vizinho;
	struct grafo *prox;
} TGrafo;


void imprime_grafo(TGrafo *g){
	while(g != NULL){
		printf("Vértice %d\n", g->id_vertice);
		printf("Vizinhos: ");
		TVizinho *v = g->prim_vizinho;
		while(v != NULL){
			printf("%d ", v->id_vizinho);
			v = v->prox;
		}
		printf("\n\n");
		g = g->prox;
	}
}

TGrafo *inicializa_grafo(){
	return NULL;
}

TGrafo* busca_vertice(TGrafo* g, int x){
	while((g != NULL) && (g->id_vertice != x)) {
		g = g->prox;
	}
	return g;
}

TVizinho* busca_aresta(TGrafo *g, int v1, int v2){
	TGrafo *pv1 = busca_vertice(g,v1);
	TGrafo *pv2 = busca_vertice(g,v2);
	TVizinho *resp = NULL;
	//checa se ambos os vértices existem
	if((pv1 != NULL) && (pv2 != NULL)) {
		//percorre a lista de vizinhos de v1 procurando por v2
		resp = pv1->prim_vizinho;
		while ((resp != NULL) && (resp->id_vizinho != v2)) {
			resp = resp->prox;
		}
	}
	return resp;
}

TGrafo *insere_vertice(TGrafo *g, int x){
	TGrafo *p = busca_vertice(g, x);
	if(p == NULL) {
		p = (TGrafo*) malloc(sizeof(TGrafo));
		p->id_vertice = x;
		p->prox = g;
		p->prim_vizinho = NULL;
		g = p;
	}
	return g;
}

void insere_um_sentido(TGrafo *g, int v1, int v2, int peso){
	TGrafo *p = busca_vertice(g, v1);
	TVizinho *nova = (TVizinho *) malloc(sizeof(TVizinho));
	nova->id_vizinho = v2;
	nova->peso_aresta = peso;
	nova->prox = p->prim_vizinho;
	p->prim_vizinho = nova;
}

void insere_aresta(TGrafo *g, int v1, int v2, int peso){
	TVizinho *v = busca_aresta(g, v1, v2);
	if(v == NULL) {
		insere_um_sentido(g, v1, v2, peso);
		insere_um_sentido(g, v2, v1, peso);
    }
}

int quantidade_vertice(TGrafo *g){
	TGrafo *aux = g;
	int cont = 0;
	while (aux){
		cont += 1;
		aux = aux-> prox;
	}
	return cont;
}

void inicializaD(TGrafo *g, int *d, int *p, bool* aberto, int s, int n){	
	for (int i = 0; i < n; i++){
		d[i] = INT_MAX/2;
		p[i] = -1;
		aberto[i] = true;
	}
	d[s] = 0;
}

void relaxa(TGrafo* g, int *d, int *p, int u, TVizinho* v){
	if(d[v->id_vizinho] > d[u] + v->peso_aresta){
		d[v->id_vizinho] = d[u] + v->peso_aresta;
		p[v->id_vizinho] = u;
	}
}

bool existeAberto(TGrafo *g, bool *aberto, int nvertices){
	for(int i = 0; i < nvertices; i++){
		if(aberto[i]) return true;
	}
	return false;
}

int menorDist(TGrafo *g, bool *aberto, int *d, int nvertices){
	int i = 0;
	
	while(!aberto[i] && i < nvertices){
		i++;
	}

	if(i == nvertices) return -1; //nao tem ninguem aberto
	
	int menor = i;
	for(i = menor+1; i < nvertices; i++){
		if(aberto[i] && (d[menor] > d[i])){
			menor = i;
		}
	}
	return menor;
}

//g eh o ponteiro para o grafo
//s vertice inicial
//d eh o vertor de distancias
//p eh o vetor de pais
//nvertices eh a quantidade de vertices do grafo
void dijkstra(TGrafo *g, int s, int *d, int *p, int nvertices) {
	bool aberto[nvertices];
    int n = quantidade_vertice(g);
	inicializaD(g, d, p, aberto, 0, n);

    while(existeAberto(g, aberto, n)) {
        int u = menorDist(g, aberto, d, n); 
        aberto[u] = false;
    
        TGrafo *aux = busca_vertice(g, u);
        TVizinho *v = aux->prim_vizinho;

        while(v) {
        relaxa(g, d, p, u, v);
            v = v->prox;
        }
    }

	return;	
}

void printDistancia(TGrafo *g, int *d){
	
	int nvertices = quantidade_vertice(g);
	for(int i = 0; i < nvertices; i++){
		printf("%d, ", d[i]);
	}
	printf("\n");
}

void printPai(TGrafo *g, int *p){

	int nvertices = quantidade_vertice(g);
	for(int i = 0; i < nvertices; i++){
		printf("%d, ", p[i]);
	}
	printf("\n");
}

int main (void) {

	TGrafo *grafo; 
	grafo = inicializa_grafo();
	grafo = insere_vertice(grafo, 9);

    grafo = insere_vertice(grafo, 8);
    grafo = insere_vertice(grafo, 7);
    grafo = insere_vertice(grafo, 6);
    grafo = insere_vertice(grafo, 5);
    grafo = insere_vertice(grafo, 4);
    grafo = insere_vertice(grafo, 3);
    grafo = insere_vertice(grafo, 2);
    grafo = insere_vertice(grafo, 1);
    grafo = insere_vertice(grafo, 0);

    insere_um_sentido(grafo, 0, 2, 5);  
    insere_um_sentido(grafo, 0, 1, 1);
    insere_um_sentido(grafo, 1, 3, 2);
    insere_um_sentido(grafo, 3, 5, 10);
    insere_um_sentido(grafo, 3, 2, 1);    
    insere_um_sentido(grafo, 2, 4, 3);
    insere_um_sentido(grafo, 4, 9, 10);
    insere_um_sentido(grafo, 4, 5, 5);
    insere_um_sentido(grafo, 0, 6, 10);
    insere_um_sentido(grafo, 6, 7, 5);
    insere_um_sentido(grafo, 7, 4, 2);
    insere_um_sentido(grafo, 7, 8, 1);
    insere_um_sentido(grafo, 8, 9, 1);
	
    imprime_grafo(grafo);
    printf("-----------------------------------------\n\n");
	//largura(grafo);

	int nvertices = quantidade_vertice(grafo);
    int *d = (int *) malloc(nvertices*sizeof(int));
    int *p = (int *) malloc(nvertices*sizeof(int));

	dijkstra(grafo, 0, d, p, nvertices);
	printDistancia(grafo, d);
	printPai(grafo, p);

    return 0;
}