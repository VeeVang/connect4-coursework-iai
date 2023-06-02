#include <iostream>
#include <climits>
#include "Point.h"
#include "Strategy.h"
#include "Judge.h"

using namespace std;

bool debug = false;
int connect_3_score = 10000;
// int connect_3_score_vertical = 3000;
int connect_2_score = 100;
int connect_3_score_increment = connect_3_score;
int piece_score = 5;

// 进一步，可以考虑周围的位置是不是可以立刻下下去的，例如对于纵向的情况，就可以给他增加一个倍率。


/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:其实这里给出的top已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
// extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY){
extern "C" Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY){
	/*
		不要更改这段代码
	*/
	int x = -1, y = -1;//最终将你的落子点存到x,y中
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	/*
		根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
		该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
	*/
	//Add your own code below

	/*--------------------参量--------------------------*/
	// 偶数？
	int depth = 6;
	
	int* modifiable_top = new int[N];
	for (int j = 0; j < N; j++) {
		modifiable_top[j] = top[j];
	}
	board[noX][noY] = 3;

	// 对方的棋子用	1 标示，自己的程序的棋子用 2 标示。
	// 不可落子点被置为3

	// 构造第一波尝试，然后找到对应的方向即可！
	int this_x, this_y;
	int maxEval = INT_MIN;
	int alpha = INT_MIN;
	// 遍历所有可能的动作
	for (int j = 0; j < N; j++) {
		int flag;
		// cerr << "isSpace of column j: " <<  isSpace(j, board, top, noX, noY) << endl;
		if (flag = isSpace(j, board, top, noX, noY)) {
			this_y = j;
			this_x = top[j] - flag;
		}
		else {
			continue;
		}

		// 更新落子和top
		board[this_x][this_y] = 2;
		modifiable_top[j] -= flag;

		// 递归调用Alpha-Beta算法，搜索下一层的节点
		int eval = alphaBeta(board, depth - 1, alpha, INT_MAX, false, M, N, this_x, this_y, modifiable_top, noX, noY);
		if (debug)
			cerr << j << "th column eval = " << eval << endl;
		
		// 恢复落子和top
		board[this_x][this_y] = 0;
		modifiable_top[j] += flag;

		// 更新alpha值
		alpha = max(alpha, eval);

		// 更新最大评估值
		maxEval = max(maxEval, eval);
		// if (debug)
		// 	cerr << "maxEval = " << maxEval << endl;
		if (maxEval == eval) {
			x = this_x;
			y = this_y;
		}
	}

	// 落子点一定为 x = _top[y] - 1
	// 当_top[y]为0时，不可以落子


	delete[] modifiable_top;

	// --- User code ends ---

	/*
		不要更改这段代码
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
*/
// extern "C" __declspec(dllexport) void clearPoint(Point* p){
extern "C" void clearPoint(Point* p){
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	添加你自己的辅助函数，你可以声明自己的类、函数，添加新的.h .cpp文件来辅助实现你的想法
*/

bool EmptyOrSelf(int* const* board, int M, int N, int i, int j, int self) {
	// out
	if (i < 0 || i >= M || j < 0 || j >= N ) {
		return false;
	}
	// in and empty
	else if (board[i][j] == 0 || board[i][j] == self) {
		return true;
	}
	else {
		return false;
	}
}

bool Empty(int* const* board, int M, int N, int i, int j, int self) {
	// out
	if (i < 0 || i >= M || j < 0 || j >= N) {
		return false;
	}
	// in and empty
	else if (board[i][j] == 0) {
		return true;
	}
	// in and not empty
	else{
		return false;
	}
}

// 棋盘价值=己方目前的价值-对方目前棋局的价值。
int evaluateBoard(int* const * board, int M, int N, int self, int opponent, const int* top) {
	return evaluateBoardFromSelf(board, M, N, self, opponent, top) - evaluateBoardFromSelf(board, M, N, opponent, self, top);
}

void printBoardAndValue(int* const * board, int M, int N, int value_of_2, int value){
	if (debug){
		cerr << "evaluating board value!..." << endl;
		cerr << "board: " << endl;
		for(int i=0; i<M; i++) {
			for(int j=0; j<N; j++) //每行有m列
				cerr<<board[i][j]<<" "; //输出第i行第j列的数
			cerr<<endl; //每输出完一行，就输出一个换行符
		}
		cerr << "value of player 2 only: " << value_of_2 << endl;
		cerr << "comprehensive value: " << value << endl;
	}
}

void printBoard(int* const * board, int M, int N){
	if (debug){
		cerr << "board: " << endl;
		for(int i=0; i<M; i++) {
			for(int j=0; j<N; j++) //每行有m列
				cerr<<board[i][j]<<" "; //输出第i行第j列的数
			cerr<<endl; //每输出完一行，就输出一个换行符
		}
	}
}

void printArray(int* arr, int N){
	if (debug){
		cerr << "top: " << endl;
		for (int j = 0; j < N; j++)
			cerr << arr[j] << " ";
		cerr << endl;
	}
}

// 棋盘估值
// 再补一个abs(top[j - 1] - (i - 1)) * connect_2_score 或者top[j + 3]下一个子就能落，那么就给更高的分数。
int evaluateBoardFromSelf(int* const* board, int M, int N, int self, int opponent, const int* top) {
	int value = 0;
	// 横向扫描
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			// 每一个棋子
			if (board[i][j] == self) {
				if (EmptyOrSelf(board, M, N, i, j - 1, self)) {
					value += piece_score;
				}
				if (EmptyOrSelf(board, M, N, i, j + 1, self)) {
					value += piece_score;
				}
			}
		}
		for (int j = 0; j < N - 1; j++) {
			// 如果二连且至少有一端可以再落子或为自己的子
			if (board[i][j] == self && board[i][j + 1] == self) {
				if (EmptyOrSelf(board, M, N, i, j - 1, self)) {
					value += connect_2_score;
				}
				if (EmptyOrSelf(board, M, N, i, j + 2, self)) {
					value += connect_2_score;
				}
			}
		}
		for (int j = 0; j < N - 2; j++) {
			// 如果三连且至少有一端可以再落子
			if (board[i][j] == self && board[i][j + 1] == self && board[i][j + 2] == self) {
				if (EmptyOrSelf(board, M, N, i, j - 1, self)) {
					value += connect_3_score;
					if (Empty(board, M, N, i, j - 1, self)) {
						value += connect_2_score * (N - (top[j - 1] - i));
					}
				}
				if (EmptyOrSelf(board, M, N, i, j + 3, self)){
					value += connect_3_score;
					if (Empty(board, M, N, i, j + 3, self)) {
						value += connect_2_score * (N - (top[j + 3] - i));
					}
				}
			}
		}
	}
	// 纵向扫描
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < M; i++) {
			// 如果二连且至少有一端可以再落子或为自己的子
			if (board[i][j] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j, self)) {
					value += piece_score;
				}
				if (EmptyOrSelf(board, M, N, i + 1, j, self)){
					value += piece_score;
				}
			}
		}
		for (int i = 0; i < M - 1; i++) {
			// 如果二连且至少有一端可以再落子或为自己的子
			if (board[i][j] == self && board[i + 1][j] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j, self)) {
					value += connect_2_score;
				}
				if (EmptyOrSelf(board, M, N, i + 2, j, self)){
					value += connect_2_score;
				}
			}
		}
		for (int i = 0; i < M - 2; i++) {
			if (board[i][j] == self && board[i + 1][j] == self && board[i + 2][j] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j, self)) {
					value += connect_3_score;
					if (Empty(board, M, N, i - 1, j, self)) {
						value += connect_2_score * (N - (top[j] - (i - 1)));
					}
				}
				if (EmptyOrSelf(board, M, N, i + 3, j, self)){
					value += connect_3_score;
					if (Empty(board, M, N, i + 3, j, self)) {
						value += connect_2_score * (N - (top[j] - (i + 3)));
					}
				}
			}
		}
	}
	// 左斜向扫描
	for (int i = 0; i < M - 2; i++) {
		for (int j = 0; j < N - 2; j++) {
			if (board[i][j] == self && board[i + 1][j + 1] == self && board[i + 2][j + 2] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j - 1, self)) {
					value += connect_3_score;
					if (Empty(board, M, N, i - 1, j - 1, self)) {
						value += connect_2_score * (N - (top[j - 1] - (i - 1)));
					}
				}
				if (EmptyOrSelf(board, M, N, i + 3, j + 3, self)){
					value += connect_3_score;
					if (Empty(board, M, N, i + 3, j + 3, self)) {
						value += connect_2_score * (N - (top[j + 3] - (i + 3)));
					}
				}
			}
		}
	}
	for (int i = 0; i < M - 1; i++) {
		for (int j = 0; j < N - 1; j++) {
			if (board[i][j] == self && board[i + 1][j + 1] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j - 1, self)) {
					value += connect_2_score;
				}
				if (EmptyOrSelf(board, M, N, i + 2, j + 2, self)){
					value += connect_2_score;
				}
			}
		}
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j - 1, self)) {
					value += piece_score;
				}
				if (EmptyOrSelf(board, M, N, i + 1, j + 1, self)){
					value += piece_score;
				}
			}
		}
	}
	// 右斜向扫描
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j + 1, self)) {
					value += piece_score;
				}
				if (EmptyOrSelf(board, M, N, i + 1, j - 1, self)){
					value += piece_score;
				}
			}
		}
	}
	for (int i = 0; i < M - 1; i++) {
		for (int j = 1; j < N; j++) {
			if (board[i][j] == self && board[i + 1][j - 1] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j + 1, self)) {
					value += connect_2_score;
				}
				if (EmptyOrSelf(board, M, N, i + 2, j - 2, self)){
					value += connect_2_score;
				}
			}
		}
	}
	for (int i = 0; i < M - 2; i++) {
		for (int j = 2; j < N; j++) {
			if (board[i][j] == self && board[i + 1][j - 1] == self && board[i + 2][j - 2] == self) {
				if (EmptyOrSelf(board, M, N, i - 1, j + 1, self)) {
					value += connect_3_score;
					if (Empty(board, M, N, i - 1, j + 1, self)) {
						value += connect_2_score * (N - (top[j + 1] - (i - 1)));
					}
				}
				if (EmptyOrSelf(board, M, N, i + 3, j - 3, self)) {
					value += connect_3_score;
					if (Empty(board, M, N, i + 3, j - 3, self)) {
						value += connect_2_score * (N - (top[j - 3] - (i + 3)));
					}
				}
			}
		}
	}
	return value;
};

int isSpace(int j, int* const* board, const int* top, int noX, int noY) {
	// 当top表明占满了时
	if (top[j] == 0) {
		return 0;
	}
	// 当top表明没占满
	// 且当上一格子就有空时
	else if (top[j] - 1 != noX || j != noY){
		return 1;
	}
	// 当top表明没占满
	// 且当上一格子为noX, noY时
	// else if (top[j] - 1 == noX && j == noY) {
	else{
		if (top[j] > 1) {
			return 2;
		}
		else {
			return 0;
		}
	}
}

// 这个算法是可以在每次更新祖先时，确保对比每次祖先的。
// 记得每次维护board和top
// 记得is tie
int alphaBeta(int** board, int depth, int alpha, int beta, bool maximizing, 
	int M, int N, 
	int last_x, int last_y, int* top, int noX, int noY) {
	// int * const* const_board = board;
	// const int * const_top = top;
	 
	int this_x;
	int this_y;
	// 如果是最大化的回合
	// machine decide
	// 2
	if (maximizing) {
		int maxEval = INT_MIN;
		if (userWin(last_x, last_y, M, N, board)) {
			// printBoardAndValue(board, M, N, -1, INT_MIN);
			return INT_MIN;
		}
		else if (isTie(N, top)) {
			// printBoardAndValue(board, M, N, -1, 0);
			return 0;
		}
		else if (depth == 0) {
			// printBoardAndValue(board, M, N, evaluateBoardFromSelf(board, M, N, 2, 1), evaluateBoard(board, M, N, 2, 1));
			return evaluateBoard(board, M, N, 2, 1, top);
		}
		// 遍历所有可能的动作
		// if (debug)
		// 	cerr << "machine (2) deciding (maximizing) ..." << endl;
		for (int j = 0; j < N; j++) {
			// printBoard(board, M, N);
			// printArray(top, N);
			int flag;
			if (flag = isSpace(j, board, top, noX, noY)) {
				this_y = j;
				this_x = top[j] - flag;
			}
			else {
				continue;
			}

			// 更新落子和top
			board[this_x][this_y] = 2;
			top[j] -= flag;

			// 递归调用Alpha-Beta算法，搜索下一层的节点
			int eval = alphaBeta(board, depth - 1, alpha, beta, false, M, N, this_x, this_y, top, noX, noY);
			// 恢复落子和top
			board[this_x][this_y] = 0;
			top[j] += flag;

			// 更新alpha值
			alpha = max(alpha, eval);

			// 更新最大评估值
			maxEval = max(maxEval, eval);

			// 执行剪枝
			if (beta <= alpha) {
				break;
			}


		}

		return maxEval;
	}
	// 如果是最小化的回合
	// 即玩家的回合
	else {
		int minEval = INT_MAX;
		if (machineWin(last_x, last_y, M, N, board)) {
			// printBoardAndValue(board, M, N, -1, INT_MAX);
			return INT_MAX;
		}
		else if (isTie(N, top)) {
			// printBoardAndValue(board, M, N, -1, 0);
			return 0;
		}
		else if (depth == 0) {
			// printBoardAndValue(board, M, N, evaluateBoardFromSelf(board, M, N, 2, 1), evaluateBoard(board, M, N, 2, 1));
			return evaluateBoard(board, M, N, 2, 1, top);
		}
		// 遍历所有可能的动作
		// if (debug)
		// 	cerr << "player (1) deciding (maximizing) ..." << endl;
		for (int j = 0; j < N; j++) {
			int flag;
			// printBoard(board, M, N);
			// printArray(top, N);
			// cerr << j << "th column is space: " << isSpace(j, board, top, noX, noY) << endl;
			if (flag = isSpace(j, board, top, noX, noY)) {
				this_y = j;
				this_x = top[j] - flag;
			}
			else {
				continue;
			}

			// 更新落子和top
			board[this_x][this_y] = 1;
			top[j] -= flag;

			// 递归调用Alpha-Beta算法，搜索下一层的节点
			int eval = alphaBeta(board, depth - 1, alpha, beta, true, M, N, this_x, this_y, top, noX, noY);

			// 恢复落子和top
			board[this_x][this_y] = 0;
			top[j] += flag;

			// 更新beta值
			beta = min(beta, eval);

			// 更新最小评估值
			minEval = min(minEval, eval);
			// if (debug)
			// 	cerr << "minEval = " << minEval << endl;

			// 执行剪枝
			if (beta <= alpha) {
				// if (debug)
				// 	cerr << "beta = " <<beta << " alpha = " << alpha <<" Prune!" << endl;
				break;
			}


		}

		return minEval;
	}
}