#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct Point
{
    int x;
    int y;
}Point;

void draw_map(char game_tab[3][3])
{
    SDL_Surface* ecran = SDL_GetVideoSurface();
    SDL_Surface* bg = IMG_Load("bg.png");
    SDL_Surface* x = IMG_Load("x.png");
    SDL_Surface* o = IMG_Load("o.png");
    SDL_Surface* empty = IMG_Load("empty.png");
    SDL_Rect posbg;
    SDL_Rect pospos;
    posbg.x = 0;
    posbg.y = 0;
    
    SDL_BlitSurface(bg, NULL, ecran, &posbg);
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            pospos.x = i*110+44;
            pospos.y = j*110+31;
        
            switch(game_tab[j][i])
            {
                case 'X':
                    SDL_BlitSurface(x, NULL, ecran, &pospos);
                break;
                
                case 'O':
                    SDL_BlitSurface(o, NULL, ecran, &pospos);
                break;
                
                case ' ':
                    SDL_BlitSurface(empty, NULL, ecran, &pospos);
                break;
                
                default:
                break;
            }
        }
    }   
    
    SDL_Flip(ecran);
    
    SDL_FreeSurface(o);
    SDL_FreeSurface(x);
    SDL_FreeSurface(empty);
    SDL_FreeSurface(bg);
}

void clear_tab(char game_tab[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            game_tab[j][i] = ' ';
        }
    }
}

int check_win(char game_tab[3][3], char last_played)
{
    SDL_Surface* ecran = SDL_GetVideoSurface();
    SDL_Surface* w;
    SDL_Rect pos;
    
    pos.x = 421;
    pos.y = 303;
    
    if(((game_tab[0][0] == game_tab[0][1]) && (game_tab[0][1] == game_tab[0][2]) && (game_tab[0][2] == last_played)) ||
       ((game_tab[1][0] == game_tab[1][1]) && (game_tab[1][1] == game_tab[1][2]) && (game_tab[1][2] == last_played)) ||
       ((game_tab[2][0] == game_tab[2][1]) && (game_tab[2][1] == game_tab[2][2]) && (game_tab[2][2] == last_played)) ||
       ((game_tab[0][0] == game_tab[1][1]) && (game_tab[1][1] == game_tab[2][2]) && (game_tab[2][2] == last_played)) ||
       ((game_tab[2][0] == game_tab[1][1]) && (game_tab[1][1] == game_tab[0][2]) && (game_tab[0][2] == last_played)) ||
       ((game_tab[0][0] == game_tab[1][0]) && (game_tab[1][0] == game_tab[2][0]) && (game_tab[2][0] == last_played)) ||
       ((game_tab[0][1] == game_tab[1][1]) && (game_tab[1][1] == game_tab[2][1]) && (game_tab[2][1] == last_played)) ||
       ((game_tab[0][2] == game_tab[1][2]) && (game_tab[1][2] == game_tab[2][2]) && (game_tab[2][2] == last_played)))
    {
        clear_tab(game_tab);
        if(last_played == 'X')
        {
            w = IMG_Load("1win.png");
            SDL_BlitSurface(w, NULL, ecran, &pos);              
            SDL_Flip(ecran);    
            SDL_FreeSurface(w);
            return 1;
        } 
        else if(last_played == 'O')
        {
            w = IMG_Load("2win.png");
            SDL_BlitSurface(w, NULL, ecran, &pos);  
            SDL_Flip(ecran);
            SDL_FreeSurface(w);
            return 2;
        } 
        else
            return 3;
            
    }
    else
    {
        int a = 0;
        for(int j = 0; j < 3; j++)
        {
            for(int i = 0; i < 3; i++)
            {
                if(game_tab[j][i] == ' ')
                    a++;
            }
        }
        if(!a)
        {
            clear_tab(game_tab);
            w = IMG_Load("draw.png");
            SDL_BlitSurface(w, NULL, ecran, &pos);   
            SDL_Flip(ecran);
            SDL_FreeSurface(w);
        } 
    }
    
    return 3;
}

void check_continue(int* continuer)
{
    int a = -1;
    
    while((a!= 1) && (a != 0))
    {
        printf("Rejouer ? 0:non\t1:oui : ");
        scanf("%d", &a);
    }
    
    *continuer = a;
}

      

int update_map(char game_tab[3][3], Point p, char last_played)
{
    if(p.x < 0)
        return 0;
        
    if(p.x > 3)
        return 0;
        
    if(p.y < 0)
        return 0;
        
    if(p.y > 3)
        return 0;
    
    if(game_tab[p.x][p.y] != ' ')
        return 0;

    game_tab[p.x][p.y] = last_played;
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            putc(game_tab[i][j], stdout);
        }
        putc(' ', stdout);
    }
    putc(0x0A, stdout);
    
    return 1;
}
       

void print_turn(char last)
{
    SDL_Surface* ecran = SDL_GetVideoSurface();
    SDL_Rect pos;
    SDL_Surface* t;
    
    pos.x = 496;
    pos.y = 31;
    
    switch(last)
    {
        case 'X':
            t = IMG_Load("x.png");
        break;
        
        case 'O':
            t = IMG_Load("o.png");
        break;
    }
    
    SDL_BlitSurface(t, NULL, ecran, &pos);
    SDL_Flip(ecran);
    
    SDL_FreeSurface(t);
}
            

int main(int argc, char* argv[])
{
    SDL_Surface* ecran;
    SDL_Event event;
    int continuer = true;
    char game_tab[3][3] = {"   ", "   ", "   "};
    char last_played = 'X';
    Point p = {-1, -1};
    int x, y = -1;
    
    if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }

    atexit(SDL_Quit); 
    ecran = SDL_SetVideoMode(640, 400, 32, SDL_HWSURFACE );
    
    draw_map(game_tab);
    
    while(continuer)
    {
        print_turn(last_played);
        while(!update_map(game_tab, p, last_played))
        {
            if(SDL_WaitEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_QUIT:
                        exit(0);
                    break;
                    
                    case SDL_MOUSEBUTTONUP:
                        x = (event.button.x - 44)/110;
                        y = (event.button.y - 31)/110;
                        
                        p.x = y;
                        p.y = x;
                    break;
                    
                    default:
                    break;
                }
            }
        }
        
        p.x = p.y = -1;
        draw_map(game_tab);
        
        switch(check_win(game_tab, last_played))
        {
            case 0:
                check_continue(&continuer);
            break;
            
            case 1:
                check_continue(&continuer);
            break;
            
            case 2:
                check_continue(&continuer);
            break;
            
            default:
            break;
        }
        
        if(last_played == 'X')
            last_played = 'O';
        else
            last_played = 'X';
    }
    
    (void)argc;
    (void)argv;
    return 0;
}
