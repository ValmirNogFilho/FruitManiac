#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "lib/gpulib.h"
#include <time.h>
#include "enderecos.h"
#include <fcntl.h>
#include <sys/mman.h>

#define DEVICE_PATH "/dev/gpu123"
#define MOUSE_DEVICE "/dev/input/event0"
#define MAX 511
#define MAX_ELEMENTS 9
#define DIFFICULTY_CRITERIA 5
int stop;
int i, j;
int xsoma = 0, ysoma = 0;
int fd_mouse;
pthread_mutex_t lock;
struct input_event ev;
int score;
unsigned int click_reset = 0;

sprite_t beam = {4, BEAM_VERTICAL, 0, 0, 0};
sprite_t player, bomb,apple,diamond, apple1, orange, pear, apple2, orange1, pear1;

#define LW_BRIDGE_BASE         0xFF200000
#define LW_BRIDGE_SPAN         0x00005000


#define zero 0b1000000
#define one 0b1111001
#define two 0b0100100
#define three 0b0110000
#define four 0b0011001
#define five 0b0010010
#define six 0b0000010
#define seven 0b1111000
#define eight 0b0000000
#define nine 0b0010000


clock_t start_time, end_time;
double fps;
int frame_count = 0;



color_t green = {0,7,0};
color_t red = {7, 0, 0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {0, 3, 0};
color_t transparent = {6, 7, 7};

void draw(unsigned char* path, int initial_address){
    int x, y, endereco;
    color_t cor = {6, 7, 7};
    color_t blue = {0,0,7};
    FILE *file;
    int r, g, b;
    int i, j;
    int address;
    color_t color;


    // Abra o arquivo em modo leitura
    file = fopen(path, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Leia cada linha e separe os valores
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            address = initial_address + 20 * j + i;
            if(fscanf(file, "%d %d %d", &r, &g, &b) != EOF) {
                color.R = r;
                color.G = g;
                color.B = b;

                setPixelOnSpriteMemory(address, color);
            }
        }
    }

    fclose(file);

    return;

}


int open_physicall (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}



void* map_physicall(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }
    return virtual_base;
}





void draw_apple_on_memory() {
    draw("sprites/apple.sprite", 10000); //endereço 25
}

void draw_orange_on_memory() {
    draw("sprites/orange.sprite", 10400); //endereço 26
}

void draw_pear_on_memory() {
    draw("sprites/pear.sprite", 10800); //endereço 27
    for(j = 0; j < 20; j++) {
            setPixelOnSpriteMemory(11200+j, transparent); //eliminando linha extra exibida pela gpu
    }
}


int open_physical_file (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical_file (int fd) {
    close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span) {
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }
    return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }
    return 0;
}


int linkNumberTo7SegCode(int number) {
    switch (number) {
        case 9:
            return nine;
        case 8:
            return eight;
        case 7:
            return seven;
        case 6:
            return six;
        case 5:
            return five;
        case 4:
            return four;
        case 3:
            return three;
        case 2:
            return two;
        case 1:
            return one;
        case 0:
            return zero;
        default:
            printf("Número %d não tem correspondência definida.\n", number);
            break;
    }
}



int collision_between_sprites(sprite_t spr1, sprite_t spr2) {
    if (spr1.rel_x < 10 && spr2.rel_x < 10) //lado extremo esquerdo: x < 10
        return (spr1.rel_x <= spr2.rel_x + 10 && spr1.rel_y >= (int) (spr2.rel_y -10) && spr1.rel_y <= spr2.rel_y + 10 &&(spr1.visible==1 && spr2.visible==1));
    return (spr1.rel_x >= (int) (spr2.rel_x -10) && spr1.rel_x <= spr2.rel_x + 10 && spr1.rel_y >= (int) (spr2.rel_y -10) && spr1.rel_y <= spr2.rel_y + 10 &&(spr1.visible==1 && spr2.visible==1));
}

// Função da thread para leitura do mouse
void* read_mouse(void* arg) {
    
    ssize_t n;
    int x, y;

    while (1) {
        n  = read(fd_mouse, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Error reading");
            continue;
        } else if (n != sizeof(ev)) {
            fprintf(stderr, "Error: read %ld bytes, expecting %ld\n", n, sizeof(ev));
            continue;
        }

        pthread_mutex_lock(&lock);

        if (ev.type == EV_REL && ev.code == REL_X) {
            x = ev.value;
            xsoma += x;
        }
        if (ev.type == EV_REL && ev.code == REL_Y) {
            y = ev.value;
            ysoma += y;
        }

        // Limitar as coordenadas acumuladas para evitar overflow
        if (xsoma < 0) xsoma = 0;
        if (xsoma > 619) xsoma = 619;
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1 && beam.rel_y == 0) {
            click_reset = ev.value;
            
        
        }

        

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}











int restart;
// Função da thread para leitura do mouse
void* read_botao(void* arg) {
    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physicall (fd)) == -1)
        return (-1);
    if ((LW_virtual = map_physicall (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
        return (-1);


        
    volatile int * chave;
    chave = (unsigned int *) (LW_virtual + KEYS_BASE);
    ssize_t n;
    int x, y;
    int pause_state;
    pause_state=1;
    int restart_state=1;
    restart=0;
    stop =1;
    while (1) {
        if(*chave==14 && pause_state ==1){
          pause_state=0; 
          if(stop == 1) {
            stop = 0;
            printf("%d",stop);
          } else {
            stop = 1;
            printf("%d",stop);

          }

        }

        if(*chave==13 && restart_state ==1){//11 7
          restart_state=0; 
          if (restart == 0) {
            restart = 1;
          } 
        }

        if(*chave==15 && restart_state==0){
            restart_state=1;
            restart=0;


        }

        if(*chave==15 && pause_state==0){
            pause_state=1;
        }

    }

    return NULL;
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
    defineSkinsToFallingElements();
    draw_apple_on_memory();
    draw_orange_on_memory();
    draw_pear_on_memory();

}

int main() {
    start_time = clock();
    int x, y;
    int fd_gpu;

    color_t color = {0, 3, 0};
    color_t click = {0, 7, 0};
    unsigned int block;
    color_t reset = {6, 7, 7};
    pthread_t mouse_thread;
    pthread_t botao_thread;
    int t, u;
    volatile int * DISPLAY_ptr;
    volatile int * DISPLAY_ptr1;
    volatile int * DISPLAY_ptr2;
    volatile int * DISPLAY_ptr3;
    volatile int * DISPLAY_ptr4;
    volatile int * DISPLAY_ptr5;
    // volatile int * key;

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    int hundreds;
    int dozens;
    int units;
    int numero;
  
    // Gera um número aleatório entre 0 e 619

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    setup();
    
    sprite_variation_t skins[4] = {BOMB, 25, 26, 27};

    setBackground(bg);

    // Abrir o dispositivo do mouse
    fd_mouse = open(MOUSE_DEVICE, O_RDONLY );
    if (fd_mouse == -1) {
        perror("Cannot open input device");
        return 1;
    }

    // Abrir o dispositivo da GPU
    fd_gpu = open(DEVICE_PATH, O_WRONLY);
    if (fd_gpu == -1) {
        perror("Cannot open GPU device");
        close(fd_mouse);
        return 1;
    }




    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical_file (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX0_BASE);
   
   DISPLAY_ptr1 = (unsigned int *) (LW_virtual + HEX1_BASE);

   DISPLAY_ptr2 = (unsigned int *) (LW_virtual + HEX2_BASE);
   
   DISPLAY_ptr3 = (unsigned int *) (LW_virtual + HEX3_BASE);

   DISPLAY_ptr4 = (unsigned int *) (LW_virtual + HEX4_BASE);
   
   DISPLAY_ptr5 = (unsigned int *) (LW_virtual + HEX5_BASE);

//    key = (unsigned int *) (LW_virtual + KEYS_BASE);


    // Inicializar o mutex
    pthread_mutex_init(&lock, NULL);

    // Criar a thread para leitura do mouse
    pthread_create(&mouse_thread, NULL, read_mouse, NULL);
    pthread_create(&botao_thread, NULL, read_botao, NULL);






    


    // double factor = 620.0 / (RAND_MAX + 1.0);  // Calcula o factor de escala

    // sprite_t fallingElements[9]={bomb,apple,diamond, apple1, orange, pear, apple2, orange1, pear1};




    // int whileCounts[10] = {0}; //controla fps dos falling elements e do tiro




    int number_of_elements = 1;
    int i;
    int delay = 15;
    while(1){
        int lifes=5;
        score=0;
        *DISPLAY_ptr3=linkNumberTo7SegCode(5); 
        *DISPLAY_ptr4=linkNumberTo7SegCode(0);    
        *DISPLAY_ptr5=linkNumberTo7SegCode(0);	    
        double factor = 620.0 / (RAND_MAX + 1.0);  // Calcula o factor de escala
        sprite_t fallingElements[9]={bomb,apple,diamond, apple1, orange, pear, apple2, orange1, pear1};

        int whileCounts[10] = {0}; //controla fps dos falling elements e do tiro
    
        if(!restart){
        while (1) {
            if(restart){
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
                        *DISPLAY_ptr3=linkNumberTo7SegCode(lifes);    	
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
                        *DISPLAY_ptr3=linkNumberTo7SegCode(lifes);
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
                        *DISPLAY_ptr3=linkNumberTo7SegCode(lifes);
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

            //if(lifes<0) break;


            // Atualiza as coordenadas acumuladas
            //pthread_mutex_lock(&lock);

            
            
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

            *DISPLAY_ptr = linkNumberTo7SegCode(hundreds);
            *DISPLAY_ptr1=linkNumberTo7SegCode(dozens);    
            *DISPLAY_ptr2=linkNumberTo7SegCode(units);	

            setSpriteOnScreen(beam);


            // Criar a estrutura do sprite com as novas coordenadas
            player.rel_x = x;


            setSpriteOnScreen(player);
            // int i;                *DISPLAY_ptr = linkNumberTo7SegCode(hundreds);

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
        }
    
        }
    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);

    // Destruir o mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
