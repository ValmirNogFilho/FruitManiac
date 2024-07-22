#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lib/gpulib.h"
#include <time.h>
#include "gameFolder/display.h"
#include "gameFolder/drawer.h"
#include "gameFolder/inputbehaviour.h"

#define DEVICE_PATH "/dev/gpu123"
#define MOUSE_DEVICE "/dev/input/event0"
#define MAX 511
#define MAX_ELEMENTS 9
#define DIFFICULTY_CRITERIA 5
int i, j;
int score;

sprite_t player, bomb,apple,diamond, apple1, orange, pear, apple2, orange1, pear1;
clock_t start_time, end_time;
double fps;
int frame_count = 0;


color_t black = {0, 0, 0};
color_t green = {0,7,0};
color_t red = {7, 0, 0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {3, 5, 7};

int collision_between_sprites(sprite_t spr1, sprite_t spr2) {
    if (spr1.rel_x < 10 && spr2.rel_x < 10) //lado extremo esquerdo: x < 10
        return (spr1.rel_x <= spr2.rel_x + 10 && spr1.rel_y >= (int) (spr2.rel_y -10) && spr1.rel_y <= spr2.rel_y + 10 &&(spr1.visible==1 && spr2.visible==1));
    return (spr1.rel_x >= (int) (spr2.rel_x -10) && spr1.rel_x <= spr2.rel_x + 10 && spr1.rel_y >= (int) (spr2.rel_y -10) && spr1.rel_y <= spr2.rel_y + 10 &&(spr1.visible==1 && spr2.visible==1));
}

void defineSkinsToFallingElements() {
    apple1.address = 9;
    apple1.rel_x = 0;
    apple1.rel_y = 20;
    apple1.variation = 25;
    apple1.visible = 0;

    orange.address = 10;
    orange.rel_x = 0;
    orange.rel_y = 20;
    orange.variation = 26;
    orange.visible = 0;

    pear.address = 11;
    pear.rel_x = 0;
    pear.rel_y = 20;
    pear.variation = 27;
    pear.visible = 0;



    apple2.address = 12;
    apple2.rel_x = 0;
    apple2.rel_y = 20;
    apple2.variation = 25;
    apple2.visible = 0;

    orange1.address = 13;
    orange1.rel_x = 0;
    orange1.rel_y = 20;
    orange1.variation = 26;
    orange1.visible = 0;

    pear1.address = 14;
    pear1.rel_x = 0;
    pear1.rel_y = 20;
    pear1.variation = 27;
    pear1.visible = 0;


    bomb.address = 8;
    bomb.rel_x = 0;
    bomb.rel_y = 20;
    bomb.variation = BOMB;
    bomb.visible = 0;

    apple.address =2;
    apple.rel_x = 0;
    apple.rel_y = 20;
    apple.variation = 25;
    apple.visible = 0;

    diamond.address =3;
    diamond.rel_x = 0;
    diamond.rel_y = 20;
    diamond.variation = DIAMOND;
    diamond.visible = 0;


    player.address = 5;
    player.rel_y = 459;
    player.variation = ENEMY_1;
    player.visible = 1;
}

void reset_sprites() {
    for(i=1;i<32;i++){
        bomb.address=i;
        bomb.visible=0;
        setSpriteOnScreen(bomb);
    }

}

void setup () {
    reset_sprites();
    draw_apple_on_memory();
    draw_orange_on_memory();
    draw_pear_on_memory();
    defineSkinsToFallingElements();
    erase_bg_screen();
    set_start_screen();
}



int main() {
    start_time = clock();
    int x, y;
    // int fd_gpu;

    color_t color = {0, 3, 0};
    color_t click = {0, 7, 0};
    unsigned int block;
    color_t reset = {6, 7, 7};
    pthread_t mouse_thread;
    pthread_t key_thread;
    int t, u;
    int fd = -1;               // used to open /dev/mem for access to physical addresses
    int hundreds;
    int dozens;
    int units;
    int numero;
  
    // Gera um número aleatório entre 0 e 619

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    setup();
    
    sprite_variation_t skins[4] = {BOMB, 25, 26, 27};

    // Abrir o dispositivo do mouse
    fd_mouse = open(MOUSE_DEVICE, O_RDONLY );
    if (fd_mouse == -1) {
        perror("Cannot open input device");
        return 1;
    }


    // Inicializar o mutex
    pthread_mutex_init(&lock, NULL);

    // Criar a thread para leitura do mouse
    pthread_create(&mouse_thread, NULL, read_mouse, NULL);
    pthread_create(&key_thread, NULL, read_key, NULL);

    color_t color1={0,4,3};


    int number_of_elements = 1;
    int i;
    int delay = 12;
    int old_score=1;

    while(end==0){
        int lifes=5;
        score=0;

        setToLifeDigit(lifes);
        setToDozensDigit(0);
        setToUnitsDigit(0);

        double factor = 620.0 / (RAND_MAX + 1.0);  // Calcula o factor de escala
        sprite_t fallingElements[9]={bomb,apple,diamond, apple1, orange, pear, apple2, orange1, pear1};

        int whileCounts[10] = {0}; //controla fps dos falling elements e do tiro
        setBackground(color1);




        if(!restart && !start){ //jogo propriamente dito, se entrar no if é possível jogar. Toda lógica do jogo está abaixo. //start==1
            erase_bg_screen();
            setBackground(bg);
            set_bg_screen();
            while (1) {


                if(score%15==0 && old_score!=score && score < 45){
                    delay--;
                    old_score=score;
                    }


                if(restart){
                    break;
                }


                if(end){
                    break;
                }

                if(stop){

                    frame_count++;
                    number_of_elements = number_of_elements < MAX_ELEMENTS ? (1 + score / DIFFICULTY_CRITERIA) : number_of_elements;
                    sprite_t fallingElement;
                    for(i=0;i<MAX_ELEMENTS;i++){

                        
                        whileCounts[i]++;
                        
                        if(fallingElements[i].visible==0){
                            fallingElements[i].rel_x = (int)(rand() * factor);
                            
                            if(i < number_of_elements){ 
                                fallingElements[i].visible=1;    
                            }
                            
                        }
                        
                        else {
                            if(whileCounts[i] >= delay){
                                fallingElements[i].rel_y++;
                                whileCounts[i] = 0;
                            }

                            if(fallingElements[i].rel_y > 479) {

                                fallingElements[i].rel_y = 0;
                                fallingElements[i].rel_x = (int)(rand() * factor);
                                lifes--;
                                setBackground(red);
                                usleep(10000);
                                setToLifeDigit(lifes);
                                setBackground(bg);
                            }
                        }
                        setSpriteOnScreen(fallingElements[i]);   


                        if(collision_between_sprites(beam, fallingElements[i]) == 1) {
                            if(fallingElements[i].variation != BOMB) //se atirou em uma fruta
                            {
                                setBackground(red);
                                usleep(10000);
                                setBackground(bg);
                                lifes--;
                                setToLifeDigit(lifes);
                            }
                            else{
                                score++;
                            }
                            
                            fallingElements[i].visible = 0;
                            fallingElements[i].rel_y = 0;
                            fallingElements[i].variation = skins[rand() % 4];
                            beam.visible=0;
                            beam.rel_y = 0;
                        }

                        if(collision_between_sprites(player, fallingElements[i]) == 1) {
                            if(fallingElements[i].variation == BOMB) //se uma bomba atingiu o player
                            {
                                setBackground(red);
                                usleep(10000);
                                setBackground(bg);
                                lifes--;
                                setToLifeDigit(lifes);
                            }
                            else if (fallingElements[i].variation == DIAMOND && lifes < 9) // se o player pegar um diamante
                            {
                                lifes++;
                                setToLifeDigit(lifes);
                            }
                            else { //se uma fruta atingiu o player
                                for(u = 0 ; u < 10; u++){
                                    player.rel_y-= 2;
                                    setSpriteOnScreen(player);
                                    usleep(5000);
                                }
                                for(u = 0 ; u < 10; u++){
                                    player.rel_y+= 2;
                                    setSpriteOnScreen(player);
                                    usleep(5000);
                                }
                                score++;
                            }
                            fallingElements[i].rel_y = 0;
                            fallingElements[i].visible = 0;
                            fallingElements[i].variation = skins[rand() % 4];
                            beam.rel_y = 0;
                        }
                    }

                    if(lifes<=0) break;
                    
                    x = xsoma;
                    y = ysoma;
                    if(click_reset == 1 && beam.rel_y == 0){
                        beam.visible = 1;
                        beam.rel_y = 439;
                        beam.rel_x = xsoma;
                        click_reset=0;
                    }
                    
                    whileCounts[9]++;
                    if (beam.rel_y > 0) {
                        if (whileCounts[9] >= 0) {
                            beam.rel_y -= 1;
                            whileCounts[9] = 0;
                        }
                    }

                    if(beam.rel_y == 0){
                        beam.visible = 0;
                    }
                    

                    hundreds = score % 10;
                    dozens= (score % 100) / 10;
                    units = score / 100;  

                    setToHundredsDigit(hundreds);
                    setToDozensDigit(dozens);
                    setToUnitsDigit(units);

                    setSpriteOnScreen(beam);


                    // Criar a estrutura do sprite com as novas coordenadas
                    player.rel_x = x;


                    setSpriteOnScreen(player);

                    end_time = clock();

                    // Calcula o tempo decorrido
                    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

                    // Se passou um segundo, calcule e exiba os FPS
                    if (elapsed_time >= 1.0) {
                        fps = frame_count / elapsed_time;
                        printf("FPS: %.2f\n", fps);

                        // Reinicie o contador de frames e o tempo inicial
                        frame_count = 0;
                        start_time = clock();
                    }

                
                }

                else {
                    set_pause_screen();
                    while (!stop) {}
                    set_bg_screen();
                }

            }


        }

        //game over
        if (lifes == 0) {
            reset_sprites();
            set_game_over_screen();
            while (1){
                if(end == 1) break;
                if(start == 1) break;

            }
            erase_bg_screen();    
        }
    }

    reset_sprites();
    erase_bg_screen();
    setBackground(black);

    // Fechar os dispositivos
    close(fd_mouse);
    // close(fd_gpu);
    
    // Destruir o mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
