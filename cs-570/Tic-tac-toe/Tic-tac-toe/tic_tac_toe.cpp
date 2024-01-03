#include "tic_tac_toe.h"

const char tic_tac_toe::SYMBOL_EMPTY = '\0';
const char* tic_tac_toe::PLAYER_SYMBOLS = "XOABCDEFGHIJKLMNPQRSTUVWYZ";
const int tic_tac_toe::PLAYER_NUM_MIN = 2;
const int tic_tac_toe::PLAYER_NUM_MAX = 26;
const int tic_tac_toe::BOARD_MIN = 3;
const int tic_tac_toe::BOARD_MAX = 99;
const int tic_tac_toe::WIN_SEQUENCE_COUNT_MIN = 3;
const char tic_tac_toe::SAVE_FILE_DELIMITER = ',';
const char* tic_tac_toe::SAVE_FILE_PATH = "save.dat";

tic_tac_toe::tic_tac_toe() {}

tic_tac_toe::~tic_tac_toe()
{
	for (int i = 0; i < this->row_column_num; ++i)
	{
		delete[] board[i];
	}

	delete[] board;
}

void tic_tac_toe::start()
{
	char cmd;
	while (true)
	{
		cout << "*******************************************" << endl;
		cout << "*******************************************" << endl;
		cout << "******************       ******************" << endl;
		cout << "****************           ****************" << endl;
		cout << "**************  Tic-Tac-Toe  **************" << endl;
		cout << "****************           ****************" << endl;
		cout << "******************       ******************" << endl;
		cout << "*******************************************" << endl;
		cout << "*******************************************" << endl;
		cout << "New game(N or n), Load game(L or l), Replay game(R or r), Exit(E or e): ";
		cin >> cmd;
		switch (tolower(cmd))
		{
		case 'n':
			new_game();
			break;
		case 'l':
			load_game();
			break;
		case 'r':
			replay_game();
			break;
		case 'e':
			return;
			break;
		default:
			break;
		}
	}

}

void tic_tac_toe::new_game()
{
	while (true)
	{
		input_config_parms();
		if (check_config_parms())
			break;
		cout << "It is impossible to win with " << player_num << " players, " << row_column_num << "x" << row_column_num << " board and " << win_sequence_count << " win sequence count, please input them again." << endl;
	}
	init();
	continue_game();
}

void tic_tac_toe::continue_game()
{
	print_board();
	int row, column;
	time_t start_time, end_time;
	while (true)
	{
		time(&start_time);
		char player = get_current_player();
		cout << "It's player '" << player << "' turn to mark." << endl;
		cout << "Press 'S' or's' to save and exit£¬ or any other keys to continue: ";
		char cmd;
		cin >> cmd;
		if (tolower(cmd) == 's')
		{
			save();
			return;
		}

		input_row_column(row, column);
		mark(player, row - 1, column - 1);
		time(&end_time);
		// add a record
		record rec;
		rec.row = row;
		rec.column = column;
		rec.time_span_seconds = difftime(end_time, start_time);
		records.push_back(rec);

		print_board();

		if (judge(player, row - 1, column - 1))
		{
			cout << "The player '" << player << "' won." << endl;
			return;
		}
		if (is_full())
		{
			cout << "The board is full, No Winner." << endl;
			return;
		}

		// switch player
		switch_player();
	}
}

void tic_tac_toe::load_game()
{
	records.clear();
	if (!load())
	{
		cout << "The save file '" << SAVE_FILE_PATH << "' doesn't exist or its data has been damaged." << endl;
		return;
	}
	init_board();
	clear_board();
	current_player = 0;

	for (vector<record>::iterator iter = records.begin(); iter != records.end(); ++iter)
	{
		if (iter->row >= 1 && iter->row <= row_column_num
			&& iter->column >= 1 && iter->column <= row_column_num
			&& !is_marked(iter->row - 1, iter->column - 1))
		{
			char player = get_current_player();
			mark(player, iter->row - 1, iter->column - 1);
			if (judge(player, iter->row - 1, iter->column - 1))
			{
				cout << "The player '" << player << "' won." << endl;
				return;
			}
			if (is_full())
			{
				cout << "The board is full, No Winner." << endl;
				return;
			}
			switch_player();
		}
	}
	continue_game();
}

void tic_tac_toe::replay_game()
{
	records.clear();
	if (!load())
	{
		cout << "The save file '" << SAVE_FILE_PATH << "' doesn't exist or its data has been damaged." << endl;
		return;
	}
	init_board();
	clear_board();
	current_player = 0;

	print_board();
	for (vector<record>::iterator iter = records.begin(); iter != records.end(); ++iter)
	{

		if (iter->row >= 1 && iter->row <= row_column_num
			&& iter->column >= 1 && iter->column <= row_column_num
			&& !is_marked(iter->row - 1, iter->column - 1))
		{
			char player = get_current_player();
			
			cout << "Waiting " << iter->time_span_seconds << " seconds for player '" << player << "' turn to mark." << endl;
			Sleep(static_cast<DWORD>(iter->time_span_seconds * 1000));


			mark(player, iter->row - 1, iter->column - 1);
			print_board();
			if (judge(player, iter->row - 1, iter->column - 1))
			{
				cout << "The player '" << player << "' won." << endl;
				return;
			}
			if (is_full())
			{
				cout << "The board is full, No Winner." << endl;
				return;
			}
			switch_player();
		}
	}
	//continue_game();
}

void tic_tac_toe::init()
{
	init_board();
	clear_board();
	current_player = 0;
	records.clear();
}

void tic_tac_toe::init_board()
{
	board = new char *[this->row_column_num];
	for (int i = 0; i < this->row_column_num; ++i)
	{
		board[i] = new char[this->row_column_num];
	}
}

void tic_tac_toe::clear_board()
{
	for (int i = 0; i < this->row_column_num; ++i)
	{
		for (int j = 0; j < this->row_column_num; ++j)
		{
			board[i][j] = SYMBOL_EMPTY;
		}
	}
}

bool tic_tac_toe::judge(const char symbol, int row, int column) const
{
	int i, j;
	int count;

	// horizontal scan
	i = row, j = column;
	count = 1;
	while (++j < row_column_num && board[i][j] == symbol)
		++count;
	j = column;
	while (--j >= 0 && board[i][j] == symbol)
		++count;
	if (count >= win_sequence_count)
		return true;

	// vertical scan
	i = row, j = column;
	count = 1;
	while (++i < row_column_num && board[i][j] == symbol)
		++count;
	i = row;
	while (--i >= 0 && board[i][j] == symbol)
		++count;
	if (count >= win_sequence_count)
		return true;

	// diagonal scan
	i = row, j = column;
	count = 1;
	while (--i >= 0 && --j >= 0 && board[i][j] == symbol)
		++count;
	i = row, j = column;
	while (++i < row_column_num && ++j < row_column_num && board[i][j] == symbol)
		++count;
	if (count >= win_sequence_count)
		return true;

	i = row, j = column;
	count = 1;
	while (++i < row_column_num && --j >= 0 && board[i][j] == symbol)
		++count;
	i = row, j = column;
	while (--i >= 0 && ++j < row_column_num && board[i][j] == symbol)
		++count;
	if (count >= win_sequence_count)
		return true;

	// extra credit: allow winning by a square whose side dimensions are n - 1, where n is the win sequence count. 
	if ((win_sequence_count - 1) * (win_sequence_count - 1) * player_num > row_column_num * row_column_num)
		return false;

	int row_start, row_end, column_start, column_end;
	row_start = row_end = row;
	column_start = column_end = column;

	// horizontal scan
	i = row, j = column;
	while (++j < row_column_num && board[i][j] == symbol)
		++column_end;
	j = column;
	while (--j >= 0 && board[i][j] == symbol)
		--column_start;

	// vertical scan
	i = row, j = column;
	while (++i < row_column_num && board[i][j] == symbol)
		++row_end;
	i = row;
	while (--i >= 0 && board[i][j] == symbol)
		--row_start;

	if (column_end - column_start + 1 == win_sequence_count - 1
		&& row_end - row_start + 1 == win_sequence_count - 1)
	{
		for (int r = row_start; r <= row_end; ++r)
		{
			for (int l = column_start; l <= column_end; ++l)
			{
				if (board[r][l] != symbol)
					return false;
			}
		}
		return true;
	}

	return false;
}

void tic_tac_toe::mark(const char symbol, int row, int column)
{
	board[row][column] = symbol;
}

bool tic_tac_toe::is_marked(int row, int column) const
{
	return board[row][column] != SYMBOL_EMPTY;
}

bool tic_tac_toe::is_full() const
{
	return records.size() >= static_cast<vector<record>::size_type>(row_column_num * row_column_num);
}

void tic_tac_toe::switch_player()
{
	current_player = ++current_player % player_num;
}

char tic_tac_toe::get_current_player() const
{
	return PLAYER_SYMBOLS[current_player];
}

void tic_tac_toe::print_board() const
{
	cout << ' ';
	for (int i = 0; i < row_column_num; ++i)
	{
		cout << setw(3) << i + 1;
		cout << ' ';
	}
	cout << endl;

	for (int i = 0; i < row_column_num; ++i)
	{
		cout <<setw(2) << i + 1;
		for (int j = 0; j < row_column_num; ++j)
		{
			if (board[i][j] == SYMBOL_EMPTY)
				cout << ' ' << ' ' << ' ';
			else
				cout << ' ' << board[i][j] << ' ';

			if (j < row_column_num - 1)
				cout << '|';
		}
		cout << endl;

		if (i < row_column_num - 1)
		{
			cout << ' ' << ' ';
			for (int m = 0; m < row_column_num; ++m)
			{
				cout << '-' << '-' << '-';
				if (m < row_column_num - 1)
					cout << '+';
			}
			cout << endl;
		}
	}
}

void tic_tac_toe::input_row_column(int &row, int &column) const
{
	// cout << "Please input a row and column number." << endl;
	while (true)
	{
		while (true)
		{
			cout << "Row(" << 1 << "-" << row_column_num << "): ";
			cin >> row;
			if (!cin.fail() && row >= 1 && row <= row_column_num)
				break;
			else
			{
				if (cin.fail())
				{
					cin.clear();
					cin.sync();
				}
				cout << "The range of row number must be between " << 1 << " and " << row_column_num << endl;
			}
		}
		while (true)
		{
			cout << "Column(" << 1 << "-" << row_column_num << "): ";
			cin >> column;
			if (!cin.fail() && column >= 1 && column <= row_column_num)
				break;
			else
			{
				if (cin.fail())
				{
					cin.clear();
					cin.sync();
				}
				cout << "The range of column number must be between " << 1 << " and " << row_column_num << endl;
			}
		}

		if (is_marked(row - 1, column - 1))
			cout << "The space " << row << " x " << column << " has been marked, please choose another one." << endl;
		else
			break;
	}
}

void tic_tac_toe::input_config_parms()
{
	// input number of players
	while (true)
	{
		cout << "How many players are playing: ";
		cin >> player_num;
		if (!cin.fail() && player_num >= PLAYER_NUM_MIN && player_num <= PLAYER_NUM_MAX)
			break;
		else
		{
			if (cin.fail())
			{
				cin.clear();
				cin.sync();
			}
			cout << "The range of players number must be between " << PLAYER_NUM_MIN << " and " << PLAYER_NUM_MAX << endl;
		}
	}
	// input size of board
	while (true)
	{
		cout << "How large the board should be: ";
		cin >> row_column_num;
		if (!cin.fail() && row_column_num >= BOARD_MIN && row_column_num <= BOARD_MAX)
			break;
		else
		{
			if (cin.fail())
			{
				cin.clear();
				cin.sync();
			}
			cout << "The size of board must be between " << BOARD_MIN << " and " << BOARD_MAX << endl;
		}
	}
	// input win count
	while (true)
	{
		cout << "What the win sequence count should be: ";
		cin >> win_sequence_count;
		if (!cin.fail() && win_sequence_count >= WIN_SEQUENCE_COUNT_MIN && win_sequence_count <= row_column_num)
			break;
		else
		{
			if (cin.fail())
			{
				cin.clear();
				cin.sync();
			}
			cout << "The range of win sequence count  must be between " << WIN_SEQUENCE_COUNT_MIN << " and " << row_column_num << endl;
		}
	}
}

bool tic_tac_toe::check_config_parms()
{
	if (player_num * win_sequence_count > row_column_num * row_column_num)
		return false;

	return true;
}

bool tic_tac_toe::save()
{
	try
	{
		ofstream save_file(SAVE_FILE_PATH);
		save_file << player_num << SAVE_FILE_DELIMITER
			<< row_column_num << SAVE_FILE_DELIMITER
			<< win_sequence_count << endl;

		for (vector<record>::iterator iter = records.begin(); iter != records.end(); ++iter)
		{
			save_file << iter->row << SAVE_FILE_DELIMITER
				<< iter->column << SAVE_FILE_DELIMITER
				<< iter->time_span_seconds << endl;
		}
	}
	catch (exception ex)
	{
		cerr << "Save file '" << SAVE_FILE_PATH << "' failure." << endl;
		return false;
	}
	return true;
}

bool tic_tac_toe::load()
{
	try
	{
		ifstream save_file(SAVE_FILE_PATH);
		string line;

		if (!getline(save_file, line))
			return false;

		string config_parms[3];
		split_string(line, SAVE_FILE_DELIMITER, config_parms, 3);
		player_num = stoi(config_parms[0]);
		row_column_num = stoi(config_parms[1]);
		win_sequence_count = stoi(config_parms[2]);
		
		if (player_num < PLAYER_NUM_MIN || player_num > PLAYER_NUM_MAX)
			return false;

		if (row_column_num < BOARD_MIN || row_column_num > BOARD_MAX)
			return false;

		if (win_sequence_count < WIN_SEQUENCE_COUNT_MIN || win_sequence_count > row_column_num)
			return false;

		if (!check_config_parms())
			return false;

		string record_items[3];
		while (getline(save_file, line))
		{
			split_string(line, SAVE_FILE_DELIMITER, record_items, 3);

			record rec;
			rec.row = stoi(record_items[0]);
			rec.column = stoi(record_items[1]);
			char **endp = 0;
			rec.time_span_seconds = stod(record_items[2]);

			records.push_back(rec);
		}

	}
	catch (exception ex)
	{
		return false;
	}
	return true;
}

void tic_tac_toe::split_string(string str, char delimiter, string results[], int length)
{
	string::size_type b_index, e_index;
	b_index = str.find_first_not_of(delimiter);
	for (int i = 0; i < length; ++i)
	{
		if (b_index == string::npos)
			throw exception();

		e_index = str.find(delimiter, b_index);
		if (e_index == string::npos)
			e_index = str.length();

		results[i] = str.substr(b_index, e_index - b_index);

		b_index = str.find_first_not_of(delimiter, e_index);
	}
	
}
