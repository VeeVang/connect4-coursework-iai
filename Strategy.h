/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2014.5                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "Point.h"

extern "C" Point *getPoint(const int M, const int N, const int *top, const int *_board,
						   const int lastX, const int lastY, const int noX, const int noY);

extern "C" void clearPoint(Point *p);

void clearArray(int M, int N, int **board);

bool EmptyOrSelf(int *const *board, int M, int N, int i, int j, int self);

bool Empty(int *const *board, int M, int N, int i, int j, int self);

int evaluateBoard(int *const *board, int M, int N, int self, int opponent);

int evaluateBoardFromSelf(int *const *board, int M, int N, int self, int opponent);

int isSpace(int j, int *const *board, const int *top, int noX, int noY);

int alphaBeta(int **board, int depth, int alpha, int beta, bool maximizing, int M, int N, int last_x, int last_y, int *top, int noX, int noY);

/*
	添加你自己的辅助函数
*/

#endif