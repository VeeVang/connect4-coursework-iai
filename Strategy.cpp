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

// ��һ�������Կ�����Χ��λ���ǲ��ǿ�����������ȥ�ģ�������������������Ϳ��Ը�������һ�����ʡ�


/*
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������
	
	input:
		Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
		M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
		top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
		_board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
				��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
				board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
				board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
		lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
				����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
		noX, noY : �����ϵĲ������ӵ�(ע:��ʵ���������top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
				������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
				��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
				��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
		���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
		������ӵ�Point
*/
// extern "C" __declspec(dllexport) Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY){
extern "C" Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY){
	/*
		��Ҫ������δ���
	*/
	int x = -1, y = -1;//���ս�������ӵ�浽x,y��
	int** board = new int*[M];
	for(int i = 0; i < M; i++){
		board[i] = new int[N];
		for(int j = 0; j < N; j++){
			board[i][j] = _board[i * N + j];
		}
	}
	/*
		�������Լ��Ĳ������������ӵ�,Ҳ���Ǹ�����Ĳ�����ɶ�x,y�ĸ�ֵ
		�ò��ֶԲ���ʹ��û�����ƣ�Ϊ�˷���ʵ�֣�����Զ����Լ��µ��ࡢ.h�ļ���.cpp�ļ�
	*/
	//Add your own code below

	/*--------------------����--------------------------*/
	// ż����
	int depth = 6;
	
	int* modifiable_top = new int[N];
	for (int j = 0; j < N; j++) {
		modifiable_top[j] = top[j];
	}
	board[noX][noY] = 3;

	// �Է���������	1 ��ʾ���Լ��ĳ���������� 2 ��ʾ��
	// �������ӵ㱻��Ϊ3

	// �����һ�����ԣ�Ȼ���ҵ���Ӧ�ķ��򼴿ɣ�
	int this_x, this_y;
	int maxEval = INT_MIN;
	int alpha = INT_MIN;
	// �������п��ܵĶ���
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

		// �������Ӻ�top
		board[this_x][this_y] = 2;
		modifiable_top[j] -= flag;

		// �ݹ����Alpha-Beta�㷨��������һ��Ľڵ�
		int eval = alphaBeta(board, depth - 1, alpha, INT_MAX, false, M, N, this_x, this_y, modifiable_top, noX, noY);
		if (debug)
			cerr << j << "th column eval = " << eval << endl;
		
		// �ָ����Ӻ�top
		board[this_x][this_y] = 0;
		modifiable_top[j] += flag;

		// ����alphaֵ
		alpha = max(alpha, eval);

		// �����������ֵ
		maxEval = max(maxEval, eval);
		// if (debug)
		// 	cerr << "maxEval = " << maxEval << endl;
		if (maxEval == eval) {
			x = this_x;
			y = this_y;
		}
	}

	// ���ӵ�һ��Ϊ x = _top[y] - 1
	// ��_top[y]Ϊ0ʱ������������


	delete[] modifiable_top;

	// --- User code ends ---

	/*
		��Ҫ������δ���
	*/
	clearArray(M, N, board);
	return new Point(x, y);
}


/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
*/
// extern "C" __declspec(dllexport) void clearPoint(Point* p){
extern "C" void clearPoint(Point* p){
	delete p;
	return;
}

/*
	���top��board����
*/
void clearArray(int M, int N, int** board){
	for(int i = 0; i < M; i++){
		delete[] board[i];
	}
	delete[] board;
}


/*
	������Լ��ĸ�������������������Լ����ࡢ����������µ�.h .cpp�ļ�������ʵ������뷨
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

// ���̼�ֵ=����Ŀǰ�ļ�ֵ-�Է�Ŀǰ��ֵļ�ֵ��
int evaluateBoard(int* const * board, int M, int N, int self, int opponent, const int* top) {
	return evaluateBoardFromSelf(board, M, N, self, opponent, top) - evaluateBoardFromSelf(board, M, N, opponent, self, top);
}

void printBoardAndValue(int* const * board, int M, int N, int value_of_2, int value){
	if (debug){
		cerr << "evaluating board value!..." << endl;
		cerr << "board: " << endl;
		for(int i=0; i<M; i++) {
			for(int j=0; j<N; j++) //ÿ����m��
				cerr<<board[i][j]<<" "; //�����i�е�j�е���
			cerr<<endl; //ÿ�����һ�У������һ�����з�
		}
		cerr << "value of player 2 only: " << value_of_2 << endl;
		cerr << "comprehensive value: " << value << endl;
	}
}

void printBoard(int* const * board, int M, int N){
	if (debug){
		cerr << "board: " << endl;
		for(int i=0; i<M; i++) {
			for(int j=0; j<N; j++) //ÿ����m��
				cerr<<board[i][j]<<" "; //�����i�е�j�е���
			cerr<<endl; //ÿ�����һ�У������һ�����з�
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

// ���̹�ֵ
// �ٲ�һ��abs(top[j - 1] - (i - 1)) * connect_2_score ����top[j + 3]��һ���Ӿ����䣬��ô�͸����ߵķ�����
int evaluateBoardFromSelf(int* const* board, int M, int N, int self, int opponent, const int* top) {
	int value = 0;
	// ����ɨ��
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			// ÿһ������
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
			// ���������������һ�˿��������ӻ�Ϊ�Լ�����
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
			// ���������������һ�˿���������
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
	// ����ɨ��
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < M; i++) {
			// ���������������һ�˿��������ӻ�Ϊ�Լ�����
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
			// ���������������һ�˿��������ӻ�Ϊ�Լ�����
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
	// ��б��ɨ��
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
	// ��б��ɨ��
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
	// ��top����ռ����ʱ
	if (top[j] == 0) {
		return 0;
	}
	// ��top����ûռ��
	// �ҵ���һ���Ӿ��п�ʱ
	else if (top[j] - 1 != noX || j != noY){
		return 1;
	}
	// ��top����ûռ��
	// �ҵ���һ����ΪnoX, noYʱ
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

// ����㷨�ǿ�����ÿ�θ�������ʱ��ȷ���Ա�ÿ�����ȵġ�
// �ǵ�ÿ��ά��board��top
// �ǵ�is tie
int alphaBeta(int** board, int depth, int alpha, int beta, bool maximizing, 
	int M, int N, 
	int last_x, int last_y, int* top, int noX, int noY) {
	// int * const* const_board = board;
	// const int * const_top = top;
	 
	int this_x;
	int this_y;
	// �������󻯵Ļغ�
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
		// �������п��ܵĶ���
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

			// �������Ӻ�top
			board[this_x][this_y] = 2;
			top[j] -= flag;

			// �ݹ����Alpha-Beta�㷨��������һ��Ľڵ�
			int eval = alphaBeta(board, depth - 1, alpha, beta, false, M, N, this_x, this_y, top, noX, noY);
			// �ָ����Ӻ�top
			board[this_x][this_y] = 0;
			top[j] += flag;

			// ����alphaֵ
			alpha = max(alpha, eval);

			// �����������ֵ
			maxEval = max(maxEval, eval);

			// ִ�м�֦
			if (beta <= alpha) {
				break;
			}


		}

		return maxEval;
	}
	// �������С���Ļغ�
	// ����ҵĻغ�
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
		// �������п��ܵĶ���
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

			// �������Ӻ�top
			board[this_x][this_y] = 1;
			top[j] -= flag;

			// �ݹ����Alpha-Beta�㷨��������һ��Ľڵ�
			int eval = alphaBeta(board, depth - 1, alpha, beta, true, M, N, this_x, this_y, top, noX, noY);

			// �ָ����Ӻ�top
			board[this_x][this_y] = 0;
			top[j] += flag;

			// ����betaֵ
			beta = min(beta, eval);

			// ������С����ֵ
			minEval = min(minEval, eval);
			// if (debug)
			// 	cerr << "minEval = " << minEval << endl;

			// ִ�м�֦
			if (beta <= alpha) {
				// if (debug)
				// 	cerr << "beta = " <<beta << " alpha = " << alpha <<" Prune!" << endl;
				break;
			}


		}

		return minEval;
	}
}