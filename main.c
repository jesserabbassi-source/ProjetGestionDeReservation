#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>

/******************************************
 *      STRUCTURES AVEC LISTES CHAÎNeES
 ******************************************/

// Structure pour stocker la reponse de l'API
struct MemoryStruct {
    char *memory;
    size_t size;
};

typedef struct {
    char nom[30];
    int capacite;
    float tarif_horaire;
    char equipements[200];
} Salle;

typedef struct {
    int id;
    char nom_client[50];
    char salle[30];
    char date[15];
    int heure_debut;
    int heure_fin;
    int nombre_personnes;
    float tarif_total;
    char statut[20];
} Reservation;

// Liste chaînee pour les salles
typedef struct NodeSalle {
    Salle data;
    struct NodeSalle* next;
} NodeSalle;

// Liste chaînee pour les reservations
typedef struct NodeReservation {
    Reservation data;
    struct NodeReservation* next;
} NodeReservation;

// Arbre binaire de recherche pour les statistiques (optionnel)
typedef struct NodeArbre {
    char nom_salle[30];
    float chiffre_affaires;
    int nb_reservations;
    struct NodeArbre* gauche;
    struct NodeArbre* droit;
} NodeArbre;



/******************************************
 *    FONCTIONS LISTE CHAÎNEE - SALLES
 ******************************************/

NodeSalle* creer_node_salle(Salle s) {
    NodeSalle* nouveau = (NodeSalle*)malloc(sizeof(NodeSalle));
    if (nouveau) {
        nouveau->data = s;
        nouveau->next = NULL;
    }
    return nouveau;
}

void ajouter_salle(NodeSalle** head, Salle s) {
    NodeSalle* nouveau = creer_node_salle(s);
    if (*head == NULL) {
        *head = nouveau;
    } else {
        NodeSalle* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nouveau;
    }
}

int compter_salles(NodeSalle* head) {
    int count = 0;
    NodeSalle* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

bool trouver_salle(NodeSalle* head, char nom[], Salle* result) {
    NodeSalle* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data.nom, nom) == 0) {
            *result = temp->data;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

/******************************************
 *  FONCTIONS LISTE CHAÎNeE - ReSERVATIONS
 ******************************************/

NodeReservation* creer_node_reservation(Reservation r) {
    NodeReservation* nouveau = (NodeReservation*)malloc(sizeof(NodeReservation));
    if (nouveau) {
        nouveau->data = r;
        nouveau->next = NULL;
    }
    return nouveau;
}

void ajouter_reservation(NodeReservation** head, Reservation r) {
    NodeReservation* nouveau = creer_node_reservation(r);
    if (*head == NULL) {
        *head = nouveau;
    } else {
        NodeReservation* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nouveau;
    }
}

int compter_reservations(NodeReservation* head) {
    int count = 0;
    NodeReservation* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

bool verifier_conflit(NodeReservation* head, char salle[], char date[], int heure_debut, int heure_fin) {
    NodeReservation* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data.salle, salle) == 0 && 
            strcmp(temp->data.date, date) == 0 &&
            strcmp(temp->data.statut, "CONFIRMEE") == 0) {
            // Verifier chevauchement
            if ((heure_debut < temp->data.heure_fin && heure_fin > temp->data.heure_debut)) {
                return true; // Conflit detecte
            }
        }
        temp = temp->next;
    }
    return false;
}

/******************************************
 *      VALIDATION DE DATE
 ******************************************/

int isLeapYear(int year) {
    if(year % 400 == 0) return 1;
    if(year % 100 == 0) return 0;
    if(year % 4 == 0) return 1;
    return 0;
}

int isValidDate(const char *date) {
    int year, month, day;
    if (!isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[2]) || !isdigit(date[3]) ||
        date[4] != '-' || !isdigit(date[5]) || !isdigit(date[6]) ||
        date[7] != '-' || !isdigit(date[8]) || !isdigit(date[9])) {
        return 0;
    }
    year = (date[0]-'0')*1000 + (date[1]-'0')*100 + (date[2]-'0')*10 + (date[3]-'0');
    month = (date[5]-'0')*10 + (date[6]-'0');
    day = (date[8]-'0')*10 + (date[9]-'0');
    if (year < 1 || month < 1 || month > 12 || day < 1) return 0;
    int month_lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) month_lengths[1] = 29;
    if (day > month_lengths[month - 1]) return 0;
    return 1;
}

/******************************************
 *         CALCUL TARIF
 ******************************************/

float calculer_montant(Reservation r, NodeSalle* head) {
    NodeSalle* temp = head;
    while (temp != NULL) {
        if (strcmp(r.salle, temp->data.nom) == 0) {
            int duree = r.heure_fin - r.heure_debut;
            return temp->data.tarif_horaire * duree;
        }
        temp = temp->next;
    }
    return -1;
}

/******************************************
 *      PERSISTANCE DONNeES
 ******************************************/

void sauvegarder_salles(NodeSalle* head) {
    FILE* f = fopen("salles.bin", "wb");
    if (!f) return;
    NodeSalle* temp = head;
    while (temp != NULL) {
        fwrite(&temp->data, sizeof(Salle), 1, f);
        temp = temp->next;
    }
    fclose(f);
}

NodeSalle* charger_salles() {
    FILE* f = fopen("salles.bin", "rb");
    if (!f) return NULL;
    NodeSalle* head = NULL;
    Salle s;
    while (fread(&s, sizeof(Salle), 1, f) == 1) {
        ajouter_salle(&head, s);
    }
    fclose(f);
    return head;
}

void sauvegarder_reservations(NodeReservation* head) {
    FILE* f = fopen("reservations.bin", "wb");
    if (!f) return;
    NodeReservation* temp = head;
    while (temp != NULL) {
        fwrite(&temp->data, sizeof(Reservation), 1, f);
        temp = temp->next;
    }
    fclose(f);
}

NodeReservation* charger_reservations() {
    FILE* f = fopen("reservations.bin", "rb");
    if (!f) return NULL;
    NodeReservation* head = NULL;
    Reservation r;
    while (fread(&r, sizeof(Reservation), 1, f) == 1) {
        ajouter_reservation(&head, r);
    }
    fclose(f);
    return head;
}

/******************************************
 *      GeNeRATION FACTURES (TÂCHE 4)
 ******************************************/

void generer_facture(Reservation r) {
    char nom_fichier[100];
    sprintf(nom_fichier, "facture_%d.txt", r.id);
    FILE* f = fopen(nom_fichier, "w");
    if (!f) {
        printf("Erreur lors de la creation de la facture\n");
        return;
    }
    fprintf(f, "========================================\n");
    fprintf(f, "          FACTURE DE RESERVATION\n");
    fprintf(f, "========================================\n\n");
    fprintf(f, "ID Reservation : %d\n", r.id);
    fprintf(f, "Client         : %s\n", r.nom_client);
    fprintf(f, "Salle          : %s\n", r.salle);
    fprintf(f, "Date           : %s\n", r.date);
    fprintf(f, "Horaire        : %dh00 - %dh00\n", r.heure_debut, r.heure_fin);
    fprintf(f, "Duree          : %d heures\n", r.heure_fin - r.heure_debut);
    fprintf(f, "Nb personnes   : %d\n", r.nombre_personnes);
    fprintf(f, "Statut         : %s\n", r.statut);
    fprintf(f, "\n----------------------------------------\n");
    fprintf(f, "MONTANT TOTAL  : %.2f DT\n", r.tarif_total);
    fprintf(f, "========================================\n");
    fclose(f);
    printf("  Facture generee : %s\n", nom_fichier);
}

/******************************************
 *         STATISTIQUES (TÂCHE 5)
 ******************************************/

void afficher_statistiques(NodeReservation* reservations, NodeSalle* salles) {
    printf("  ________________________________________\n");
    printf("  ________________________________________\n");
    printf("||          STATISTIQUES                  ||\n");
    printf("╚________________________________________╝\n\n");
    
    // Chiffre d'affaires par salle
    printf("Chiffre d'affaires par salle :\n");
    printf("--------------------------------\n");
    NodeSalle* s = salles;
    while (s != NULL) {
        float ca = 0;
        int nb_res = 0;
        NodeReservation* r = reservations;
        while (r != NULL) {
            if (strcmp(r->data.salle, s->data.nom) == 0 && 
                strcmp(r->data.statut, "CONFIRMEE") == 0) {
                ca += r->data.tarif_total;
                nb_res++;
            }
            r = r->next;
        }
        printf("%s : %.2f DT (%d reservations)\n", s->data.nom, ca, nb_res);
        s = s->next;
    }
    
    // Reservations par mois
    printf("\nReservations par mois (annee 2025) :\n");
    printf("------------------------------------\n");
    for (int mois = 1; mois <= 12; mois++) {
        int count = 0;
        NodeReservation* r = reservations;
        while (r != NULL) {
            int m;
            sscanf(r->data.date + 5, "%2d", &m);
            if (m == mois && strcmp(r->data.statut, "CONFIRMEE") == 0) {
                count++;
            }
            r = r->next;
        }
        if (count > 0) {
            printf("Mois %02d : %d reservations\n", mois, count);
        }
    }
}

/******************************************
 *      BOT CONVERSATIONNEL (BONUS)
 ******************************************/

 // Fonction callback pour recevoir les donnees de curl
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        printf("Erreur: pas assez de memoire\n");
        return 0;
    }
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

// Fonction pour extraire la reponse du JSON
void extraire_reponse(const char *json, char *reponse, size_t taille_max) {
    // Debug: afficher le JSON reçu
    // printf("JSON reçu: %s\n", json);
    
    // Chercher "response":" ou "response" : " (avec espaces possibles)
    const char *debut = strstr(json, "\"response\"");
    if (debut == NULL) {
        strcpy(reponse, "Erreur: champ 'response' non trouve");
        return;
    }
    
    // Avancer jusqu'au premier guillemet après "response"
    debut = strchr(debut + 10, '\"');  // +10 pour sauter "response"
    if (debut == NULL) {
        strcpy(reponse, "Erreur: format JSON invalide");
        return;
    }
    debut++; // Sauter le guillemet d'ouverture
    
    // Trouver la fin de la reponse
    const char *fin = debut;
    while (*fin != '\0') {
        if (*fin == '\"' && *(fin - 1) != '\\') {
            break;  // Guillemet non échappé trouvé
        }
        fin++;
    }
    
    if (*fin == '\0') {
        strcpy(reponse, "Erreur: fin de reponse non trouvee");
        return;
    }
    
    size_t longueur = fin - debut;
    if (longueur >= taille_max) {
        longueur = taille_max - 1;
    }
    
    strncpy(reponse, debut, longueur);
    reponse[longueur] = '\0';
    
    // Remplacer les echappements
    char temp[2000];
    int j = 0;
    for (int i = 0; reponse[i] != '\0' && j < 1999; i++) {
        if (reponse[i] == '\\' && reponse[i+1] == 'n') {
            temp[j++] = '\n';
            i++;
        } else if (reponse[i] == '\\' && reponse[i+1] == '\"') {
            temp[j++] = '\"';
            i++;
        } else if (reponse[i] == '\\' && reponse[i+1] == '\\') {
            temp[j++] = '\\';
            i++;
        } else {
            temp[j++] = reponse[i];
        }
    }
    temp[j] = '\0';
    strcpy(reponse, temp);
}


void bot_conversationnel(NodeSalle* salles, NodeReservation* reservations) {
    char question[500];
    printf("  ________________________________________\n");
    printf("||    BOT D'ASSISTANCE - Posez votre     ||\n");
    printf("||         question (ou 'quitter')        ||\n");
    printf("╚________________________________________╝\n\n");
    
    // Initialiser curl globalement
    curl_global_init(CURL_GLOBAL_ALL);
    
    while (1) {
        printf("Vous : ");
        fgets(question, sizeof(question), stdin);
        question[strcspn(question, "\n")] = 0;
        
        if (strcmp(question, "quitter") == 0) break;
        
        // Preparer la requete JSON
        char json_data[1000];
        // Echapper les guillemets dans la question
        char question_escaped[500];
        int j = 0;
        for (int i = 0; question[i] != '\0' && j < 498; i++) {
            if (question[i] == '\"' || question[i] == '\\') {
                question_escaped[j++] = '\\';
            }
            question_escaped[j++] = question[i];
        }
        question_escaped[j] = '\0';
        
        sprintf(json_data, "{\"question\":\"%s\"}", question_escaped);
        
        // Configuration curl
        CURL *curl;
        CURLcode res;
        struct MemoryStruct chunk;
        
        chunk.memory = malloc(1);
        chunk.size = 0;
        
        curl = curl_easy_init();
        if(curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/chat");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

            // Disable SSL verification for localhost (HTTP not HTTPS)
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            
            printf("Bot : ");
            fflush(stdout);
            
            // Executer la requete
            res = curl_easy_perform(curl);
            
            if(res != CURLE_OK) {
                printf("Erreur de connexion: %s\n", curl_easy_strerror(res));
                printf("Assurez-vous que le serveur Flask est lance (python app.py)\n");
            } else {
                // Extraire et afficher la reponse
                char reponse[2000];
                extraire_reponse(chunk.memory, reponse, sizeof(reponse));
                printf("%s\n", reponse);
            }
            
            // Nettoyage
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        } else {
            printf("Erreur: impossible d'initialiser curl\n");
        }
        
        free(chunk.memory);
        printf("\n");
    }
    
    curl_global_cleanup();
}

/******************************************
 *         MENUS INTERACTIFS
 ******************************************/

void menu_salles(NodeSalle** salles) {
    int choix;
    do {
        printf("  ________________________________________\n");
        printf("||         GESTION DES SALLES             ||\n");
        printf("||________________________________________||\n");
        printf("|| 1. Ajouter une salle                   ||\n");
        printf("|| 2. Afficher toutes les salles          ||\n");
        printf("|| 3. Rechercher une salle                ||\n");
        printf("|| 0. Retour                              ||\n");
        printf("  ________________________________________\n");
        printf("Choix : ");
        scanf("%d", &choix);
        getchar();
        
        if (choix == 1) {
            Salle s;
            int nbs=compter_salles(*salles);
            char ch[20] = "salle";   // assez grand pour contenir "salleX"
            char c = (char)(nbs + 65); // 0 → 'A', 1 → 'B', etc.

            char ch1[2];   // 1 lettre + '\0'
            ch1[0] = c;
            ch1[1] = '\0';

            strcat(ch, ch1);
            strcpy(s.nom, ch);
            printf("Capacite : ");
            scanf("%d", &s.capacite);
            printf("Tarif horaire : ");
            scanf("%f", &s.tarif_horaire);
            printf("equipements : ");
            getchar();
            fgets(s.equipements, sizeof(s.equipements), stdin);
            s.equipements[strcspn(s.equipements, "\n")] = 0;
            ajouter_salle(salles, s);
            sauvegarder_salles(*salles);
            printf("  Salle ajoutee avec succes!\n");
        }
        else if (choix == 2) {
            printf("\nListe des salles :\n");
            printf("------------------\n");
            NodeSalle* temp = *salles;
            while (temp != NULL) {
                printf("Nom: %s | Capacite: %d | Tarif: %.2f DT/h\n", 
                       temp->data.nom, temp->data.capacite, temp->data.tarif_horaire);
                temp = temp->next;
            }
        }
        else if (choix == 3) {
            char nom[30];
            Salle s;
            printf("Nom de la salle : ");
            scanf("%s", nom);
            if (trouver_salle(*salles, nom, &s)) {
                printf("\n  Salle trouvee :\n");
                printf("  Capacite: %d personnes\n", s.capacite);
                printf("  Tarif: %.2f DT/heure\n", s.tarif_horaire);
                printf("  equipements: %s\n", s.equipements);
            } else {
                printf("  Salle non trouvee.\n");
            }
        }
    } while (choix != 0);
}

void menu_reservations(NodeReservation** reservations, NodeSalle* salles) {
    int choix;
    do {
        printf("  ________________________________________\n");
        printf("||       GESTION DES ReSERVATIONS         ||\n");
        printf("||________________________________________||\n");
        printf("||________________________________________||\n");
        printf("|| 1. Nouvelle reservation                ||\n");
        printf("|| 2. Afficher les reservations           ||\n");
        printf("|| 3. Annuler une reservation             ||\n");
        printf("|| 0. Retour                              ||\n");
        printf("  ________________________________________\n");
        printf("Choix : ");
        scanf("%d", &choix);
        getchar();
        
        if (choix == 1) {
            Reservation r;
            Salle s;
            r.id = compter_reservations(*reservations) + 1;
            
            printf("Nom du client : ");
            scanf("%s", r.nom_client);
            
            do {
                printf("Nom de la salle : ");
                scanf("%s", r.salle);
                if (!trouver_salle(salles, r.salle, &s)) {
                    printf("  Salle non trouvee!\n");
                }
            } while (!trouver_salle(salles, r.salle, &s));
            
            do {
                printf("Date (YYYY-MM-DD) : ");
                scanf("%s", r.date);
            } while (!isValidDate(r.date));
            
            do {
                printf("Heure debut (ex: 9) : ");
                scanf("%d", &r.heure_debut);
                printf("Heure fin (ex: 12) : ");
                scanf("%d", &r.heure_fin);
                
                if (r.heure_fin <= r.heure_debut) {
                    printf("  Heure fin doit être apres heure debut!\n");
                } else if (verifier_conflit(*reservations, r.salle, r.date, r.heure_debut, r.heure_fin)) {
                    printf("  Conflit detecte! Cette salle est deja reservee.\n");
                    r.heure_fin = r.heure_debut;
                }
            } while (r.heure_fin <= r.heure_debut);
            
            do {
                printf("Nombre de personnes : ");
                scanf("%d", &r.nombre_personnes);
                if (r.nombre_personnes > s.capacite) {
                    printf("  Capacite maximale: %d personnes\n", s.capacite);
                }
            } while (r.nombre_personnes > s.capacite);
            
            strcpy(r.statut, "CONFIRMEE");
            r.tarif_total = calculer_montant(r, salles);
            
            ajouter_reservation(reservations, r);
            sauvegarder_reservations(*reservations);
            generer_facture(r);
            printf("  Reservation creee! Montant: %.2f DT\n", r.tarif_total);
        }
        else if (choix == 2) {
            printf("\nListe des reservations :\n");
            printf("------------------------\n");
            NodeReservation* temp = *reservations;
            while (temp != NULL) {
                printf("ID:%d | Client:%s | Salle:%s | Date:%s | %dh-%dh | %.2f DT | %s\n",
                       temp->data.id, temp->data.nom_client, temp->data.salle,
                       temp->data.date, temp->data.heure_debut, temp->data.heure_fin,
                       temp->data.tarif_total, temp->data.statut);
                temp = temp->next;
            }
        }
        else if (choix == 3) {
            int id;
            printf("ID de la reservation a annuler : ");
            scanf("%d", &id);
            NodeReservation* temp = *reservations;
            bool trouve = false;
            while (temp != NULL) {
                if (temp->data.id == id) {
                    strcpy(temp->data.statut, "ANNULEE");
                    trouve = true;
                    sauvegarder_reservations(*reservations);
                    printf("  Reservation annulee.\n");
                    break;
                }
                temp = temp->next;
            }
            if (!trouve) printf("  Reservation non trouvee.\n");
        }
    } while (choix != 0);
}

/******************************************
 *            MAIN FUNCTION
 ******************************************/

int main() {
    NodeSalle* salles = charger_salles();
    NodeReservation* reservations = charger_reservations();
    printf("%zu",sizeof(Salle));
    int choix;
    
    printf("\n");
    printf("  ________________________________________\n");
    printf("||________________________________________||\n");
    printf("||  SYSTEME DE GESTION DE ReSERVATIONS    ||\n");
    printf("||         SALLES DE ReUNION              ||\n");
    printf("  ________________________________________\n");
    printf("  ________________________________________\n");
    
    do {
        printf("  ________________________________________\n");
        printf("||            MENU PRINCIPAL              ||\n");
        printf("||________________________________________||\n");
        printf("|| 1. Gestion des reservations            ||\n");
        printf("|| 2. Gestion des salles                  ||\n");
        printf("|| 3. Consulter les tarifs                ||\n");
        printf("|| 4. Statistiques                        ||\n");
        printf("|| 5. Parler avec notre bot               ||\n");
        printf("|| 0. Quitter                             ||\n");
        printf("  ________________________________________\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar();
        
        switch(choix) {
            case 1:
                menu_reservations(&reservations, salles);
                break;
            case 2:
                menu_salles(&salles);
                break;
            case 3:
                printf("  ________________________________________\n");
                printf("  ________________________________________\n");
                printf("||            NOS TARIFS                  ||\n");
                printf("  ________________________________________\n");
                printf("||________________________________________||\n");
                NodeSalle* temp = salles;
                while (temp != NULL) {
                    printf("%s : %.2f DT/heure (Capacite: %d)\n", 
                           temp->data.nom, temp->data.tarif_horaire, temp->data.capacite);
                    temp = temp->next;
                }
                break;
            case 4:
                afficher_statistiques(reservations, salles);
                break;
            case 5:
                bot_conversationnel(salles, reservations);
                break;
            case 0:
                printf("\nAu revoir! \n");
                break;
            default:
                printf("Choix invalide!\n");
        }
        
    } while (choix != 0);
    FILE *f=fopen("sal.bin","wb");
    // Liberation de la memoire
    while (salles != NULL) {
        NodeSalle* temp = salles;
        
        // Visualisation 3D
        fwrite(&salles->data.nom,sizeof(char[30]),1,f);
            fwrite(&salles->data.capacite,sizeof(int),1,f);
            fwrite(&salles->data.tarif_horaire,sizeof(float),1,f);
            fwrite(&salles->data.equipements,sizeof(char[200]),1,f);
        salles = salles->next;
        free(temp);
    }
    f=fopen("res.bin","wb");
    while (reservations != NULL) {
        NodeReservation* temp = reservations;
        // Visualisation 3D
        fwrite(&reservations->data.id,sizeof(int),1,f);
            fwrite(&reservations->data.nom_client,sizeof(char[50]),1,f);
            fwrite(&reservations->data.salle,sizeof(char[30]),1,f);
            fwrite(&reservations->data.date,sizeof(char[15]),1,f);
            fwrite(&reservations->data.heure_debut,sizeof(int),1,f);
            fwrite(&reservations->data.heure_fin,sizeof(int),1,f);
            fwrite(&reservations->data.nombre_personnes,sizeof(int),1,f);
            fwrite(&reservations->data.tarif_total,sizeof(float),1,f);
            fwrite(&reservations->data.statut,sizeof(char[20]),1,f);
        reservations = reservations->next;
        free(temp);
    }
    
    return 0;
}