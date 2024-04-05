#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
/*
 * print the errors
 */
void print_error(char *message,void *extra){
	printf("\033[31m[-] \033[00m");
	printf(message,extra);
}
/*
 * print the messages
 */
void print_message(char *message,void *extra){
	printf("\033[32m[*] \033[00m");
	printf(message,extra);
}
/*
 * print the informations
 */
void print_info(char *message,void *extra){
	printf("\033[34m[+] \033[00m");
	printf(message,extra);
}
/*
 * print colored bytes
 */
void colored_byte(char *message,void *extra,char byte){
		if(byte == 0){
			printf("\033[31m");
		}else if(byte < 31){
	        printf("\033[32m");
	 	}else if(byte > 127){
	 	    printf("\033[34m");
		}else{
			printf("\033[00m");
		}
		printf(message,extra);
		printf("\033[00m");
}
/*
 * function to detect the file size
 */
int get_file_size(int fd){
	struct stat stat_struct;
	if(fstat(fd,&stat_struct) == -1){
		return -1;
	}
	return (int) stat_struct.st_size;
}
/*
 * the usage function
 */
void usage(char * programme){
	printf("Usage %s [-f FILE][-r][-x/-t]\n",programme);
	printf("       _______              _                   _______\n");
	printf("       \\------\\   _      __(_)___  ____ ______ /------/\n");
	printf("        \\-----|  | | /| / / / __ \\/ __ `/ ___/ |-----/\n");
	printf("         \\----|  | |/ |/ / / / / / /_/ (__  )  |----/\n");
	printf("          \\---|  |__/|__/_/_/ /_/\\__, /____/   |---/\n");
	printf("           \\__/                 /____/         \\__/\n");
	printf("            |   _____________________________    |\n");
	printf("            |__|                             |___|\n");
	printf("               |    Created By Yassin0xe     |\n");
	printf("               |_____________________________|\n");
	printf("\n");
	printf(" Options\n\n");
	printf("\t--file      -f FILE     The directory to thr shellcode file\n");
	printf("\t--run       -r          Run the Shell Code\n");
	printf("\t--hexdump   -x          Show The shellCode As Hexdump\n");
	printf("\t--textdump  -t          Show the shellcode as Textump\n");
	printf("\t--help      -h          Show The HElp Menu\n");
	printf("\n");
	printf(" Exemples\n\n");
	printf("\t%s -f shellcode.bin -r\n",programme);
	printf("\t%s -f shellcode.bin -r -x\n",programme);
	printf("\t%s -f shellcode.bin -r -t\n",programme);
	printf("\t%s -f shellcode.bin -x\n",programme);
	printf("\t%s -f shellcode.bin -t\n",programme);
	exit(-1);
}
/*
 * print shellcode as hexdump format
 */
void hexdump(const char * data_buffer,int length){
	unsigned char byte;
	unsigned int i,j=0,m=0;
	printf("\n");
	for(i = 0;i<length;i++){
		if(i%16 == 0){
			printf("%08x  ",m);
			m++;
		}
		if((i%8 == 0) && (i%16 != 0)){
			printf("  ");
		}
		byte = data_buffer[i];
		colored_byte("%02x ",byte,byte);

		if((i%16 == 15) || (i == length-1)){
			for(j=0;j < 15 - i%16;j++){
				printf("   ");
				if(((j+1)%8 == 0) && ((j+1)%16 != 0)){
					printf("  ");
				}
			}
			printf("| ");
			for(j=(i-i%16);j<=i;j++){
				byte = data_buffer[j];
				if((j%8 == 0) && (j%16 != 0)){
					printf("  ");
				}
			 	if(byte<127 && byte>31){
					printf("%c",byte);
				}
				else{
					colored_byte(".",NULL,byte);
				}
			}
			printf("\n");
		}

	}
	printf("\n");
}
/*
 * text dump function to the shell as a array
 * for c language
 */
void textdump(const char * data_buffer,int length){
	unsigned char byte;
	unsigned int i,j=0,m=0;
	printf("\n");
	printf("unsigned char buffer[] = \n");
	for(i = 0;i<length;i++){
		if(i%16 == 0){
			printf("\"");
			m++;
		}
		byte = data_buffer[i];
		printf("\\x%02x",byte);

		if((i%16 == 15) || (i == length-1)){
			printf("\"\n");
		}

	}
	printf("\n");
}
/*
 * run the allocated shellcode in the memory
 */
void run_shellcode(char * shellcode){
	int (*run_shell)();
	run_shell = (int (*)()) shellcode;
	print_info("the shellcode is at %p\n",shellcode);
	print_message("start runing the shellcode\n",NULL);
	(int)(*run_shell)();
	exit(1);
}
/*
 * the main function
 */
int main(int argc,char * argv[]){
	if(argc == 1){
		usage(argv[0]);
	}
	int i;
	char * file;
	int dump_state=0,run_state=0;
	for(i=1;i<argc;i++){
		if(!strcmp(argv[i],"--run") || !strcmp(argv[i],"-r")){
			run_state = 1;
		}
		else if(!strcmp(argv[i],"--hexdump") || !strcmp(argv[i],"-x")){
			dump_state = 1;
		}
		else if(!strcmp(argv[i],"--textdump") || !strcmp(argv[i],"-t")){
			dump_state = 2;
		}
		else if(!strcmp(argv[i],"--file") || !strcmp(argv[i],"-f")){
			i++;
			if(argv[i] && (argv[i][0] != '-')){
				file = argv[i];
			}else{
				usage(argv[0]);
				exit(-1);
			}
		}else{
			usage(argv[0]);
		}
	}
	print_message("opening (%s)\n",file);
	int fd = open(file,O_RDONLY,0);
	if(fd == -1){
		print_error("failed open (%s)\n",argv[1]);
		exit(-1);
	}
	int file_size;
	file_size = get_file_size(fd);
	if(file_size == -1){
		print_error("failed get the file size\n",NULL);
		close(fd);
		exit(-1);
	}
	unsigned char shellcode[file_size];
	if(shellcode == NULL){
		print_error("[-] failed memory allocation\n",NULL);
		close(fd);
		exit(-1);
	}
	if( read(fd,shellcode,file_size) == -1){
		print_error("failed read the file\n",NULL);
		exit(-1);
	}else{
		print_info("seccessful read (%d bytes)\n",file_size);
	}
		
	close(fd);
	if(dump_state == 1){
		print_message("the shellcode hexdump\n",NULL);
		hexdump(shellcode,file_size);
	}else if(dump_state == 2){
		print_message("the shellcode textdump\n",NULL);
		textdump(shellcode,file_size);
	}
	if(run_state == 1){
		run_shellcode(shellcode);
	}
	return 1;
}
