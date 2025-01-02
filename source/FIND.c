#include "functions.h"
#include "file_util.h"
#define MAX_FILE_LINE 1024
#define MAX_LINE_LEN 1024

struct LINE_INFO{
    char *line;
    int line_num;
    int if_in;
};

void display_help() {
    print("FIND [/V] [/C] [/N] [/I]  -[string] +[file path]\n\n  /V         显示所有未包含指定字符串的行。\n  /C         仅显示包含字符串的行数。\n  /N         显示行号。\n  /I         搜索字符串时忽略大小写。\n\n  -[string] 指定要搜索的文本字符串。\n  +[file path] 文件路径\n\n");
}

int main(int argc, char *argv[]) {
    if(argc < 2 ){
        return 0;
    }
    if (argv[1][0] == '/' && argv[1][1] == '?')
    {
        display_help();
        return 0;
    }
    int v_option = 0;
    int c_option = 0;
    int n_option = 0;
    int i_option = 0;

    char *pattern = NULL;
    char *file_path = NULL;

    // print(int_to_char(argc));

    for (int i = 1; i < argc ;i++){

        if (argv[i] == NULL) {
            continue;
        }
        char *tmp = argv[i];
        // print(tmp);
        // print("\n");
        if(argv[i][0] == '/'){
            if(argv[i][1] == 'V'||argv[i][1] == 'v')
                v_option = 1;
            if(argv[i][1] == 'C'||argv[i][1] == 'c')
                c_option = 1;
            if(argv[i][1] == 'N'||argv[i][1] == 'n')
                n_option = 1;
            if(argv[i][1] == 'I'||argv[i][1] == 'i')
                i_option = 1;
        }else if(argv[i][0] == '-' && argv[i][1] != '\0'){
            pattern = argv[i]+1;
        }else if(argv[i][0] == '+' && argv[i][1] != '\0'){
            file_path = argv[i]+1;
        }
    }

    if(pattern == NULL || file_path == NULL){
        print("invalid input\n");
        return 0;
    }


    print("---------------");
    print(file_path);


    FILE *file = fopen(file_path, "r");
    struct LINE_INFO* result = search(file, pattern, i_option);

    if(v_option){
                if(c_option){
            int count = 0;
            for(int i = 0; result[i].line != NULL; i++){
                if(!result[i].if_in){
                    count++;
                }
            }
            print(": ");
            print(int_to_char(count));
        }else{
            print("\n");
            for(int i = 0; result[i].line != NULL; i++){
                if(!result[i].if_in){
                    if(n_option){
                        print("[");
                        print(int_to_char(result[i].line_num));
                        print("]");
                        print(" ");
                    }
                    print(result[i].line);
                }
            }
        }
    }else{
        if(c_option){
            int count = 0;
            for(int i = 0; result[i].line != NULL; i++){
                if(result[i].if_in){
                    count++;
                }
            }
            print(": ");
            print(int_to_char(count));
        }else{
            print("\n");
            for(int i = 0; result[i].line != NULL; i++){
                if(result[i].if_in){
                    if(n_option){
                        print("[");
                        print(int_to_char(result[i].line_num));
                        print("]");
                        print(" ");
                    }
                    print(result[i].line);
                }
            }
        }
    }
    print("\n");
    fclose(file);

}