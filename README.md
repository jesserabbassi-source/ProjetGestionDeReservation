# 🏢 Système de Gestion de Réservations - Salles de Réunion

Un système complet de gestion de réservations de salles de réunion développé en C, utilisant des structures de données avancées (listes chaînées) et intégrant un chatbot intelligent via API REST.

## 📋 Table des Matières

- [Fonctionnalités](#fonctionnalités)
- [Technologies Utilisées](#technologies-utilisées)
- [Installation](#installation)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Structure du Projet](#structure-du-projet)
- [Captures d'Écran](#captures-décran)
- [Fichiers Générés](#fichiers-générés)

## ✨ Fonctionnalités

### 1. Gestion des Réservations
- ✅ Création de nouvelles réservations
- ✅ Affichage de toutes les réservations
- ✅ Annulation de réservations
- ✅ Détection automatique des conflits horaires
- ✅ Validation des dates (format YYYY-MM-DD)
- ✅ Vérification de la capacité des salles
- ✅ Génération automatique de factures

### 2. Gestion des Salles
- 📊 Ajout de nouvelles salles
- 📊 Affichage de toutes les salles disponibles
- 📊 Recherche de salles spécifiques
- 📊 Configuration de la capacité et des tarifs
- 📊 Gestion des équipements

### 3. Système de Tarification
- 💰 Calcul automatique du montant total
- 💰 Tarifs horaires personnalisés par salle
- 💰 Affichage des tarifs disponibles

### 4. Génération de Factures
- 📄 Création automatique de fichiers `facture_[ID].txt`
- 📄 Informations complètes (client, salle, horaires, montant)
- 📄 Format professionnel et lisible

### 5. Statistiques
- 📈 Chiffre d'affaires par salle
- 📈 Nombre de réservations par salle
- 📈 Réservations par mois (année 2025)
- 📈 Analyse des performances

### 6. Chatbot Conversationnel (BONUS)
- 🤖 Assistant intelligent via API REST
- 🤖 Réponses en temps réel
- 🤖 Communication avec serveur Flask (port 5000)
- 🤖 Interface conversationnelle naturelle

## 🛠 Technologies Utilisées

- **Langage**: C (Standard C11)
- **Bibliothèques**:
  - `stdio.h` - Entrées/sorties
  - `string.h` - Manipulation de chaînes
  - `stdlib.h` - Allocation mémoire
  - `curl/curl.h` - Requêtes HTTP pour le chatbot
- **Structures de données**:
  - Listes chaînées (NodeSalle, NodeReservation)
  - Structures personnalisées (Salle, Reservation)
- **Persistance**: Fichiers binaires (.bin)

## 📦 Installation

### Prérequis

1. **Compilateur GCC** (MinGW pour Windows)
2. **Bibliothèque cURL**
   - Télécharger depuis: https://curl.se/windows/
   - Extraire dans `C:/Libraries/curl/`
3. **Serveur Flask** (pour le chatbot)
   ```bash
   pip install flask
   ```

### Configuration cURL

Assurez-vous que la structure suivante existe:
```
C:/Libraries/curl/
├── include/
│   └── curl/
│       └── curl.h
└── lib/
    └── libcurl.a
```

## ⚙️ Compilation

### Windows (MinGW)

```bash
gcc -g main.c -I C:/Libraries/curl/include -L C:/Libraries/curl/lib -lcurl -lws2_32 -o main.exe
```

### Linux

```bash
gcc -g main.c -lcurl -o main
```

### Options de compilation
- `-g` : Informations de débogage
- `-I` : Chemin des headers cURL
- `-L` : Chemin des bibliothèques cURL
- `-lcurl` : Lier avec libcurl
- `-lws2_32` : Winsock2 (Windows uniquement)

## 🚀 Utilisation

### Lancer le programme

```bash
./main.exe
```

### Démarrer le chatbot (optionnel)

```bash
python app.py
```
Le serveur Flask doit tourner sur `http://localhost:5000` pour que le chatbot fonctionne.

## 📂 Structure du Projet

```
projet/
├── main.c                 # Code source principal
├── main.exe              # Exécutable compilé
├── salles.bin            # Base de données des salles
├── reservations.bin      # Base de données des réservations
├── sal.bin               # Sauvegarde des salles
├── res.bin               # Sauvegarde des réservations
├── facture_1.txt         # Factures générées
├── facture_2.txt
├── app.py                # Serveur Flask (chatbot)
└── README.md             # Documentation
```

## 📸 Captures d'Écran

### Menu Principal
```
  ________________________________________
||________________________________________|
||  SYSTEME DE GESTION DE RÉSERVATIONS    ||
||         SALLES DE RÉUNION              ||
  ________________________________________
  ________________________________________

  ________________________________________
||            MENU PRINCIPAL              ||
||________________________________________|
|| 1. Gestion des reservations            ||
|| 2. Gestion des salles                  ||
|| 3. Consulter les tarifs                ||
|| 4. Statistiques                        ||
|| 5. Parler avec notre bot               ||
|| 0. Quitter                             ||
  ________________________________________
```

### Gestion des Salles
```
  ________________________________________
||         GESTION DES SALLES             ||
||________________________________________|
|| 1. Ajouter une salle                   ||
|| 2. Afficher toutes les salles          ||
|| 3. Rechercher une salle                ||
|| 0. Retour                              ||
  ________________________________________

Liste des salles :
------------------
Nom: salleA | Capacite: 20 | Tarif: 50.00 DT/h
Nom: salleB | Capacite: 10 | Tarif: 30.00 DT/h
Nom: salleC | Capacite: 50 | Tarif: 100.00 DT/h
```

### Création de Réservation
```
Nom du client : Ahmed
Nom de la salle : salleA
Date (YYYY-MM-DD) : 2025-12-15
Heure debut (ex: 9) : 9
Heure fin (ex: 12) : 12
Nombre de personnes : 15

  Reservation creee! Montant: 150.00 DT
  Facture generee : facture_1.txt
```

### Exemple de Facture Générée
```
========================================
          FACTURE DE RESERVATION
========================================

ID Reservation : 1
Client         : Ahmed
Salle          : salleA
Date           : 2025-12-15
Horaire        : 9h00 - 12h00
Duree          : 3 heures
Nb personnes   : 15
Statut         : CONFIRMEE

----------------------------------------
MONTANT TOTAL  : 150.00 DT
========================================
```

### Statistiques
```
  ________________________________________
||          STATISTIQUES                  ||
╚________________________________________╝

Chiffre d'affaires par salle :
--------------------------------
salleA : 450.00 DT (3 reservations)
salleB : 180.00 DT (2 reservations)
salleC : 200.00 DT (1 reservations)

Reservations par mois (annee 2025) :
------------------------------------
Mois 12 : 6 reservations
```

### Chatbot Conversationnel
```
  ________________________________________
||    BOT D'ASSISTANCE - Posez votre     ||
||         question (ou 'quitter')        ||
╚________________________________________╝

Vous : Quelles sont les salles disponibles ?
Bot : Nous avons actuellement 3 salles disponibles :
      - salleA (20 personnes, 50 DT/h)
      - salleB (10 personnes, 30 DT/h)
      - salleC (50 personnes, 100 DT/h)

Vous : Comment réserver une salle ?
Bot : Pour réserver une salle, sélectionnez l'option 1
      dans le menu principal...
```

### Détection de Conflit
```
Date (YYYY-MM-DD) : 2025-12-15
Heure debut (ex: 9) : 10
Heure fin (ex: 12) : 13

  Conflit detecte! Cette salle est deja reservee.
  
Veuillez choisir un autre créneau horaire.
```

### 3D VISUALISATION
<img width="1917" height="1005" alt="image" src="https://github.com/user-attachments/assets/dc6f7d18-96fa-42e3-ada7-0570930e95a6" />
<img width="1919" height="1003" alt="image" src="https://github.com/user-attachments/assets/e2464a44-1fd5-416b-99f5-7101b6e4025d" />
<img width="1917" height="1002" alt="image" src="https://github.com/user-attachments/assets/a1dc34a2-9e4b-4254-bd48-204ad586bb86" />
<img width="1919" height="1079" alt="Capture d&#39;écran 2025-12-10 114341" src="https://github.com/user-attachments/assets/6bf87152-b9fd-4cb2-a146-9ff700cb70ed" />
<img width="1919" height="1079" alt="Capture d&#39;écran 2025-12-10 114347" src="https://github.com/user-attachments/assets/32e8e6d5-7c7a-43a1-85ab-6c14f96b96d7" />





## 📁 Fichiers Générés

### Fichiers de Données
- `salles.bin` - Base de données binaire des salles
- `reservations.bin` - Base de données binaire des réservations
- `sal.bin` - Sauvegarde alternative des salles
- `res.bin` - Sauvegarde alternative des réservations

### Fichiers de Factures
- `facture_1.txt`, `facture_2.txt`, etc.
- Format texte lisible
- Une facture par réservation

## 🔧 Fonctionnalités Techniques

### Structures de Données

**Liste Chaînée - Salles**
```c
typedef struct NodeSalle {
    Salle data;
    struct NodeSalle* next;
} NodeSalle;
```

**Liste Chaînée - Réservations**
```c
typedef struct NodeReservation {
    Reservation data;
    struct NodeReservation* next;
} NodeReservation;
```

### Validation

- ✅ Validation des dates (années bissextiles incluses)
- ✅ Vérification de la capacité des salles
- ✅ Détection des conflits horaires
- ✅ Validation des heures (début < fin)

### Persistance

- Sauvegarde automatique après chaque modification
- Format binaire pour optimisation de l'espace
- Chargement automatique au démarrage

## 🤝 Contribution

Pour contribuer à ce projet :

1. Fork le repository
2. Créez une branche (`git checkout -b feature/amelioration`)
3. Commit vos changements (`git commit -m 'Ajout fonctionnalité'`)
4. Push vers la branche (`git push origin feature/amelioration`)
5. Ouvrez une Pull Request

## 📝 Licence

Ce projet est développé dans un cadre éducatif.

## 👥 Auteur

Projet de gestion de réservations - Système de Salles de Réunion - JERBI AHMED - ABBASSI JESSER - ABDELLATIF RANA 



---

**Note**: Ce système utilise des listes chaînées pour une gestion dynamique de la mémoire et une persistance des données via fichiers binaires. Le chatbot nécessite un serveur Flask fonctionnel sur le port 5000.
