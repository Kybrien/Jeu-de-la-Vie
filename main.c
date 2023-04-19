#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define CELL_SIZE 10
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

void affichage_menu(){
    printf("\n<-------------------------------->\n");
    printf("< Bienvenue sur le jeu de la Vie >\n");
    printf("<-------------------------------->\n\n");
    printf("Voici les configurations disponibles:\n");
    printf("1) Configuration aleatoire (les cellules seront disposees de maniere aleatoire)\n");
    printf("2) Configuration utilisateur (vous choisissez vous meme l'emplacement des cellules)\n");
    printf("0) QUITTER LE PROGRAMME\n");
    return;
}

// On gere le choix du joueur pour savoir quelle configuration il veut lancer
int selection(int choix){

    if (choix == 1)
    {
        printf("\n--- LANCEMENT DE LA CONFIGURATION ALEATOIRE ---\n");
        random();
    }

    if (choix == 2)
    {
        printf("\n--- LANCEMENT DE LA CONFIGURATION JOUEUR ---\n");
        user();
    }

    if (choix == 0)
    {
        printf("\n--- MERCI ET A TRES BIENTOT ---\n");
    }

    return choix;
}

//Configuration aleatoire
int random(){

    // SDL Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);

    // Creation de la fenetre de jeu
    SDL_Window *window = SDL_CreateWindow("Le Jeu de la VIE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    // Verification de l'initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Verification de la creation de la fenetre de jeu
    if (window == NULL)
    {
        printf("Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    // On cree un rendu sur la fenetre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur lors de la création du rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Event windowEvent;

    // Initialisation de la grille de cellules aléatoires
    int grid[GRID_WIDTH][GRID_HEIGHT];
    int i, j;
    for (i = 0; i < GRID_WIDTH; i++)
    {
        for (j = 0; j < GRID_HEIGHT; j++)
        {
            grid[i][j] = rand() % 2;
        }
    }

    //CODE 
    SDL_Event event;
    while (1)
    {
        // Affichage de la grille de cellules
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (i = 0; i < GRID_WIDTH; i++)
        {
            for (j = 0; j < GRID_HEIGHT; j++)
            {
                if (grid[i][j])
                {
                    SDL_Rect cell = {i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
        SDL_RenderPresent(renderer);

        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // Ferme la fenetre
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }

        // On met a jour la grille de cellules
        int new_grid[GRID_WIDTH][GRID_HEIGHT];
        for (i = 0; i < GRID_WIDTH; i++)
        {
            // On verifie les voisins de la cellule
            for (j = 0; j < GRID_HEIGHT; j++)
            {

                int voisin = 0;
                if (i > 0 && j > 0 && grid[i - 1][j - 1])
                {
                    voisin++;
                }
                if (i > 0 && grid[i - 1][j])
                {
                    voisin++;
                }
                if (i > 0 && j < GRID_HEIGHT - 1 && grid[i - 1][j + 1])
                {
                    voisin++;
                }
                if (j > 0 && grid[i][j - 1])
                {
                    voisin++;
                }
                if (j < GRID_HEIGHT - 1 && grid[i][j + 1])
                {
                    voisin++;
                }
                if (i < GRID_WIDTH - 1 && j > 0 && grid[i + 1][j - 1])
                {
                    voisin++;
                }
                if (i < GRID_WIDTH - 1 && grid[i + 1][j])
                {
                    voisin++;
                }
                if (i < GRID_WIDTH - 1 && j < GRID_HEIGHT - 1 && grid[i + 1][j + 1])
                {
                    voisin++;
                }
                if (grid[i][j])
                {
                    if (voisin == 2 || voisin == 3)
                    {
                        new_grid[i][j] = 1;
                    }
                    else
                    {
                        new_grid[i][j] = 0;
                    }
                }
                else
                {
                    if (voisin == 3)
                    {
                        new_grid[i][j] = 1;
                    }
                    else
                    {
                        new_grid[i][j] = 0;
                    }
                }
            }
        }
        for (i = 0; i < GRID_WIDTH; i++)
        {
            for (j = 0; j < GRID_HEIGHT; j++)
            {
                grid[i][j] = new_grid[i][j];
            }
        }
        SDL_Delay(100);
    }

    return EXIT_SUCCESS;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
    }

    //On ferme tout pour econnomiser la memoire
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


//Configuration ou l'utilisateur choisit l'emplacement des cellules
int user()
{

    // SDL Initialisation
    SDL_Init(SDL_INIT_EVERYTHING);

    // Creation de la fenetre de jeu
    SDL_Window *window = SDL_CreateWindow("Le Jeu de la VIE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    // Verification de l'initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    
    // Verification de la creation de la fenetre de jeu
    if (window == NULL)
    {
        printf("Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur lors de la création du rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Event windowEvent;

    // Initialisation de la grille de cellules aléatoires
    int grid[GRID_WIDTH][GRID_HEIGHT];
    int i, j;
    for (i = 0; i < GRID_WIDTH; i++)
    {
        for (j = 0; j < GRID_HEIGHT; j++)
        {
            grid[i][j] = 0; // Et ça
        }
    }

    //CODE
    int running = 1;
    int running2 = 1;
    SDL_Event event;
    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // Récupération des coordonnées de la souris
                    int x = event.button.x;
                    int y = event.button.y;

                    // Ajustement des coordonnées à la grille
                    x = (int)(x / CELL_SIZE);
                    y = (int)(y / CELL_SIZE);

                    grid[x][y] = 1; // C'est ça

                    // Dessin de la cellule sur l'écran
                    SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    // Boucle principale
                    SDL_Event event;
                    while (running2 == 1) 
                    {
                        // Affichage de la grille de cellules
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        for (int i = 0; i < GRID_WIDTH; i++)
                        {
                            for (int j = 0; j < GRID_HEIGHT; j++)
                            {
                                if (grid[i][j])
                                {
                                    SDL_Rect cell = {i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                    SDL_RenderFillRect(renderer, &cell);
                                }
                            }
                        }
                        SDL_RenderPresent(renderer); 


                        // Mise à jour de la grille de cellules
                        int new_grid[GRID_WIDTH][GRID_HEIGHT];

                        //On verifie les cellules voisines
                        for (int i = 0; i < GRID_WIDTH; i++)
                        {

                            for (int j = 0; j < GRID_HEIGHT; j++)
                            {

                                int voisin = 0;
                                if (i > 0 && j > 0 && grid[i - 1][j - 1])
                                {
                                    voisin++;
                                }
                                if (i > 0 && grid[i - 1][j])
                                {
                                    voisin++;
                                }
                                if (i > 0 && j < GRID_HEIGHT - 1 && grid[i - 1][j + 1])
                                {
                                    voisin++;
                                }
                                if (j > 0 && grid[i][j - 1])
                                {
                                    voisin++;
                                }
                                if (j < GRID_HEIGHT - 1 && grid[i][j + 1])
                                {
                                    voisin++;
                                }
                                if (i < GRID_WIDTH - 1 && j > 0 && grid[i + 1][j - 1])
                                {
                                    voisin++;
                                }
                                if (i < GRID_WIDTH - 1 && grid[i + 1][j])
                                {
                                    voisin++;
                                }
                                if (i < GRID_WIDTH - 1 && j < GRID_HEIGHT - 1 && grid[i + 1][j + 1])
                                {
                                    voisin++;
                                }
                                if (grid[i][j])
                                {
                                    if (voisin == 2 || voisin == 3)
                                    {
                                        new_grid[i][j] = 1;
                                    }
                                    else
                                    {
                                        new_grid[i][j] = 0;
                                    }
                                }
                                else
                                {
                                    if (voisin == 3)
                                    {
                                        new_grid[i][j] = 1;
                                    }
                                    else
                                    {
                                        new_grid[i][j] = 0;
                                    }
                                }
                            }
                        }
                        for (int i = 0; i < GRID_WIDTH; i++)
                        {
                            for (int j = 0; j < GRID_HEIGHT; j++)
                            {
                                grid[i][j] = new_grid[i][j];
                            }
                        }
                        //On verifie si le joueur veut quitter la fenetre ou non 
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_QUIT) {
                                running2 = 0;
                                break;
                            }
                        }   
                        SDL_Delay(100);
                    }
                }
                break;
            }
        }
        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char *argv[]){

    int choix;
    // Tant qu'on a pas de reponse satisfaisante on redemande
    do{
        affichage_menu();
        printf("\nQuel configuration choisissez-vous ?\n > ");
        fflush(stdin);
        scanf("%d", &choix);
        selection(choix);
        system("cls");
    } while (choix != 0);

    return 0;
}