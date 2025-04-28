#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 1024
#define DELIMITER " \t\r\n\a" // các ký tự phân cách giữa các từ trong command
#define MAX_NUM_WORDS 100     // số từ tối đa trong 1 command


char *read_command()
{
    int numOfChar = BUFFER_SIZE;                     // số ký tự tối đa của 1 command
    int position = 0;                                // index dùng cho các ký tự
    char *buffer = malloc(sizeof(char) * numOfChar); // xin cấp phát bộ nhớ
    int c;                                           // biến dùng để đọc từng ký tự từ stdin

    if (buffer == NULL)
    { // kiểm tra xem có cấp phát thành công không
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    // bắt đầu đọc command từ stdin, đọc từng ký tự
    // nếu ký tự đọc đc là end of file hoặc new line thì dừng lại
    // ngược lại thì tiếp tục đọc
    while (1)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
            ++position;
        }
        // trong lúc lấy command mà position vượt quá số ký tự tối đa
        // thì cấp phát thêm bộ nhớ cho buffer
        if (position >= numOfChar)
        {
            numOfChar += BUFFER_SIZE;
            buffer = realloc(buffer, numOfChar * sizeof(char)); // cấp phát lại bộ nhớ
            if (buffer == NULL)
            {
                fprintf(stderr, "Unable to allocate memory\n");
                //return buffer;
                exit(1);
                
            }
        }
    }
}

char **split_command(char *command)
{
    int numOfWords = MAX_NUM_WORDS;                     // số từ tối đa trong 1 command
    int position = 0;                                   // index để duyệt các TỪ trong command
    char **words = malloc(numOfWords * sizeof(char *)); // cấp phát bộ nhớ cho mảng các từ
    char *word;                                         // pointer point to single word in command

    if (words == NULL)
    { // kiểm tra xem cấp phát bộ nhớ cho mảng các từ có thành công không
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    word = strtok(command, DELIMITER); // Từ đầu tiêntiên
    while (word != NULL)
    { // nếu từ không phải NULL thì tiếp tục xử lý
        // cấp phát bộ nhớ cho từ và copy từ vào mảng các từ
        words[position] = word;
        ++position;

        if (position >= numOfWords)
        {
            numOfWords += MAX_NUM_WORDS;
            words = realloc(words, sizeof(char *) * numOfWords);
            if (words == NULL)
            {
                fprintf(stderr, "Unable to allocate memory\n");
                exit(1);
            }
        }
        word = strtok(NULL, DELIMITER); // lấy từ tiếp theo trong command
    }
    words[position] = NULL;
    return words;
}

int handle_builtIn_command(char **words)
{
    if (words[0] == NULL) // no commandcommand
    {
        return 1;
    }

    if (strcmp(words[0], "exit") == 0)
    { // nếu từ đầu tiên là exit thì thoát
        return 0;
    }

    if (strcmp(words[0], "lastboot") == 0) {
        int result = system("lastboot.bat");
        if (result != 0) {
            perror("Error executing lastboot.bat");
        }
        return 1;
    }

    if (strcmp(words[0], "cd") == 0) // command to open a file ( a dỉactory)
    {
        if (words[1] == NULL)
        {
            fprintf(stderr, "missing directory\n");
        }
        else
        {
            if (!SetCurrentDirectoryA(words[1]))
            {
                fprintf(stderr, "Can note change directory to %s\n", words[1]);
            }
        }
        return 1;
    }

    if (strcmp(words[0], "greet") == 0)
    {
        if (words[1] != NULL)
        {
            printf("Xin chào, %s! Chúc bạn code vui vẻ!\n", words[1]);
        }
        else
        {
            printf("Xin chào bạn coder giấu tên!\n");
        }
        return 1;
    }

    // Xóa màn hình
    if (strcmp(words[0], "cls") == 0)
    {
        system("cls"); // hàm gọi lệnh hệ thống từ chương trình C
        return 1;
    }

    // echo: printprint text
    if (strcmp(words[0], "echo") == 0)
    {
        for (int i = 1; words[i] != NULL; i++)
        {
            printf("%s ", words[i]);
        }
        printf("\n");
        return 1;
    }

    // copy file
    if (strcmp(words[0], "copy") == 0)
    {
        if (words[1] == NULL || words[2] == NULL)
        {
            fprintf(stderr, "copy: need source andand destination\n");
        }
        else
        {
            char cmd[512];
            sprintf(cmd, "copy \"%s\" \"%s\"", words[1], words[2]);
            system(cmd);
        }
        return 1;
    }

    // move file to ....
    if (strcmp(words[0], "move") == 0)
    {
        if (words[1] == NULL || words[2] == NULL)
        {
            fprintf(stderr, "move: cần source và destination\n");
        }
        else
        {
            char cmd[512];
            sprintf(cmd, "move \"%s\" \"%s\"", words[1], words[2]);
            system(cmd);
        }
        return 1;
    }

    // delete file
    if (strcmp(words[0], "del") == 0)
    {
        if (words[1] == NULL)
        {
            fprintf(stderr, "del: cần tên file\n");
        }
        else
        {
            char cmd[512];
            sprintf(cmd, "del \"%s\"", words[1]);
            system(cmd);
        }
        return 1;
    }

    // list files in the current folder
    if (strcmp(words[0], "dir") == 0)
    {
        system("dir");
        return 1;
    }

    // remove directory
    if (strcmp(words[0], "rd") == 0)
    {
        if (words[1] == NULL)
        {
            fprintf(stderr, "rd: need a diractory\n");
        }
        else
        {
            char cmd[512];
            sprintf(cmd, "rd \"%s\"", words[1]);
            system(cmd);
        }
        return 1;
    }

    if (strcmp(words[0], "help") == 0)
    {
        printf("Bulit-in commands:\n");
        printf(" exit: exit the shell\n");
        printf(" cd: change directory\n");
        printf(" help: show this help\n");
        printf(" echo: show the command\n");
        printf(" cls: clear the screen\n");
        printf(" copy: copy file to a new destination\n");
        printf(" move: move file to a new destination\n");
        printf(" del: delete files\n");
        printf(" dir: list files in the current directory\n");
        printf(" rd: remove directory\n");
        printf("lastboot: show last boot time\n");
        return 1;
    }
    return 1;
}

int execute_command(char **words) {
    if (words[0] == NULL) {
        return 1; // Không có lệnh gì thì tiếp tục shell
    }

    // Kiểm tra built-in
    int built_in_result = handle_builtIn_command(words);
    if (built_in_result == 0 || built_in_result == 1) {
        return built_in_result; // 0: thoát, 1: tiếp tục
    }

    // Nếu không phải built-in thì chạy command ngoài
    // Ghép lại thành 1 command line
    char command_line[1024] = "";
    for (int i = 0; words[i] != NULL; i++) {
        strcat(command_line, words[i]);
        strcat(command_line, " ");
    }

    // Tạo tiến trình con để chạy command
    STARTUPINFOA si;  // tiến trình mới dùng với CreateProcessA
    PROCESS_INFORMATION pi; // lưu giữ thông tin của tiến trình mới và thread mới 
    // 2 cái trên đi cùng nhau để tạo ra một tiến trình concon
    ZeroMemory(&si, sizeof(si)); // khởi tạo lại tiến trình si, dọn sạch bộ nhớ cấp phát cho sisi   
    si.cb = sizeof(si); // kích thước của cấu trúc, cái này là bắt buộc để hệ điều hành còn biết nên cấp phát bộ nhớ như nàonào
    ZeroMemory(&pi, sizeof(pi)); // khởi tạo lại tiến trình pi, dọn sạch bộ nhớ cấp phát cho pi

    // CreateProcessA cần chuỗi có thể chỉnh sửa, nên phải copy
    char command_copy[512];
    strcpy(command_copy, command_line);

    // Tạo tiến trình
    if (!CreateProcessA(
            NULL,             // ứng dụng
            command_copy,     // dòng lệnh
            NULL,             // không kế thừa security
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi)) {
        fprintf(stderr, "Không thể thực thi lệnh: %s\n", words[0]);
        return 1;
    }

    // Đợi tiến trình con kết thúc
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1; // Tiếp tục shell
}

void main_loop(void)
{
    char *command;
    char **word_in_command;
    int condition_loop = 1;

    do
    {
        printf("chauShell> ");
        command = read_command();
        word_in_command = split_command(command);
        condition_loop = execute_command(word_in_command);
       
        free(command);
        free(word_in_command);
    } while (condition_loop);
}

int main()
{
    main_loop();
    return 0;
}
