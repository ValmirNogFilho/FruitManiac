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

int xsoma = 0, ysoma = 0;
int fd_mouse;
pthread_mutex_t lock;
struct input_event ev;

#define LW_BRIDGE_BASE         0xFF200000
#define LW_BRIDGE_SPAN         0x00005000

#define nove 0b0010000


#define oito 0b0000000


#define sete 0b1111000


#define seis 0b0000010


#define cinco 0b0010010


#define quatro 0b0011001

#define tres 0b0110000

#define dois 0b0100100

#define zero 0b1000000

#define um 0b1111001


int i, j;
color_t green = {0,7,0};
color_t red = {7, 0, 0};
color_t white = {7,7,7};
color_t brown = {6,3,0};
color_t bg = {6, 7, 7};

// void ladrao(){
    //-----------------------------------------------------------------------------------------------------------------------//   
//LADRÃO PADRÃO

// int x, y;
// color_t cor;
// for (y = 0; y < 21; y++) {
//     for (x = 0; x < 20; x++) {
//         int endereco = 10000 + y * 20 + x;
//         int azul = 7, verde = 7, vermelho = 6; // Cor padrão (transparente)

//         //Condições para preto
//         if (
//             ((x == 5 || x == 15) && (y >= 8 && y <= 11)) ||

//             ((x == 6 || x == 14 ) && ((y >= 3 && y <= 5) || (y >= 7 && y <= 11))) ||

//             (x == 7 && ((y >= 2 && y <= 9) || (y >= 12 && y <= 18))) ||

//             (x == 8 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 8) || (y >= 12 && y <= 18))) ||

//             (x == 9 && ((y >= 1 && y <= 3) || (y >= 5 && y <= 7) || (y >= 12 && y <= 18))) ||

//             (x == 10 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 6) || (y >= 11 && y <= 13))) ||

//             (x == 11 && ((y >= 1 && y <= 3) || (y >= 5 && y <= 6) || (y >= 10 && y <= 18))) ||

//             (x == 12 && ((y >= 1 && y <= 2) || (y >= 5 && y <= 6) || (y >= 9 && y <= 18))) ||

//             (x == 13 && ((y >= 2 && y <= 6) || (y >= 8 && y <= 18)))
//         ) {
//             vermelho = 0;
//             verde = 0;
//             azul = 0;
//         }

//         //Condições para marrom
//         else if (
//             (x == 7 && (y >= 10 && y <= 11)) ||

//             (x == 8 && (y >= 9 && y <= 11)) ||

//             (x == 9 && (y >= 8 && y <= 11)) ||

//             (x == 10 && (y >= 7 && y <= 10)) ||

//             (x == 11 && (y >= 7 && y <= 9)) ||

//             (x == 12 && (y >= 7 && y <= 8)) ||

//             (x == 13 && y == 7)
//         ) {
//             vermelho = 4;
//             verde = 2;
//             azul = 2;
//         }

//         //Condições para rosa cor de pele
//         else if (
//             ((x == 8 || x == 12 || x == 10) && (y >= 3 && y <= 4)) ||
//             ((x == 9 || x == 11) && y == 4) ||
//             ((x == 5 || x == 6 || x == 14 || x == 15)  && y == 12)
//         ) {
//             vermelho = 7;
//             verde = 6;
//             azul = 5;
//         }

//         if (y == 0 || y == 20) {
//             azul = 7; 
//             verde = 7; 
//             vermelho = 6;
//         }

//         cor.R = vermelho;
//         cor.G = verde;
//         cor.B = azul;
//         setPixelOnSpriteMemory(endereco, cor);
//     }

// }
// }

void draw_apple(){
    int x, y, endereco;
    color_t cor = {6, 7, 7};
    color_t blue = {0,0,7};


    FILE *file;
    int r, g, b;
    int i, j;
    int address;
    color_t color;
    // Abra o arquivo em modo leitura
    file = fopen("sprites/apple.sprite", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Leia cada linha e separe os valores
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            address = 10000 + 20 * j + i;
            if(fscanf(file, "%d %d %d", &r, &g, &b) != EOF) {
                color.R = r;
                color.G = g;
                color.B = b;

                setPixelOnSpriteMemory(address, color);
            }
        }
    }

    fclose(file);

    return 0;

}

int open_physicall (int fd) {
    if (fd == -1)
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }
    return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physicall (int fd) {
    close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
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

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physicall(void * virtual_base, unsigned int span) {
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }
    return 0;
}




// Define as cores
// void draw_apple() {
//     int x, y;
//     for (y = 0; y < 20; y++) {
//         for (x = 0; x < 20; x++) {
//             int endereco = 10000 + y * 20 + x;
//             color_t cor;
//             cor.R = 6;
//             cor.G = 7;
//             cor.B = 7;


//             // pixels da maçã
//             if ((x - 10) * (x - 10) + (y - 10) * (y - 10) <= 36) {
//                 cor.R = 7;
//                 cor.G = 0;
//                 cor.B = 0;
//             }

//             // caule
//             if ((x >= 9 && x <= 11) && (y >= 2 && y <= 6)) {
//                 cor.R = 6;
//                 cor.G = 3;
//                 cor.B = 0;
//             }

//             // folha
//             if ((x >= 5 && x <= 7) && (y >= 2 && y <= 4)) {
//                 cor.R = 0;
//                 cor.G = 7;
//                 cor.B = 0;
//             }
//             // cor.R = 0;
//             // cor.G = 0;
//             // cor.B = 0;
//             setPixelOnSpriteMemory(endereco, cor);
//         }
//     }
// }

/**
#define BACKGROUND_COLOR {7, 7, 7}
#define APPLE_COLOR {7, 0, 0}
#define ORANGE_COLOR {7, 7, 0}
#define PEAR_COLOR {0, 7, 0}
#define STEM_COLOR {6, 3, 0}
#define LEAF_COLOR {0, 7, 0}

*/

void draw_orange() {
    int x, y;
    color_t cor = {6,7,7};
    int endereco;
    for (x = 0; x < 20; x++) {
        for (y = 0; y < 21; y++) {
            endereco = 10400 + y * 20 + x;

            // pixels da laranja
            if ((x - 10) * (x - 10) + (y - 10) * (y - 10) <= 36) {
                cor.R = 6;
                cor.G = 3;
                cor.B = 1;
            }

            // caule
            if ((x >= 9 && x <= 11) && (y >= 2 && y <= 6)) {
                cor.R = 0;
                cor.G = 0;
                cor.B = 0;
            }

            setPixelOnSpriteMemory(endereco, cor);
        }
    }
}


void draw_pear() {
    int x, y;
    int endereco;
    color_t cor = {6, 7, 7};

    for (x = 0; x < 20; x++) {
        for (y = 0; y < 21; y++) {
            endereco = 10800 + y * 20 + x;

            // pixels da pêra
            if ((x >= 7 && x <= 14) && (y >= 8 && y <= 16) &&
                !(x >= 9 && x <= 12 && y >= 10 && y <= 14)) {
                cor.R = 0;
                cor.G = 7;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }

            // caule
            if ((x >= 10 && x <= 12) && (y >= 2 && y <= 6)) {
                cor.R = 6;
                cor.G = 3;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }

            // folha
            if ((x >= 7 && x <= 9) && (y >= 1 && y <= 3)) {
                cor.R = 0;
                cor.G = 7;
                cor.B = 0;
                setPixelOnSpriteMemory(endereco, cor);
            }


            setPixelOnSpriteMemory(endereco, cor);
        }
    }
}




int verificarNumero(int numero) {
    switch (numero) {
        case 9:
            return nove;
        case 8:
            return oito;
        case 7:
            return sete;
        case 6:
            return seis;
        case 5:
            return cinco;
        case 4:
            return quatro;
        case 3:
            return tres;
        case 2:
            return dois;
        case 0:
            return zero;
        case 1:
            return um;
        default:
            printf("Número %d não tem correspondência definida.\n", numero);
            break;
    }
}


int tiros;




unsigned int click2 = 0;

int collision_between_sprites(sprite_t spr1, sprite_t spr2) {
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
        if (ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value == 1) {
            click2 = ev.value;
            
        
        }

        

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    int x, y;
    int fd_gpu;
    sprite_t spr;
    sprite_t beam;

    int vidas=5;

    sprite_t enemy;
    sprite_t enemy1;
    sprite_t enemy2;
    sprite_t fruit, fruit1, fruit2, fruit3, fruit4, fruit5;

    color_t color = {0, 3, 0};
    color_t click = {0, 7, 0};
    unsigned int block;
    int i;
    color_t reset = {6, 7, 7};
    pthread_t mouse_thread;

    //draw_x();
    draw_apple();
    draw_pear();
    draw_orange();
    

    fruit.address = 9;
    fruit.rel_x = 0;
    fruit.rel_y = 20;
    fruit.variation = 25;
    fruit.visible = 0;

    fruit1.address = 10;
    fruit1.rel_x = 0;
    fruit1.rel_y = 20;
    fruit1.variation = 26;
    fruit1.visible = 0;

    fruit2.address = 11;
    fruit2.rel_x = 0;
    fruit2.rel_y = 20;
    fruit2.variation = 27;
    fruit2.visible = 0;


    fruit3.address = 12;
    fruit3.rel_x = 0;
    fruit3.rel_y = 20;
    fruit3.variation = 25;
    fruit3.visible = 0;

    fruit4.address = 13;
    fruit4.rel_x = 0;
    fruit4.rel_y = 20;
    fruit4.variation = 26;
    fruit4.visible = 0;

    fruit5.address = 14;
    fruit5.rel_x = 0;
    fruit5.rel_y = 20;
    fruit5.variation = 27;
    fruit5.visible = 0;


    enemy.address = 8;
    enemy.rel_x = 0;
    enemy.rel_y = 20;
    enemy.variation = BOMB;
    enemy.visible = 0;

    enemy1.address =2;
    enemy1.rel_x = 0;
    enemy1.rel_y = 20;
    enemy1.variation = 25;
    enemy1.visible = 0;




    enemy2.address =3;
    enemy2.rel_x = 0;
    enemy2.rel_y = 20;
    enemy2.variation = DIAMOND;
    enemy2.visible = 0;


    spr.address = 5;
    spr.rel_y = 459;
    spr.variation = ENEMY_1;
    spr.visible = 1;

    beam.address = 4;
    beam.rel_y = 0;
    beam.variation = BEAM_VERTICAL;
    beam.visible = 0;

    setBackground(color);

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




    volatile int * DISPLAY_ptr;
    volatile int * DISPLAY_ptrum;
    volatile int * DISPLAY_ptrdois;
    volatile int * DISPLAY_ptrtres;
    volatile int * DISPLAY_ptrquarto;
    volatile int * DISPLAY_ptrquinto;
    volatile int * chave;

    int fd = -1;               // used to open /dev/mem for access to physical addresses
    void *LW_virtual;          // used to map physical addresses for the light-weight bridge
    
   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physicall (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physicall (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   DISPLAY_ptr = (unsigned int *) (LW_virtual + HEX0_BASE);
   
   DISPLAY_ptrum = (unsigned int *) (LW_virtual + HEX1_BASE);

   DISPLAY_ptrdois = (unsigned int *) (LW_virtual + HEX2_BASE);
   
   DISPLAY_ptrtres = (unsigned int *) (LW_virtual + HEX3_BASE);

   DISPLAY_ptrquarto = (unsigned int *) (LW_virtual + HEX4_BASE);
   
   DISPLAY_ptrquinto = (unsigned int *) (LW_virtual + HEX5_BASE);



    // Inicializar o mutex
    pthread_mutex_init(&lock, NULL);

    // Criar a thread para leitura do mouse
    pthread_create(&mouse_thread, NULL, read_mouse, NULL);
    int primeironum;
    int segundonum;
    int terceironum;






    *DISPLAY_ptrtres=verificarNumero(5); 
    *DISPLAY_ptrquarto=verificarNumero(0);    
    *DISPLAY_ptrquinto=verificarNumero(0);	



    int numero;
    double fator = 620.0 / (RAND_MAX + 1.0);  // Calcula o fator de escala

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Gera um número aleatório entre 0 e 619

    int numero_aleatorio;
    int numero_aleatorio1;
    int numero_aleatorio2;



    srand(time(NULL));
    sprite_t lista1[9]={enemy,enemy1,enemy2, fruit, fruit1, fruit2, fruit3, fruit4, fruit5};

    // Loop principal
    int whileCounts[9] = {0};
    while (1) {

        int i;
        for(i=0;i<9;i++){
            whileCounts[i]++;
            if(lista1[i].visible==0){
                
                
                lista1[i].rel_x = (int)(rand() * fator);
                lista1[i].visible=1;    
                
            }
            else {
                if(whileCounts[i] >= 15){
                    lista1[i].rel_y++;
                    whileCounts[i] = 0;

                }

                if(lista1[i].rel_y > 479) {

                    lista1[i].rel_y = 0;
                    lista1[i].rel_x = (int)(rand() * fator);
                    vidas--;
                    setBackground(red);
                    usleep(100000);
                    *DISPLAY_ptrtres=verificarNumero(vidas);    	
                    setBackground(color);
                }
            }
            setSpriteOnScreen(lista1[i]);   
        }

        //if(vidas<0) break;


        // Atualiza as coordenadas acumuladas
        pthread_mutex_lock(&lock);

        
        
        x = xsoma;
        y = ysoma;
        if(click2 == 1){
            beam.visible = 1;
            beam.rel_y = 439;
            beam.rel_x = xsoma;
            click2=0;


            
        }
        
        if (beam.rel_y >= 11 && beam.visible == 1) {
            beam.rel_y -= 1;
        }
        
        int t, u;
        for(t = 0; t<9; t++){
            if(collision_between_sprites(beam, lista1[t]) == 1) {
                if(lista1[t].variation != BOMB)
                {
                    setBackground(red);
                    usleep(100000);
                    setBackground(color);
                    vidas--;
                    *DISPLAY_ptrtres=verificarNumero(vidas);
                }
            
                tiros++;
                primeironum = tiros % 10;
                segundonum= (tiros % 100) / 10;
                terceironum = tiros / 100;  

                *DISPLAY_ptr = verificarNumero(primeironum);
                *DISPLAY_ptrum=verificarNumero(segundonum);    
                *DISPLAY_ptrdois=verificarNumero(terceironum);	
                
                printf("colidiu \n");
                lista1[t].visible = 0;
                lista1[t].rel_y = 0;
                beam.visible=0;
            }

            if(collision_between_sprites(spr, lista1[t]) == 1) {
                if(lista1[t].variation == BOMB)
                {
                    setBackground(red);
                    usleep(100000);
                    setBackground(color);
                    vidas--;
                    *DISPLAY_ptrtres=verificarNumero(vidas);
                }
                else {
                    for(u = 0 ; u < 10; u++){
                        spr.rel_y-= 2;
                        setSpriteOnScreen(spr);
                        usleep(5000);
                    }
                    for(u = 0 ; u < 10; u++){
                        spr.rel_y+= 2;
                        setSpriteOnScreen(spr);
                        usleep(5000);
                    }
                }
                lista1[t].rel_y = 0;
                lista1[t].visible = 0;
                
            }

        }
        
        
        pthread_mutex_unlock(&lock);

        setSpriteOnScreen(beam);


        // Criar a estrutura do sprite com as novas coordenadas
        spr.rel_x = x;


        setSpriteOnScreen(spr);
        // int i;                *DISPLAY_ptr = verificarNumero(primeironum);

        // for(i=1;i<32;i++){
        //     enemy.address=i;
        //     enemy.visible=0;
        //     setSpriteOnScreen(enemy);
        // }
        
        
    }

    // Fechar os dispositivos
    close(fd_mouse);
    close(fd_gpu);

    // Destruir o mutex
    pthread_mutex_destroy(&lock);

    return 0;
}