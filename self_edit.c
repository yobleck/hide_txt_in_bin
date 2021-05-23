//gcc self_edit.c -lcrypt -o self_edit
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void main(int argc, char *argv[]){
    
    char* cryp = crypt(getpass("input pass here:\n"), "$6$sult"); //$6$=sha512 TODO: $rounds=xxx$
    //printf("%s\n", cryp); //https://man7.org/linux/man-pages/man3/crypt.3.html
    
    if(!strcmp(cryp, "$6$sult$Q0XzRqWLhxukRtFI4X1eG.As0I2EGR8rXro/KiCDaPw1va46zi.Y5nvtPsXLAl9X7TZj5WAs6WOHbB7rtfgl01")){ //pass = pass
        if(argc >= 2){
            if(!strcmp(argv[1],"r")){
                
                FILE *f = fopen("./self_edit", "rb");
                
                //get file size
                fseek(f, 0L, SEEK_END);
                int size_f = ftell(f);
                printf("file size: %d\n", size_f);
                rewind(f);
                
                //put file in char array buffer thing
                char buf[size_f];
                fread(buf, sizeof(char), size_f, f);
                
                //scan file for message header: "i" x8
                int msg_index = 0;
                for(int i = 0; i < size_f; i++){
                    if(buf[i] == 'i'){ //!strcmp(buf[i], 'i')
                        if(buf[i+1] == 'i' && buf[i+2] == 'i' && buf[i+3] == 'i' && buf[i+4] == 'i' &&
                           buf[i+5] == 'i' && buf[i+6] == 'i' && buf[i+7] == 'i'){
                            msg_index = i+7;
                        }
                    }
                }
                //write file contents to stdout as hex
                printf("hidden text:\n");
                for(int i = msg_index; i < size_f; i++){
                    printf("%x ", buf[i]);
                    if(i%16 == 0 && i != 0){
                        printf("\n");
                    }
                }
                
                fclose(f);
            }
            
            else if(!strcmp(argv[1],"w")){
                if(argc == 3){
                    
                    //open file 1 and get file size
                    FILE *f = fopen("./self_edit", "rb");
                    fseek(f, 0L, SEEK_END);
                    int f_size = ftell(f);
                    printf("size of input file: %d\n", f_size);
                    rewind(f);
                    
                    //TODO: convert argv[2] to aes?
                    //create file 2 and set size to file 1 + input arg len
                    //https://stackoverflow.com/questions/7775027/how-to-create-file-of-x-size
                    FILE* f2 = fopen("./output", "wb");
                    int f_size_out = f_size +8+strlen(argv[2]);
                    fseek(f2, f_size_out, SEEK_SET); // does this do anything?
                    //fputc('\0', f2); //terminating null?
                    rewind(f2);
                    printf("size of output file: %d\n", f_size_out);
                    
                    //buffer of chars from f1
                    char buf[f_size_out];
                    fread(buf, sizeof(char), f_size, f);
                    
                    //buffer of chars from argv[2]
                    char input[8+strlen(argv[2])];
                    strcpy(input, "iiii"); //header for text section at end of file
                    strcat(input, "iiii"); //stops "i" x8 from showing up in binary file
                    strcat(input, argv[2]);
                    printf("input: %s\n", input + 8);
                    
                    //add argv[2] to buf
                    for (int i = f_size; i < f_size_out; i++){
                        buf[i] = input[i-f_size];
                    }
                    
                    //write to f2
                    fwrite(buf, sizeof(buf), 1, f2);
                    
                    fclose(f);
                    fclose(f2);
                    printf("remember to manually rename output to desired name\n");
                }
                
                else{
                    printf("no string inputted or too many args\n");
                }
            }
            
            else if(!strcmp(argv[1],"h") || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")){
                printf("HELP:\nr: to read hex data for file\nw: to write data to temp file");
            }
            
            else{
                printf("HELP:\nr: to read hex data for file\nw: to write data to temp file");
            }
        }
        
        else{
            printf("HELP:\nr: to read hex data for file\nw: to write data to temp file");
        }
        
    } //end pass check
    else{
        printf("ERROR: password did not match\nHELP:\nr: to read hex data for file\nw: to write data to temp file");
    }
}
