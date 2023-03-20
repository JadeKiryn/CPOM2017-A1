// name: Qixiang Wu
// unikey: qiwu4270
// SID: 510004952

// code here

#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 19
#define HOLE_SIZE  7

#define fin stdin
#define fout stdout

typedef struct COORDINATE {
    char go_style[4];
    int x, y;
} COORDINATE;


typedef struct BOARD {
    int stone_num;
    int stone_state; // 1 for black, 2 for white
    COORDINATE mist;
    int go_board[BOARD_SIZE][BOARD_SIZE]; // 0 for empty, 1 for blace, 2 for white
    COORDINATE history[BOARD_SIZE * BOARD_SIZE];
} BOARD;

int check_win(BOARD *board);

int main () {
    // FILE *fin = fopen("test.in", "r");
    // FILE *fout = fopen("test.out", "w");
    char command_line[20];
    char command[10];
    char go_coordinate[10];

    BOARD board;
    board.stone_num = 0;
    memset(board.go_board, 0, sizeof(board.go_board));
    strcpy(board.mist.go_style, "J10");
    board.mist.x = 9;
    board.mist.y = 9;
    board.stone_state = 1;

    int winner = 0;
    
    while(1) {
        fgets(command_line, 20, fin);
        if(strcmp(command_line, "who\n") == 0) {
            if(board.stone_state == 1) {
                fprintf(fout, "B\n");
            } else {
                fprintf(fout, "W\n");
            }
        } else if(strcmp(command_line, "term\n") == 0) {
            return 1;
        } else if(strcmp(command_line, "resign\n") == 0) {
            if(board.stone_state == 1) {
                winner = 2;
            } else {
                winner = 1;
            }
            break;
        } else if(strcmp(command_line, "view\n") == 0) {
            char view_str[HOLE_SIZE*HOLE_SIZE+1];
            int x;
            int y;
            for(int i = 0; i < HOLE_SIZE * HOLE_SIZE; i++) {
                x = board.mist.x - (HOLE_SIZE - 1) / 2 + i % 7;
                y = board.mist.y + (HOLE_SIZE - 1) / 2 - i / 7;
                if(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                    view_str[i] = 'x';
                } else if(board.go_board[x][y] == 0) {
                    view_str[i] = '.';
                } else if(board.go_board[x][y] == 1) {
                    view_str[i] = '#';
                } else {
                    view_str[i] = 'o';
                }
            }
            view_str[HOLE_SIZE*HOLE_SIZE] = '\0';
            fprintf(fout, "%s,%s\n", board.mist.go_style, view_str);
        } else if(strcmp(command_line, "history\n") == 0) {
            for(int i = 0; i < board.stone_num; i++) {
                fprintf(fout, "%s", board.history[i].go_style);
            }
            fprintf(fout, "\n");
        } else {
            char tmp[10];
            if(sscanf(command_line, "%s %s %s", command, go_coordinate, tmp) != 2) {
                fprintf(fout, "Invalid!\n");
            } else if (strcmp(command, "place") != 0) {
                fprintf(fout, "Invalid!\n");
            } else {
                // Place
                char c;
                int r;
                if(sscanf(go_coordinate, "%c%d", &c, &r) != 2) {
                    fprintf(fout, "Invalid coordinate\n");
                    continue;
                }
                if(c < 'A' || c > 'S' || r < 1 || r > BOARD_SIZE) {
                    fprintf(fout, "Invalid coordinate\n");
                    continue;
                }

                int x = c - 'A';
                int y = r - 1;
                if(board.go_board[x][y] != 0) {
                    fprintf(fout, "Occupied coordinate\n");
                    continue;
                }

                COORDINATE *cur_stone = &(board.history[board.stone_num]);
                strcpy(cur_stone->go_style, go_coordinate);
                cur_stone->x = x;
                cur_stone->y = y;
                board.go_board[x][y] = board.stone_state;
                board.stone_state = board.stone_state % 2 + 1;
                board.stone_num += 1;

                // Change the mist ordinary
                x += 1;
                y += 1;
                int mist_x = 1 + (5 * x * x + 3 * x + 4) % BOARD_SIZE;
                int mist_y = 1 + (4 * y * y + 2 * y - 4) % BOARD_SIZE;
                board.mist.x = mist_x - 1;
                board.mist.y = mist_y - 1;
                char mist_go_coordinate[4];
                mist_go_coordinate[0] = 'A' + mist_x - 1;
                if(mist_y > 9) {
                    mist_go_coordinate[1] = '1';
                    mist_go_coordinate[2] = '0' + mist_y % 10;
                } else {
                    mist_go_coordinate[1] = '0' + mist_y % 10;
                    mist_go_coordinate[2] = '\0';
                }
                mist_go_coordinate[3] = '\0';
                strcpy(board.mist.go_style, mist_go_coordinate);
                
                // Check win, tie
                int game_state = check_win(&board);
                if(game_state != 0) {
                    winner = game_state;
                    break;
                }
                if(board.stone_num == BOARD_SIZE * BOARD_SIZE) {
                    winner = 0;
                    break;
                }
            }
        }
    }

    // Exit
    if(winner == 0) {
        fprintf(fout, "Wow, a tie!\n");
    } else if(winner == 1) {
        fprintf(fout, "Black wins!\n");
    } else {
        fprintf(fout, "White wins!\n");
    }

    for(int i = 0; i < board.stone_num; i++) {
        fprintf(fout, "%s", board.history[i].go_style);
    }
    fprintf(fout, "\nThank you for playing!\n");

    // fclose(fin);
    // fclose(fout);
    return 0;
}

int check_win(BOARD *board) {
    int count = 0;
    int x = board->history[board->stone_num-1].x;
    int y = board->history[board->stone_num-1].y;
    int stone_state = board->stone_state % 2 + 1;

    for(int i = x - 4; i <= x + 4; i++) {
        if(i < 0 || i >= BOARD_SIZE || board->go_board[x][i] != stone_state) {
            count = 0;
        } else {
            count++;
            if(count >= 5) {
                return stone_state;
            }
        }
    }

    count = 0;
    for (int i = y - 4; i <= y + 4; i++) {
        if (i < 0 || i >= BOARD_SIZE || board->go_board[i][x] != stone_state) {
            count = 0;
        } else {
            count++;
            if (count >= 5) {
                return stone_state;
            }
        }
    }

    count = 0;
    for (int i = -4; i <= 4; i++) {
        if (x + i < 0 || x + i >= BOARD_SIZE || y + i < 0 || y + i >= BOARD_SIZE || 
            board->go_board[x+i][y+i] != stone_state) {
            count = 0;
        } else {
            count++;
            if (count >= 5) {
                return stone_state;
            }
        }
    }

    count = 0;
    for (int i = -4; i <= 4; i++) {
        if (x - i < 0 || x - i >= BOARD_SIZE || y + i < 0 || y + i >= BOARD_SIZE || 
            board->go_board[x-i][y+i] != stone_state) {
            count = 0;
        } else {
            count++;
            if (count >= 5) {
                return stone_state;
            }
        }
    }    

    return 0;
}
