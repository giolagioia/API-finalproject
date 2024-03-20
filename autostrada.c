#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct nodo{
    int distanza;  //rappresenta la distanza da 0 della stazione
    int numero_veicoli;
    int autonomia[512];
    int steps;
    struct nodo* dad;
    struct nodo* left;
    struct nodo* right;
};
typedef struct nodo *albero;

struct blocco{
    int distanza;
    struct blocco* next;
};
typedef struct blocco *lista;

albero root=NULL;   //dichiara che l'albero radice parte vuoto

albero crea_stazione(int distanza, int numero_veicoli, int autonomie[]);
void aggiungi_stazione(int distanza, int numero_veicoli, int autonomie[]);
albero successore(albero x);
albero predecessore(albero x);
void demolisci_stazione(int distance);
void aggiungi_auto(int distance, int valore);
void rottama_auto(int distance, int valore);
void pianifica_percorso(int partenza, int arrivo);
int  max_autonomia(int array[], int elementi);  //restituisce il max da un puntatore array
lista inserisci_intesta(lista l, int el);
void stampa(lista l);
albero trova_stazione(int el);


int main() {
   FILE *f1=stdin;
  // FILE *f1=fopen ("/Users/giovanni/Documents/Università/Corsi da superare/Progetto finale/open_101.txt", "r");
    char comparison[20];
    int distanza;
    int numero_veicoli;
    int autonomie[512];
    int valore;
    int start,end;

    while(fscanf(f1,"%s",comparison) != EOF)
    {
        if(strcmp(comparison, "aggiungi-stazione")==0)
        {
            if (fscanf(f1,"%d %d", &distanza, &numero_veicoli)!=EOF)
            for(int i=0; i<numero_veicoli; i++)
            {
                assert(fscanf(f1, "%d", &autonomie[i]));
            }
            aggiungi_stazione(distanza, numero_veicoli, autonomie);
        }
        if(strcmp(comparison, "demolisci-stazione")==0)
        {
            if(fscanf(f1,"%d", &distanza)!=EOF)
            demolisci_stazione(distanza);
        }
        if(strcmp(comparison, "aggiungi-auto")==0)
        {
            if(fscanf(f1,"%d %d", &distanza, &valore)!=EOF)
            aggiungi_auto(distanza, valore);
        }
        if(strcmp(comparison, "rottama-auto")==0)
        {
            if(fscanf(f1,"%d %d", &distanza, &valore)!=EOF)
            rottama_auto(distanza, valore);
        }
        if(strcmp(comparison, "pianifica-percorso")==0)
        {
            if(fscanf(f1,"%d %d", &start, &end)!=EOF)
            pianifica_percorso(start, end);
        }

    }
    fclose(f1);
    return 0;
}

albero trova_stazione(int el)
{
    albero x=root;
    while(x!=NULL)
    {
        if(x->distanza>el)
            x=x->left;
        else if(x->distanza<el)
            x=x->right;
        else if (x->distanza==el)
            break;
    }
    return x;
}

albero crea_stazione(int distanza, int numero_veicoli, int autonomie[])
{
    struct nodo *nuovo=malloc(sizeof(struct nodo));
    nuovo->distanza=distanza;
    for(int i=0; i<numero_veicoli; i++)
    {
        nuovo->autonomia[i]=autonomie[i];
    }
    nuovo->numero_veicoli=numero_veicoli;
    nuovo->dad=NULL;
    nuovo->left=NULL;
    nuovo->right=NULL;
    return nuovo;
}

void aggiungi_stazione(int distanza, int numero_veicoli, int autonomie[])
{
    albero z= crea_stazione(distanza, numero_veicoli, autonomie);
    albero y=NULL;
    albero x=root;

    if(root==NULL)
    {
        root=z;
    }
    else
    {
        while(x!=NULL)
        {
            y=x; //salvo in y l'ultimo nodo visitato
            if(z->distanza==x->distanza)
                {printf("non aggiunta\n");
                return;}
            if(z->distanza<x->distanza)
                x=x->left;
            else
                x=x->right;
        }
        z->dad=y;
        if(z->distanza<y->distanza)
            y->left=z;
        else if(z->distanza>y->distanza)
            y->right=z;
    }
    printf("aggiunta\n");
}

albero successore(albero x)
{
    if (x->right!=NULL && x!=NULL)
    {
        x=x->right;
        while (x->left!=NULL)    //minimo dell'albero
            x=x->left;
        return x;
    }
    albero y=x->dad;  //successore di x è il primo elemento y risalendo da x tale che x è nel sotto albero sinistro di y
    while(y!=NULL && x==y->right)
    {
        x=y;
        y=y->dad;
    }
    return y;
}

albero predecessore(albero x)
{
    if (x->left!=NULL)
    {
        x=x->left;
        while (x->right!=NULL)    //massimo dell'albero
            x=x->right;
        return x;
    }
    albero y=x->dad;  //predecessore di x è il primo elemento y risalendo da x tale che x è nel sotto albero destro di y
    while(y!=NULL && x==y->left)
    {
        x=y;
        y=y->dad;
    }
    return y;
}

void demolisci_stazione(int distance)  //ha senso fare una free per la demolizione delle stazion?
{
    albero x=root;
    albero y,z;  //y effettivo indirizzo da eliminare
    while(x!=NULL)
    {
        if(x->distanza>distance)
            x=x->left;
        else if(x->distanza<distance)
            x=x->right;
        else if (x->distanza==distance)
            break;
    }
    if(x==NULL)
    {
        printf("non demolita\n");
        return;
    }
    if((x->left==NULL || x->right==NULL))
        y=x;
    else
        y=successore(x);
    if (y->left!=NULL)
        z=y->left;
    else
        z=y->right;
    if(z!=NULL)
        z->dad=y->dad;
    if (y->dad==NULL)
        root=z;
    else if(y==y->dad->left)  //cioè se y è figlio sinistro nella catena
        y->dad->left=z;
    else
        y->dad->right=z;
    if(y!=x) //ricopio tutti i valori
    {
        x->distanza = y->distanza;
        int i;
        for(i=0; i<y->numero_veicoli; i++)
            x->autonomia[i]=y->autonomia[i];
        x->steps=y->steps;
        x->numero_veicoli=y->numero_veicoli;
    }
    free(y);
    printf("demolita\n");
}

void aggiungi_auto(int distance, int valore)
{
    albero x=root;
    while(x!=NULL)
    {
        if(x->distanza>distance)
            x=x->left;
        else if(x->distanza<distance)
            x=x->right;
        else if (x->distanza==distance)
            break;
    }
    if(x==NULL || x->numero_veicoli==512)
    {
        printf("non aggiunta\n");
        return;
    }

    //if(x->autonomia[x->numero_veicoli]==0)
        x->autonomia[x->numero_veicoli]=valore;
 /*   else
    {
        int i=0;
        while(x->autonomia[i]!=0 && i<512)
            i++;
        x->autonomia[i]=valore;
    }*/
    x->numero_veicoli++;
    printf("aggiunta\n");
}

void rottama_auto(int distance, int valore)  //basta metterlo a zero o devo scorrere tutti i successivi al prec?
{
    albero x=root;
    int i=0;
    while(x!=NULL)
    {
        if(x->distanza>distance)
            x=x->left;
        else if(x->distanza<distance)
            x=x->right;
        else if (x->distanza==distance)
            break;
    }
    if(x==NULL)
    {
        printf("non rottamata\n");
        return;
    }
    while(i<512 && x->autonomia[i]!=valore)
         i++;
    if (i==512)
        printf("non rottamata\n");
    else if (x->autonomia[i]==valore)
    {
        x->autonomia[i]=0;
        x->numero_veicoli--;
        int j;
        for(j=i; j<x->numero_veicoli; j++)
            x->autonomia[j]=x->autonomia[j+1];
        printf("rottamata\n");
    }
}

int max_autonomia(int array[], int elementi)
{
    int max=array[0];
    for(int i=1; i<elementi; i++)
    {
        if(array[i]>max)
            max=array[i];
    }
    return max;
}

void pianifica_percorso(int partenza, int arrivo)
{
    if(partenza==arrivo){
        printf("%d\n", partenza);
        return;}

    lista myl=NULL; //lista in cui inserisco il percorso definitivo

    if(partenza<arrivo)
    {
        albero x=trova_stazione(partenza);
        albero y=trova_stazione(arrivo);
        if(x==NULL || y==NULL){
            printf("nessun percorso\n");
            return;
        }

        int confronto;  //confronta tutti i passi minimi di ogni stazioni x
        x->steps=0;  //inizializzo a zero il numero di passi minimi per arrivare alla stazione di partenza
        albero raggiungibile= successore(x); //segna l'ultima stazione non raggiunta

        while(x!=y) //marchia tutte le stazioni con i passi necessari per arrivare finchè non arrivo all'arrivo
        {
            confronto = x->steps;
            while ((raggiungibile->distanza - x->distanza) > max_autonomia(x->autonomia, x->numero_veicoli) &&
                   x != raggiungibile && confronto == x->steps) //salvo in x la prima stazione disponibile che mi permette di arrivare a quella non raggiunta
                x = successore(x); //scorro gli x raggiunti per arrivare al primo elemento non raggiunto

            if (x == raggiungibile)  //cioè se x non riesce ad arrivare al primo elemento non raggiunto se non da se stesso
            {
                printf("nessun percorso\n");
                return;
            }

            while (raggiungibile != y && (raggiungibile->distanza - x->distanza) <= max_autonomia(x->autonomia, x->numero_veicoli))
            { //finchè da x posso raggiungere una stazione marco il numero di passi necessari
                raggiungibile->steps = x->steps + 1;
                raggiungibile = successore(raggiungibile);
            }
            if (raggiungibile == y && (raggiungibile->distanza - x->distanza) <= max_autonomia(x->autonomia, x->numero_veicoli))
            {
                raggiungibile->steps = x->steps + 1;
                break;
            }

            x= successore(x);
        }

        myl= inserisci_intesta(myl, y->distanza); //iserisco in testa l'ultimo elemento
        int diff=y->distanza; //salvo l'ultimo elemento aggiunto nella lista
        confronto=y->steps;
        while(y!=NULL && y->distanza!=partenza)
        {
            while(y->steps==confronto && y!=NULL) //scorro dalla fine finchè non trovo il primo con n-1 passi
            {
                y=predecessore(y);
            }

            confronto=y->steps;
            int minimo=2147483647;
            if((diff- y->distanza) <= max_autonomia(y->autonomia,y->numero_veicoli))
                minimo=y->distanza;
            while(y->distanza!=partenza && y->steps==confronto) //scorro tutti gli n-1 passi per trovare il più piccolo che arriva all'ultima stazione del percorso definitivo
            {
                y=predecessore(y);
                if((diff- y->distanza) <= max_autonomia(y->autonomia,y->numero_veicoli) && y->distanza<minimo)
                    minimo = y->distanza;
            }
            diff=minimo;
            myl=inserisci_intesta(myl,minimo);
        }
    }

    if(partenza>arrivo)
    {
        albero x=trova_stazione(partenza);
        albero y=trova_stazione(arrivo);
        if(x==NULL || y==NULL){
            printf("nessun percorso\n");
            return;
        }
        int confronto;
        x->steps=0;
        albero raggiungibile= predecessore(x);

        while(x!=y) //marchia tutte le stazioni con i passi necessari per arrivare
        {
            confronto=x->steps;
            while ((x->distanza - raggiungibile->distanza) > max_autonomia(x->autonomia, x->numero_veicoli) && x!=raggiungibile && confronto==x->steps)
                x = predecessore(x); //scorro gli x raggiunti per arrivare al primo elemento non raggiunto

            if(x==raggiungibile)  //cioè x non riesce ad arrivare al primo elemento scoperto con autonomia
            {
                printf("nessun percorso\n");
                return;
            }
            while (raggiungibile!=y && (x->distanza - raggiungibile->distanza) <= max_autonomia(x->autonomia,x->numero_veicoli))
            {
                raggiungibile->steps=x->steps+1;
                raggiungibile= predecessore(raggiungibile);
            }
            if(raggiungibile==y && (x->distanza - raggiungibile->distanza) <= max_autonomia(x->autonomia,x->numero_veicoli))
            {
                raggiungibile->steps=x->steps+1;
                break;
            }

            x= predecessore(x);
        }

        myl= inserisci_intesta(myl, y->distanza);
        int diff=y->distanza;
        confronto=y->steps;
        while(y!=NULL && y->distanza!=partenza)
        {
            while(y->steps==confronto && y!=NULL)
            {
                y=successore(y);
            }

            int minimo=2147483647;  //valore massimo di int, cercare un'opzione migliore
            confronto=y->steps;
            if((y->distanza - diff) <= max_autonomia(y->autonomia,y->numero_veicoli))
                minimo=y->distanza;
            while(y->distanza!=partenza && y->steps==confronto)
            {
                y=successore(y);
                if((y->distanza - diff) <= max_autonomia(y->autonomia,y->numero_veicoli) && y->distanza<minimo)
                    minimo = y->distanza;
            }
            diff=minimo;
            myl=inserisci_intesta(myl,minimo);
        }
    }

    printf("%d", partenza);
    if(myl->distanza==partenza)
        myl=myl->next;
    stampa(myl);

    /*albero x= trova_stazione(94);
        printf("%d %d",max_autonomia(x->autonomia, x->numero_veicoli), x->numero_veicoli);*/
}

lista inserisci_intesta(lista l, int el)
{
    struct blocco *temp=malloc(sizeof(struct blocco));
    temp->next=l;
    temp->distanza=el;
    l=temp;
    return l;
}

void stampa(lista l)
{
    if(l == NULL)
        printf("\n");
    else
    {
        printf(" %d", l->distanza);
        stampa (l->next);
    }
}

