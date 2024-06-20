#include <linux/kernel.h> //para usar funcoes como printk etc
#include <linux/module.h> //macros de definacao de licensa e autores
#include <linux/init.h> //usado para iniciar o modulo e finalizar 
#include <asm/io.h> //para usar o ioremap
#include "address_map_arm.h" //endereços da ponte
#include <linux/uaccess.h> //usa as funcoes copy_to_user e copy_from_user
#include <linux/fs.h> //arquivo de operacoes, que permite uso de funcoes como open/close e read/write para a gpu
#include <linux/cdev.h> //char driver usa o cdev, usado para registrar o driver no kernel
#include <linux/delay.h> //necessario para usar o delay para controlar o uso das fifo

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Icaro, Valmir, Fabio e Nalbert");
MODULE_DESCRIPTION("DRIVER PARA GPU");
//CONSTANTES
#define DATA_A  0x80 //endereço do FIFO de instrucoes
#define DATA_B  0x70 //endereço do FIFO de dados
#define START 0xc0 //endereço do sinal para ''confirmar'' o envio de dados
#define WR_FULL 0xb0 //endereço do sinal que dirá se a fifo ta cheia

#define DRIVER_NAME "gpu123" //nome

static unsigned char data[8];  //Buffer que guardará os dados que serão passados para GPU

void * LW_virtual; // Endereço base da ponte Lightweight 
volatile int *DATA_A_PTR, *DATA_B_PTR, *START_PTR, *WR_FULL_PTR; // "endereços virtuais"


//struct que guarda o major e minor number do driver
static struct drvgpu_data{
    dev_t devnum; //guardará o major number retornado pelo kernel
    struct cdev cdev; //usado para representar o driver de caracteres
} drvgpu_data;


//inode é usado para salvar os metadados do arquivo (se ele faz operacoes de leitura e escrita), fornece informacoes sobre o dispositivo acessado
//filp é o arquivo associado ao driver, mantem informações sobre o estado do arquivo (se é so leitura ou escrita)

int
deviceOpen(struct inode *inode,struct file *filp){ 
        printk(KERN_INFO "aberto");
        return 0;
}


int ret;

int t;

//filp é uma strcut do tipo file, ela serve como um ''''ponteiro'''' que aponta para qual driver esse arquivo está relacionado
ssize_t
deviceWrite(struct file* filp, const unsigned char *bufSourceData, size_t bufCount, loff_t* curOffset){
    int ret;
    unsigned int concat, concat1;
    printk(KERN_INFO "escrevendo");

    //copia do espaço do usário para o buffer do driver
    ret = copy_from_user(data, bufSourceData, bufCount);//primeiro parametro é o buffer do driver, segundo é o buffer do lado do usário e o terceiro o tamanho do buffer(usuario)
    if (ret != 0) {
        printk(KERN_ERR "Falhou na cópia de dados do espaço do usuário\n");
        return -EFAULT; 
    }

    //lógica utilizada para evitar perca de dados quando a fifo está cheia
    while(*WR_FULL_PTR){ //enquanto a fifo estiver cheia continue no loop
        msleep(10);     //dê um tempo para esvaziar
        if(*WR_FULL_PTR == 0)// se a fifo estiver vazia, saia do while loop
          break;
    }

    concat = data[7] << 24 | data[6] << 16 | data[5] << 8 | data[4];//pegando os ultimo valores do buffer e deslocando para criar uma palavra de dados 32 bits
    concat1 = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];//pegando os primeiros valores do buffer e deslocando para formar uma palavra de instrucao de 32 bits


    *START_PTR = 0; //sinal antes de escrever
    *DATA_A_PTR = concat1; //passando os valores para a Fifo de instrucoes
    *DATA_B_PTR = concat; // passando os valores para fifo de dados
    *START_PTR = 1; //habilitando o envio dos dados da fifo pro processador gráfico

    return bufCount; //retornando o tamanho do buffer que foi mandado
}



int
deviceClose(struct inode *inode, struct file *filp){
        return 0;
}

//função usado nos testes de passagem de dados
ssize_t
deviceRead(struct file* filp,char *bufDestination, size_t bufCount, loff_t* curOffset){
        int ret;
        ret=copy_to_user(bufDestination,data + *curOffset,bufCount); //copiando os dados escritos no kernel para o espaço do usário 
        if (ret != 0) {
                printk(KERN_ERR "Falhou na coía de dados para o espaco de usuario\n");
                return -EFAULT; // 
        }
        printk(KERN_INFO "lendo");
        return bufCount;
}


//struct do tipo file_operations define que tipo de operacoes podem ser feitas no dispotivo
//são ponteiros para as funcoes do dispotivo
static const struct file_operations fops = {
        .owner=THIS_MODULE,
        .open = deviceOpen, //aponta para o metodo no momento que abre o dispotivo
        .release = deviceClose, //aponta para o metodo no momento que fecha o dispotivo
        .write = deviceWrite, //aponta para o metodo no momento que deve-se escrever no dispotivo
        .read = deviceRead, //aponta para o metodo no momento que deve-se ler o dispotivo
};
   

static int __init
init_kernelgpudriver(void) {


    int result;


    //alocando um ''major number'' para associar posteriormente com um arquivo de dispositivo

    //devnum vai guardar o major number que vai ser usado para adicionar o driver no sistema
    //o segundo parametro ''0'' é o minor number, usado para dinstiguir drivers com o msm major number, usado também no momento de criação do node 
    //numero de minor number ligado ao dispotivo, só 1
    result = alloc_chrdev_region(&drvgpu_data.devnum, 0, 1, DRIVER_NAME); 


    if (result) {
        pr_err("%s: Não foi possível alocar um numero para o driver!\n", DRIVER_NAME);//verificando se deu tudo certo
        return result;
    }


    //inicializando o char driver, configurando esse tipo cdev(char driver) para operações de leitura e escrita (e outras) com o dispositivo

    // primeiro argumento é a estrutura básica para char drivers, está sendo inicializada
    //segundo é um ponteiro para uma struct ''file operations'', que define o que pode ser feito no dispotivo
    cdev_init(&drvgpu_data.cdev, &fops);



    //adicionando o char driver ao sistema de dispositivos do tipo char, no kernel 

    //primeiro argumento é a estrura cdev (que representa o driver)
    //segundo é o que contem o major e minor number
    //ultimo argumento o numero de dispositivos sendo registrados
    result = cdev_add(&drvgpu_data.cdev, drvgpu_data.devnum, 1);

    if (result) {
        pr_err("%s: Falha no registro do driver de caracters\n", DRIVER_NAME);//verificando se o registro teve sucesso
        unregister_chrdev_region(drvgpu_data.devnum, 1);
        return result;
    }

    //mapeando memória física em memoria virtual para protecao 
    LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    
    //endereços para acesso de fifos e sinais de controle 
    DATA_A_PTR = (unsigned int *) (LW_virtual + DATA_A);
    DATA_B_PTR = (unsigned int *) (LW_virtual + DATA_B);  
    START_PTR = (unsigned int *) (LW_virtual + START);  
    WR_FULL_PTR = (unsigned int *) (LW_virtual + WR_FULL);  


    pr_info("%s: initialized.\n", DRIVER_NAME);


   return 0;
}


static void __exit
exit_kernelgpudriver(void) {
   *START_PTR = 0;
    iounmap (LW_virtual); //''desmapeando''
    cdev_del(&drvgpu_data.cdev);//deletando a representacao do driver
    unregister_chrdev_region(drvgpu_data.devnum, 1); //excluindo os major e minor number
    pr_info("%s: exiting.\n", DRIVER_NAME);
}


module_init(init_kernelgpudriver);
module_exit(exit_kernelgpudriver);


