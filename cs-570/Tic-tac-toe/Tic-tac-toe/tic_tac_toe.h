#ifndef _TIC_TAC_TOE_H_
#define _TIC_TAC_TOE_H_

#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <time.h>
#include <windows.h>

using namespace std;

typedef struct
{
	int row;
	int column;
	double time_span_seconds;
} record;

class tic_tac_toe
{
public:
	tic_tac_toe();
	~tic_tac_toe(); 

	void start();
	
private:
	void init();
	void init_board();
	void clear_board();
	bool judge(const char symbol, int row, int column) const;
	void mark(const char symbol, int row, int column);
	bool is_marked(int row, int column) const;
	bool is_full() const;
	void switch_player();
	char get_current_player() const;
	void print_board() const;
	void input_config_parms();
	bool check_config_parms();
	void input_row_column(int &row, int &column) const;

	void new_game();
	void continue_game();
	void load_game();
	void replay_game();
	bool save();
	bool load();
	void split_string(string str, char delimiter, string results[], int length);
	long long get_current_time();

	int player_num;
	int row_column_num;
	int win_sequence_count;
	char **board;
	int current_player;
	vector<record> records;

	
	static const char SYMBOL_EMPTY;
	static const char *PLAYER_SYMBOLS;
	static const int PLAYER_NUM_MIN;
	static const int PLAYER_NUM_MAX;
	static const int BOARD_MIN;
	static const int BOARD_MAX;
	static const int WIN_SEQUENCE_COUNT_MIN;
	static const char SAVE_FILE_DELIMITER;
	static const char *SAVE_FILE_PATH;
};


#endif