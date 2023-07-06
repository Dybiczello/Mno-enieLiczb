#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>
#include <string.h>

#define SYSFS_FILE_A1 "/sys/kernel/sykt/dmda1"
#define SYSFS_FILE_A2 "/sys/kernel/sykt/dmda2"
#define SYSFS_FILE_W  "/sys/kernel/sykt/dmdw"
#define SYSFS_FILE_L  "/sys/kernel/sykt/dmdl"
#define SYSFS_FILE_B  "/sys/kernel/sykt/dmdb"
#define SYSFS_FILE_COUNTER  "/sys/kernel/sykt/counter"
#define uint32_t unsigned long
#define MAX_BUFF 1024 
 
int mnozenie(uint32_t a1, uint32_t a2){ 
	char buffer[MAX_BUFF];
	
	
	FILE *a1_file; 		
	FILE *a2_file; 		
	FILE *w_file; 		
	FILE *b_file; 		
  	FILE *l_file;        
	FILE *counter_file; 		
	
	

    a1_file = fopen(SYSFS_FILE_A1, "w");
	if (a1_file == NULL){					
		printf("Nie udalo sie otworzyc pliku A1");
		return -1;
	}
	snprintf(buffer, sizeof(buffer), "%d", a1);
	fwrite(buffer, 1, sizeof(buffer), a1_file);
    fclose(a1_file);
	
    a2_file = fopen(SYSFS_FILE_A2, "w");
	if (a2_file == NULL){
		printf("Nie udalo sie otworzyc pliku A2");
		return -1;
	}
	snprintf(buffer, sizeof(buffer), "%d", a2);
	fwrite(buffer, 1, sizeof(buffer), a2_file);
    fclose(a2_file);
	
    b_file = fopen(SYSFS_FILE_B, "w");
	if (b_file == NULL){
		printf("Nie udalo sie otworzyc pliku B");
		return -1;
	}
	snprintf(buffer, sizeof(buffer),"%d",15);
	fwrite(buffer, sizeof(buffer), 1, b_file);
    fclose(b_file);
	
	for(;;){
		b_file = fopen(SYSFS_FILE_B, "r");
		if (b_file == NULL){
			printf("Nie udalo sie otworzyc pliku B");
			return -1;
		}
		fread(buffer, sizeof(buffer), 1, b_file);

		if (buffer[0] == '0') break;
    	fclose(b_file);

        sleep(1);
	  }
    
    counter_file = fopen(SYSFS_FILE_COUNTER, "r");
    if (counter_file == NULL){
        printf("Nie udalo sie otworzyc pliku COUNTER");
        return -1;
    }
	
    
	w_file = fopen(SYSFS_FILE_W, "r");
	if (w_file == NULL){
		printf("Nie udalo sie otworzyc pliku W");
		return -1;
	  }

  l_file = fopen(SYSFS_FILE_L, "r");
  if (l_file == NULL){
    printf("Nie udalo sie otworzyc pliku L");
    return -1;
  }

  fseek(w_file, 0, SEEK_SET);
    
  memset(buffer, 0, sizeof(buffer));

  sleep(1);
  fgets(buffer, MAX_BUFF, w_file);
     
    
 	fclose(w_file);
  fclose(l_file);
	
  printf("Wynik mnozenia liczb: %d i %d wynosi: ", a1, a2);
  printf(buffer);
  printf("\n");  
	
	return 0;
} 

int main(void){
	printf("Compiled at %s %s\n", __DATE__, __TIME__);
	mnozenie(15,10);
 	mnozenie(1,0);
 	mnozenie(50,1);
	mnozenie(789,1234);    
 	mnozenie(8,64);
	return 0;
}

