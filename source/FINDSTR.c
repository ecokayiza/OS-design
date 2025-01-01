#include "functions.h"
#include <stdlib.h>

struct LINE_INFO{
    char *line;
    int line_num;
    int if_in;
};

void display_help() {
    print("1-在FIND的基础上添加了多匹配模式与多文件支持\n   如 FINDSTR -pattern1 -pattern2 +path1 +path2\n2-添加了对当前目录的支持\n  如 FINDSTR -pattern +filename\n");
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


    const char *patterns[argc - 1];
    int pattern_count = 0;
    char *file_paths[argc - 1];
    int file_count = 0;

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
            patterns[pattern_count] = argv[i]+1;
            pattern_count++;
        }else if(argv[i][0] == '+' && argv[i][1] != '\0'){
            file_paths[file_count] = argv[i]+1;
            file_count++;
        }
    }

    if(pattern_count == 0 || file_count == 0){
        print("invalid input\n");
        return 0;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        // print("Current working dir: ");
        // print(cwd);
        // print("\n");
        perror("getcwd() error");
        return 1;
    } 
    // for (int i = 0; i < pattern_count; i++)
    // {
    //     print(patterns[i]);
    //     print("\n");
    // }
    for (int i = 0; i < file_count; i++)
    {
        if(!contains(file_paths[i],"/",0)){
            char *full_path = malloc(1024);
            int l = 0;
            for (l; cwd[l];l++)
                full_path[l] = cwd[l];
            full_path[l] = '/';
            int m = 0;
            for (m; file_paths[i][m]; m++)
                full_path[l+m+1] = file_paths[i][m];
            full_path[l+m+1] = '\0';

            file_paths[i] = full_path;
        }
    }
    // for (int i = 0; i < file_count; i++){
    //     print(file_paths[i]);
    //     print("\n");
    // }

    struct LINE_INFO *result;
    struct LINE_INFO **results = (struct LINE_INFO **)malloc(file_count * MAX_FILE_LINE * sizeof(struct LINE_INFO *));

    for(int i = 0; i < file_count; i++){
        FILE *file = fopen(file_paths[i], "r");
        result =multi_search(file, patterns, pattern_count, i_option);
        fclose(file);
        results[i] = result;
    }
    
    // print(file_path);
    for(int i = 0; i < file_count; i++){
        result = results[i];
        print("---------------");
        print(file_paths[i]);

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
                print("\n");
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
                print("\n");
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

    }


    print("\n");



}

