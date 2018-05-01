//
//  main.c
//  maths pour l'info
//
//  Created by Paul Basnier on 28/04/2018.
//  Copyright © 2018 Paul Basnier. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct state{
    int init;
    int final;
    int id;
    
}state;

typedef struct transition{
    state init;
    state final;
    char tr;
    
}transition;

typedef struct automaton{
    int nb_char;
    int nb_state;
    int nb_init_state;
    int nb_final_state;
    int synchrone;
    state* auto_state;
    int nb_transition;
    transition* auto_transition;
    int*** transition_table;
    
}automaton;

int size_int(int nb){
    int a=1;
    while(nb/10 > 0){
        nb=nb/10;
        a++;
    }
    return a;
}

state group_states(state A1, state A2){
    state A;
    A.final=0;
    A.init=0;
    A.id = A1.id + size_int(A1.id)*10*A2.id;
    if (A1.final==1 || A2.final==1) {
        A.final=1;
    }
    if (A1.init==1 || A2.init==1) {
        A.init=1;
    }
    return A;
}

automaton determinization_and_completion_asynchronous(automaton automate){
    automaton automate2;
    int nb_max_states =pow(2,automate.nb_state)*sizeof(state);
    automate2.auto_state = malloc(nb_max_states);
    
    
    return automate;
}

int is_asynchronous(automaton automate){
    int a;
    if(automate.synchrone==1){
        printf("l'automate est asynchrone car : ");
        for (a=0; a<automate.nb_transition; a++) {
            if(automate.auto_transition[a].tr == '*'){
                printf("(%d)->(%c)->(%d) ",automate.auto_transition[a].init.id,automate.auto_transition[a].tr,automate.auto_transition[a].final.id);
            }
        }
        printf("\n");
        return 1;
    }
    else{
        printf("l'automate est synchrone\n");
        return 0;
    }
}



int str2num(char* str){
    int d,u,res;
    d=str[0]-48;
    u=str[1]-48;
    if(u==0 && d==0){
        res=0;
    }
    else if(d==-48){
        res=d;
    }
    else if(u==-48){
        res=d;
    }
    else{
        res=u+d*10;
    }
    return res;
}

automaton read_automaton_on_file(FILE* fichier)
{
                //initialisation
    automaton new_auto;
    
                //initialisation du nombre de caracteres dans l'alphabet.
    char* autom =calloc(2*sizeof(char),2*sizeof(char));
    fscanf(fichier,"%s",autom);
    new_auto.nb_char=str2num(autom);
    
                //initialisation du nombre d'états.
    fscanf(fichier,"%s",autom);
    new_auto.nb_state=str2num(autom);
    
                //création des etats.
    new_auto.auto_state = malloc(new_auto.nb_state*sizeof(state));
    int a;
    for(a=0;a<new_auto.nb_state;a++){
        new_auto.auto_state[a].id=a;
        new_auto.auto_state[a].init=0;
        new_auto.auto_state[a].final=0;
    }
    
                //nombre d'etats initiaux.
    fscanf(fichier,"%s",autom);
    new_auto.nb_init_state=str2num(autom);
    
                //attribution des états initiaux.
    int b =0;
    for(a=0;a<new_auto.nb_init_state;a++){
        fscanf(fichier,"%s",autom);
        b=str2num(autom);
        new_auto.auto_state[b].init=1;
    }
    
            //nombre d'etats finaux.
    fscanf(fichier,"%s",autom);
    new_auto.nb_final_state=str2num(autom);
    
            //attribution des états finaux.
    for(a=0;a<new_auto.nb_final_state;a++){
        fscanf(fichier,"%s",autom);
        b=str2num(autom);
        new_auto.auto_state[b].final=1;
    }
    
            //nombre de transitions.
    fscanf(fichier,"%s",autom);
    new_auto.nb_transition=str2num(autom);
    new_auto.auto_transition = malloc(new_auto.nb_transition*(sizeof(transition)));
    
            //création des transitions.
    for(b=0;b<new_auto.nb_transition;b++){
        fscanf(fichier,"%s",autom);
        new_auto.auto_transition[b].init=new_auto.auto_state[str2num(autom)];
        fscanf(fichier,"%s",autom);
        new_auto.auto_transition[b].tr=autom[0];
        fscanf(fichier,"%s",autom);
        new_auto.auto_transition[b].final=new_auto.auto_state[str2num(autom)];
        
    }
    
    int ***tab;
    

    
    int x;
    char star;
    new_auto.synchrone=0;
    for(x=0;x<new_auto.nb_transition;x++){
        star = new_auto.auto_transition[x].tr;
        
        int star2 = (int)star-97;
        if(star2==-55){
            new_auto.synchrone=1;
        }
    }
    
    if(new_auto.synchrone == 1){
        new_auto.nb_char++;
    }
    
    int i,j,f;
    int taille1=new_auto.nb_state;
    int taille2=new_auto.nb_char;
    int taille3=new_auto.nb_state;
    
    
    tab = malloc(taille1 * sizeof(*tab));
    for(i=0 ; i < taille1 ; i++){
        tab[i] = malloc(taille2 * sizeof(*tab));
    }
    for(i=0 ; i < taille1 ; i++){
        for(j=0 ; j < taille2 ; j++){
            tab[i][j] = calloc(taille3 * sizeof(***tab),taille3 * sizeof(***tab));
            for(f=0;f<taille3;f++){
                tab[i][j][f]=-1;
            }
        }
    }
    
    
    for(x=0;x<new_auto.nb_transition;x++){
        int init = new_auto.auto_transition[x].init.id;
        char tr = new_auto.auto_transition[x].tr;
        int final = new_auto.auto_transition[x].final.id;
        
        int tr2 = (int)tr-97;
        if(tr2==-55) tr2=new_auto.nb_char-1;
        
        int q=0;
        while(tab[init][tr2][q]!=-1){
            q++;
        }
        tab[init][tr2][q]=final;
    }
    
    new_auto.transition_table = tab;
 
    return new_auto;
    
}

void disp_automaton(automaton new_auto){
    
    printf("display of the automaton\n");
    printf("----------------------\n");
    printf("nb of char in the alphabet = %d\n",new_auto.nb_char);
    printf("nb of states in the automaton = %d\n",new_auto.nb_state);
    printf("----------------------\n");
    printf("I/F | S  |");
    int a=0;
    for(a=0;a<new_auto.nb_char;a++){
        if(a==new_auto.nb_char-1 && new_auto.synchrone==1) printf("*          |");
        else printf("%c          |",a+97);
    }
    printf("\n");
    int*** tab = new_auto.transition_table;
    int c,b;
    for (b=0; b<new_auto.nb_state; b++) {
        if(new_auto.auto_state[b].init==1 && new_auto.auto_state[b].final==1){
            printf("<-> ");
        }
        else if(new_auto.auto_state[b].init==1){
            printf(" -> ");
        }
        else if(new_auto.auto_state[b].final==1){
            printf(" <- ");
        }
        else printf("    ");
        printf("|%d)  |",b);
        for (a=0; a<new_auto.nb_char; a++) {
            //printf("(%d/%d)",b,a);
            for (c=0; c<new_auto.nb_state; c++) {
                if(tab[b][a][c] != -1)
                printf("%d,",tab[b][a][c]);
                else{
                    printf(" ,");
                }
            }

            printf(" |");
        }
        printf("\n");
    }
    
    
}

int main(int argc, const char * argv[]) {
    
    /*int i;
    for(i=0;i<256;i++){
        printf("%d)%c\n",i,i);
    }*/
    
    FILE* fichier = fopen("automaton.txt", "r+");
    automaton automaton_1 = read_automaton_on_file(fichier);
    disp_automaton(automaton_1);
    is_asynchronous(automaton_1);
    state grouped = group_states(automaton_1.auto_state[0], automaton_1.auto_state[3]);
    printf("grouped: id=%d ; final=%d ; init=%d\n",grouped.id,grouped.final,grouped.init);
    
    return 0;
}
