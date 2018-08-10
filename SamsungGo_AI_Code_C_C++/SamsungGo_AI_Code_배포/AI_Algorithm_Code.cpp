// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "Connect6Algo.h"

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:SixNeck,Department:KAIST" };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MYDEBUG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int MY_BOARD_SIZE = 19;
const int MAX_DEPTH = 3;
const int MAX_CAND = 7;
const long long INF = 1e15;

std::pair<std::pair<int, int>, std::pair<int, int> > next_step;
std::vector<std::pair<int, int> > able;

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_candidate(int cand_num, bool isMyStance);
long long montecarlo(int depth);

void update_eval(std::pair<int, int>);
void restore_eval(std::pair<int, int>);
void update_block(std::pair<int, int>);
long long evaluate();

long long wholeEval = 0;
long long diff = 0;
bool invalid[19][19][4];
long long score[19][19][4];
bool isAbled[19][19];

long long tcut = 0;

void first_move();

bool fit(int x, int y);

void do_the_algorithm();

void myinit();
bool myinited = false;

typedef std::pair<int, int> Point;
int safe_board(int a, int b);
bool is_kill_angle(bool isMyStance, Point a, Point b);
std::pair<Point, Point> get_kill_point(bool isMyStance);
void updateAble();
bool isWinStone(bool isMyStance, int x, int y);
std::pair<Point, Point> defenseDeath(bool isMyStance);

bool imfirst;
int turncnt;

#ifdef MYDEBUG
FILE * logname;
#endif

void do_the_algorithm()
{
	
    // 이거 왜 안됨 ㅠ
    montecarlo(0);
    return;
    // 한 수만 본다
    
    int i, j, k, l;
    long long r = -INF * 1000, c1, c2;
    for(i = 0; i<19; i++)
    {
        for(j = 0; j<19; j++)
        {
            if(board[i][j] != 0)
                continue;

            board[i][j] = 1;
            update_eval({ i, j });
            c1 = evaluate();

            for(k = 0; k<19; k++)
            {
                for(l = 0; l<19; l++)
                {
                    if(board[k][l] != 0)
                        continue;

                    board[k][l] = 1;
                    update_eval({ k, l });
                    c2 = evaluate();

                    if(c1 + c2 > r)
                    {
                        r = c1 + c2;
                        next_step = { { i, j }, { k, l } };
                    }

                    board[k][l] = 0;
                    restore_eval({ k, l });
                }
            }

            board[i][j] = 0;
            restore_eval({ i, j });
        }
    }
}

int my_x[2];
int my_y[2];
int my_cnt = 0;

void myturn(int cnt) {
	tcut = 0;
    int x[2], y[2];
	if (cnt == 1) imfirst = true;
    // 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
    // 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

    if(!myinited)
    {
        myinited = true;
        myinit();
    }

	int di[] = { 1,1,-1,-1,0,0,1,-1 };
	int dj[] = {-1,1,1,-1,1,-1,0,0};
	turncnt++;
    if(op_cnt)
    {
        for(int i = 0; i<op_cnt; i++)
        {
#ifdef MYDEBUG
            fprintf(logname, "TURN : %d\nop (%d, %d)\n", turncnt, op_x[i], op_y[i]);
            fflush(logname);
#endif

            update_eval({ op_x[i], op_y[i] });
			for (int newx = op_x[i] - 2; newx <= op_x[i] + 2; newx++) {
				for (int newy = op_y[i] - 2; newy <= op_y[i] + 2; newy++) {
					if (fit(newx, newy) && board[newx][newy] == 0 && isAbled[newx][newy] == false) {
						isAbled[newx][newy] = true;
						able.push_back({ newx, newy });
					}
				}
			}
        }
    }
    if(my_cnt)
    {
        for(int i = 0; i<my_cnt; i++)
        {

#ifdef MYDEBUG
            fprintf(logname, "my (%d, %d)\n", my_x[i], my_y[i]);
            fflush(logname);
#endif
            update_eval({ my_x[i], my_y[i] });
			for (int newx = my_x[i] - 2; newx <= my_x[i] + 2; newx++) {
				for (int newy = my_y[i] - 2; newy <= my_y[i] + 2; newy++) {
					if (fit(newx, newy) && board[newx][newy] == 0 && isAbled[newx][newy] == false) {
						isAbled[newx][newy] = true;
						able.push_back({ newx, newy });
					}
				}
			}
        }
    }
    diff = 0;
	updateAble();
    if(cnt == 1)	
    {
        first_move();
        x[0] = next_step.first.first;
        y[0] = next_step.first.second;
    }
    else
    {
        auto ret = get_kill_point(true);
#ifdef MYDEBUG
		fprintf(logname, "kill %d %d %d %d\n", ret.first.first, ret.first.second, ret.second.first, ret.second.second);
		fflush(logname);
#endif
		if (ret.first.first == -1) {
			ret = defenseDeath(true);
#ifdef MYDEBUG
			fprintf(logname, "death %d %d %d %d\n", ret.first.first, ret.first.second, ret.second.first, ret.second.second);
			fflush(logname);
#endif 
			if (ret.first.first != -1)
			{
				if (ret.second.first != -1)
				{
#ifdef MYDEBUG
					fprintf(logname, " prevent death with two points\n");
					fflush(logname);
#endif 
				}
				else
				{
					board[ret.first.first][ret.first.second] = 1;
					update_eval(ret.first);
					long long best = evaluate();
					for (auto p : able)
					{
						if (board[p.first][p.second] != 0) continue;
						board[p.first][p.second] = 1;
						update_eval(p);
						if (best < evaluate()) {
							best = evaluate();
							ret.second = p;
						}
						board[p.first][p.second] = 0;
						update_eval(p);
					}
					board[ret.first.first][ret.first.second] = 0;
					update_eval(ret.first);
#ifdef MYDEBUG
					fprintf(logname, "death plus %d %d\n", ret.second.first, ret.second.second);
					fflush(logname);
#endif
				}
				x[0] = ret.first.first;
				y[0] = ret.first.second;
				x[1] = ret.second.first;
				y[1] = ret.second.second;
			}
		}
		if(ret.first.first == -1)
        {
            do_the_algorithm();
#ifdef MYDEBUG
			fprintf(logname, "do algo %d %d %d %d\n", next_step.first.first, next_step.first.second, next_step.second.first, next_step.second.second);
			fflush(logname);
#endif 
            x[0] = next_step.first.first;
            y[0] = next_step.first.second;
            x[1] = next_step.second.first;
            y[1] = next_step.second.second;
        }
        else
        {
#ifdef MYDEBUG
			fprintf(logname, "??? %d %d %d %d\n", ret.first.first, ret.first.second, ret.second.first, ret.second.second);
			fflush(logname);
#endif 
            x[0] = ret.first.first;
            y[0] = ret.first.second;
            x[1] = ret.second.first;
            y[1] = ret.second.second;
        }
    }

    my_cnt = cnt;
    for(int i = 0; i < cnt; i++)
    {
        my_x[i] = x[i];
        my_y[i] = y[i];
    }

#ifdef MYDEBUG
    fprintf(logname, "put (%d, %d), (%d %d) cnt = %d\ntcut %lld\n", x[0], y[0], x[1], y[1], cnt, tcut);
    fflush(logname);
#endif

    // 이 부분에서 자신이 놓을 돌을 출력하십시오.
    // 필수 함수 : domymove(x배열,y배열,배열크기)
    // 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.

    domymove(x, y, cnt);
}

bool fit(int x, int y)
{
    return 0<=x && x<19 && 0<=y && y<19;
}

void myinit()
{
    int x, y, i, j;

#ifdef MYDEBUG
    if(imfirst) while((logname = fopen("mylog_black.txt", "w")) == NULL);
	else  while ((logname = fopen("mylog_white.txt", "w")) == NULL);
#endif

    //srand((unsigned)time(NULL));
	wholeEval = 0;
    for(i = 0; i < MY_BOARD_SIZE; i++) {
        for(j = 0; j < MY_BOARD_SIZE; j++) {
            //able.push_back({ i, j });
			isAbled[i][j] = false;
        }
    }

    for(i = 0; i<19; i++)
    {
        for(j = 0; j<19; j++)
        {
            x = i + 5;
            y = j;
            if(!fit(x, y))
                invalid[i][j][0] = 1;

            x = i;
            y = j + 5;
            if(!fit(x, y))
                invalid[i][j][1] = 1;

            x = i + 5;
            y = j + 5;
            if(!fit(x, y))
                invalid[i][j][2] = 1;

            x = i + 5;
            y = j - 5;
            if(!fit(x, y))
                invalid[i][j][3] = 1;
        }
    }

#ifdef MYDEBUG
    int cc = 0, d;
    for(i = 0; i<19; i++) {
        for(j = 0; j<19; j++) {
            for(d = 0; d<4; d++) {
                if(!invalid[i][j][d]) {
                    cc++;
                }
            }
        }
    }
    fprintf(logname, "unit cnt without block = %d\n", cc);
    fflush(logname);
#endif

    for(i = 0; i<block_cnt; i++)
        update_block({ block_x[i], block_y[i] });
}

const int dx[4] = { -1, 0, -1, -1 };
const int dy[4] = { 0, -1, -1, 1 };

void first_move()
{
    int md, mb, td, tb, nx, ny, i, j, k, d;
    md = 10000;
    mb = 10000;
    for(i = 0; i<19; i++)
    {
        for(j = 0; j<19; j++)
        {
            if(board[i][j] != 0)
                continue;

            td = (i-9)*(i-9) + (j-9)*(j-9);

            tb = 0;
            for(d = 0; d<4; d++)
            {
                for(k = -7; k<=7; k++)
                {
                    nx = i + dx[d]*k;
                    ny = j + dy[d]*k;

                    if(!fit(nx, ny) || board[nx][ny] == 3)
                        tb += (k==-7 || k==7 ? 1 : 2);
                }
            }

            if(tb < mb || tb == mb && td < md)
            {
                mb = tb;
                md = td;
                next_step.first.first = i;
                next_step.first.second = j;
            }
        }
    }
}

void update_score(int x, int y, int d, long long r)
{
	wholeEval += r - score[x][y][d];
    diff += r - score[x][y][d];
    score[x][y][d] = r;
}

void update_block(std::pair<int, int> p)
{
    int x, y, nx, ny, d, i;
    x = p.first;
    y = p.second;

    for(d = 0; d<4; d++)
    {
        for(i = 0; i<6; i++)
        {
            nx = x + dx[d]*i;
            ny = y + dy[d]*i;

            if(fit(nx, ny))
                invalid[nx][ny][d] = 1;
        }
    }
}

// Gave bonus to 4 and 5 (Potential 1 turn kill)
const long long hard[7] = {
    0,
    1,
    4,
    9,
    32,
    50,
    INF
};

void update_eval(std::pair<int, int> p)
{
    int cx, cy, nx, ny, x, y, d, i, j;
    cx = p.first;
    cy = p.second;

    for(d = 0; d<4; d++)
    {
        for(i = -1; i<7; i++)
        {
            nx = cx + dx[d]*i;
            ny = cy + dy[d]*i;

            if(!fit(nx, ny) || invalid[nx][ny][d])
                continue;

            int cnt1, cnt2;
            cnt1 = cnt2 = 0;
            for(j = 0; j<6; j++)
            {
                x = nx - dx[d]*j;
                y = ny - dy[d]*j;
                if(board[x][y] == 1)
                    cnt1++;
                else if(board[x][y] == 2)
                    cnt2++;
                //else
                //    assert(board[x][y] == 0);
            }

            if(cnt1 == 0 && cnt2 == 0 || cnt1 != 0 && cnt2 != 0)
            {
                update_score(nx, ny, d, 0);
                continue;
            }

            int col = cnt1 ? 1 : 2;
            int cnt = cnt1 ? cnt1 : cnt2;

            x = nx - dx[d]*(-1);
            y = ny - dy[d]*(-1);
            if(fit(x, y) && board[x][y] == col)
            {
                update_score(nx, ny, d, 0);
                continue;
            }

            x = nx - dx[d]*6;
            y = ny - dy[d]*6;
            if(fit(x, y) && board[x][y] == col)
            {
                update_score(nx, ny, d, 0);
                continue;
            }

            if(col == 1)
                update_score(nx, ny, d, hard[cnt]);
            else
                update_score(nx, ny, d, -hard[cnt]);
        }
    }
}

void restore_eval(std::pair<int, int> p)
{
    update_eval(p);
}

long long evaluate()
{
    //return diff;
	return wholeEval;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_candidate(int cand_num, bool isMyStance)
{
    long long val;
    std::vector<std::pair<long long, std::pair<int, int> > > cont;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> res;
    for(int i = 0; i < able.size(); i++)
    {
        if(board[able[i].first][able[i].second] != 0) continue;
        for(int j = i + 1; j < able.size(); j++)
        {
            if(board[able[j].first][able[j].second] != 0) continue;
            if(able[i] == able[j]) continue;
            std::pair<int, int> p1 = able[i], p2 = able[j];

            board[able[i].first][able[i].second] = isMyStance ? 1 : 2;
            board[able[j].first][able[j].second] = isMyStance ? 1 : 2;
            update_eval(p1); update_eval(p2);

            if(isMyStance) val = -evaluate();
            else val = evaluate();

            board[able[i].first][able[i].second] = 0;
            board[able[j].first][able[j].second] = 0;
            restore_eval(p1); restore_eval(p2);

            cont.push_back({ val,{ i, j } });
        }
    }
    std::sort(cont.begin(), cont.end());
    if(cont.empty()) return res;
    long long threshold = cont[min((int)cont.size()-1, cand_num - 1)].first;
    int left, right;
    //[left, right) 구간에서 cont의 값은 threshold와 같음
    left = right = min((int)cont.size() - 1, cand_num - 1);
    while(left - 1 >= 0 && cont[left - 1].first == threshold) left--;
    while(right < (int)cont.size() && cont[right].first == threshold) right++;
    std::random_shuffle(cont.begin()+left, cont.begin()+right);
    for(int i = 0; i < min(cand_num, (int)cont.size()); i++) res.push_back({able[cont[i].second.first], able[cont[i].second.second]});
    return res;
}

long long montecarlo(int depth)
{
    //auto choose = depth&1 ? [](int a, int b)->int { return min(a, b); } : [](int a, int b)->int { return max(a, b); };
    bool is_ret_upd = false;
    long long ret;
    if(depth == MAX_DEPTH) {
        return evaluate();
    }
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int> > > candidate = get_candidate(MAX_CAND, !(depth&1));
    std::pair<std::pair<int, int>, std::pair<int, int> > opt;
    for(auto &cand : candidate) {

        int xx1, xx2, yy1, yy2;
        xx1 = cand.first.first;
        yy1 = cand.first.second;
        xx2 = cand.second.first;
        yy2 = cand.second.second;
		if (board[xx1][yy1] != 0 || board[xx2][yy2] != 0) continue;
#ifdef MYDEBUG
        //fprintf(logname, "candidate : %d %d , %d %d (%d) (%d)\n", xx1, yy1, xx2, yy2, board[xx1][yy1], board[xx2][yy2]);
        fflush(logname);
#endif
		tcut++;
        //assert(board[xx1][yy1] == 0 && board[xx2][yy2] == 0);
        //assert(xx1 != yy1 || xx2 != yy2);

        board[cand.first.first][cand.first.second] = (depth&1) + 1;
        board[cand.second.first][cand.second.second] = (depth&1) + 1;

        update_eval(cand.first);
        update_eval(cand.second);

        long long tmp = montecarlo(depth+1);
        if(!is_ret_upd)
        {
            ret = tmp;
            is_ret_upd = true;
            opt = cand;
        }
        else
        {
            long long new_ret = depth&1 ? min(ret, montecarlo(depth+1)) : max(ret, montecarlo(depth+1));
            if(new_ret != ret) {
                ret = new_ret;
                opt = cand;
            }
        }

        board[cand.first.first][cand.first.second] = 0;
        board[cand.second.first][cand.second.second] = 0;

        restore_eval(cand.second);
        restore_eval(cand.first);

        if(ret < -1e9 || ret > 1e9)
            break;
    }
    if(depth == 0) {
        next_step = opt;
    }
    return ret;
}



int safe_board(int a, int b, Point c, Point d, int stone)
{
    Point e = { a, b };
    if(a<0 || a >= MY_BOARD_SIZE || b<0 || b >= MY_BOARD_SIZE) return -1;
    if(e == c || e == d) return stone;
    return board[a][b];
}

bool is_kill_angle(bool isMyStance, Point a, Point b)
{
    int dx[] = { 1, 1, 0, -1 };
    int dy[] = { 0, 1, 1, 1 };
    int stone = isMyStance ? 1 : 2;
    for(int i = 0; i<MY_BOARD_SIZE; i++)
    {
        for(int j = 0; j<MY_BOARD_SIZE; j++)
        {
            if(safe_board(i, j, a, b, stone) != stone) continue;
            for(int dir = 0; dir < 4; dir++)
            {
                if(safe_board(i - dx[dir], j - dy[dir], a, b, stone) == stone) continue;
                int cnt = 1, nowx = i, nowy = j;
                while(safe_board(nowx + dx[dir], nowy + dy[dir], a, b, stone) == stone) {
                    nowx += dx[dir]; nowy += dy[dir]; cnt++;
                }
                if(cnt == 6) return true;
            }
        }
    }
    return false;
}

//do not update "able"
//not angle => return {{-1, -1}, {-1, -1}}
std::pair<Point, Point> get_kill_point(bool isMyStance)
{
    for(int i = 0; i < able.size(); i++)
    {
        if(board[able[i].first][able[i].second] != 0) continue;
        for(int j = i + 1; j < able.size(); j++)
        {
            if(board[able[j].first][able[j].second] != 0) continue;
            if(able[i] == able[j]) continue;
            if(is_kill_angle(isMyStance, able[i], able[j]))
            {
                return{ able[i], able[j] };
            }
        }
    }
    return{ { -1, -1 }, { -1, -1 } };
}

void updateAble()
{
	std::vector<Point> newable;
	for (auto p : able) {
		if (board[p.first][p.second] == 0)
			newable.push_back(p);
	}
	std::swap(able, newable);
}


int safe_board2(int a, int b)
{
	if (a<0 || a >= MY_BOARD_SIZE || b<0 || b >= MY_BOARD_SIZE) return -1;
	return board[a][b];
}

//상대방이 둘 곳들을 반환
std::vector<Point> search_death_angle(bool isMyStance)
{
	std::vector<Point> cands;
	int dx[] = { 1, 1, 0, -1 };
	int dy[] = { 0, 1, 1, 1 };
	int stone = isMyStance ? 2 : 1;
	bool isIn[19][19];
	for (int i = 0; i < 19; i++)for (int j = 0; j < 19; j++)isIn[i][j] = false;
	for (int i = 0; i < MY_BOARD_SIZE; i++)
	{
		for (int j = 0; j < MY_BOARD_SIZE; j++)
		{
			for (int dir = 0; dir < 4; dir++)
			{
				if (safe_board2(i - dx[dir], j - dy[dir]) == stone)continue;
				int cnt = 0, nowx = i, nowy = j, blank = 0;
				for (int k = 0; k < 6; k++) {
					if (safe_board2(nowx, nowy) == stone) cnt++;
					else if (safe_board2(nowx, nowy) == 0) blank++;
					nowx += dx[dir]; nowy += dy[dir];
				}
				if (safe_board2(nowx, nowy) == stone || cnt < 4 || (blank + cnt != 6)) continue;
				nowx = i; nowy = j;
				for (int k = 0; k < 6; k++) {
					if (safe_board2(nowx, nowy) == 0 && isIn[nowx][nowy] == false) {
						cands.push_back({ nowx, nowy });
						isIn[nowx][nowy] = true;
					}
					nowx += dx[dir]; nowy += dy[dir];
				}
			}
		}
	}
	return cands;
}

bool isWinStone(bool isMyStance, int cx, int cy)
{
	int col, nx, ny, x, y, d, i, j;

	col = isMyStance ? 1 : 2;

	for (d = 0; d<4; d++)
	{
		for (i = -1; i<7; i++)
		{
			nx = cx + dx[d] * i;
			ny = cy + dy[d] * i;

			if (!fit(nx, ny) || invalid[nx][ny][d])
				continue;

			int cnt = 0;
			for (j = 0; j<6; j++)
			{
				x = nx - dx[d] * j;
				y = ny - dy[d] * j;
				if (board[x][y] == col)
					cnt++;
			}

			if (cnt != 6)
				continue;

			x = nx - dx[d] * (-1);
			y = ny - dy[d] * (-1);
			if (fit(x, y) && board[x][y] == col)
				continue;

			x = nx - dx[d] * 6;
			y = ny - dy[d] * 6;
			if (fit(x, y) && board[x][y] == col)
				continue;

			return true;
		}
	}

	return false;
}

std::pair<Point, Point> defenseDeath(bool isMyStance)
{
	std::pair<Point, Point> best = { {-1,-1}, {-1,-1} }; bool isBestExist = false;
	long long bestval;
	std::vector<Point> & enemy_cands = search_death_angle(isMyStance); // 무조건 다 비어있는 칸만 있어야됨
	if (enemy_cands.empty()) return{ {-1,-1},{-1,-1} };
	if (enemy_cands.size() == 1) {
		return{ enemy_cands.front(), {-1, -1} };
	}
	int enemy_stone = isMyStance ? 2 : 1;
	int my_stone = isMyStance ? 1 : 2;

	for (int i = 0; i < enemy_cands.size(); i++)
	{
		board[enemy_cands[i].first][enemy_cands[i].second] = my_stone;
		update_eval(enemy_cands[i]);
			bool isDefensed = true;
			for (int a = 0; isDefensed && a < enemy_cands.size(); a++)
			{
				if (a == i) continue;
				board[enemy_cands[a].first][enemy_cands[a].second] = enemy_stone;
				if (isWinStone(!isMyStance, enemy_cands[a].first, enemy_cands[a].second))
					isDefensed = false;
#ifdef MYDEBUG
				fprintf(logname, "attack A With (%d %d)\n", enemy_cands[a].first, enemy_cands[a].second);
				fflush(logname);
#endif			
				for (int b = a + 1; isDefensed && b < enemy_cands.size(); b++)
				{
					if (b == i) continue;
#ifdef MYDEBUG
					fprintf(logname, "attack B With (%d %d)\n", enemy_cands[b].first, enemy_cands[b].second);
					fflush(logname);
#endif			
					board[enemy_cands[b].first][enemy_cands[b].second] = enemy_stone;
					if (isWinStone(!isMyStance, enemy_cands[b].first, enemy_cands[b].second))
						isDefensed = false;
					board[enemy_cands[b].first][enemy_cands[b].second] = 0;
				}
				board[enemy_cands[a].first][enemy_cands[a].second] = 0;
			}
			if (isDefensed == true) {
				if (isBestExist == false) {
					isBestExist = true;
#ifdef MYDEBUG
					fprintf(logname, "DEFENSE With %d %d\n", enemy_cands[i].first, enemy_cands[i].second);
					fflush(logname);
#endif
					best = { enemy_cands[i], {-1,-1} };
					bestval = evaluate();
				}
				else {
					if (bestval < evaluate()) {
						bestval = evaluate();
						best = { enemy_cands[i],{ -1,-1 } };
					}
				}
			}
		board[enemy_cands[i].first][enemy_cands[i].second] = 0;
		restore_eval(enemy_cands[i]);
	}
	if (isBestExist) return best;

	for (int i = 0; i < enemy_cands.size(); i++)
	{
		board[enemy_cands[i].first][enemy_cands[i].second] = my_stone;
		update_eval(enemy_cands[i]);
		for (int j = i+1; j < enemy_cands.size(); j++)
		{
			board[enemy_cands[j].first][enemy_cands[j].second] = my_stone;
			update_eval(enemy_cands[j]);

			bool isDefensed = true;
			for (int a = 0; isDefensed && a < enemy_cands.size(); a++)
			{
				if (a == i || a == j) continue;
				board[enemy_cands[a].first][enemy_cands[a].second] = enemy_stone;
				if (isWinStone(!isMyStance, enemy_cands[a].first, enemy_cands[a].second))
					isDefensed = false;
				for (int b = a + 1; isDefensed && b < enemy_cands.size(); b++)
				{
					if (b == i || b == j) continue;
					board[enemy_cands[b].first][enemy_cands[b].second] = enemy_stone;
					if (isWinStone(!isMyStance, enemy_cands[b].first, enemy_cands[b].second))
						isDefensed = false;
					board[enemy_cands[b].first][enemy_cands[b].second] = 0;
				}
				board[enemy_cands[a].first][enemy_cands[a].second] = 0;
			}
			if (isDefensed == true) {
				if (isBestExist == false) {
					isBestExist = true;
					best = { enemy_cands[i], enemy_cands[j] };
					bestval = evaluate();
				}
				else {
					if (bestval < evaluate()) {
						bestval = evaluate();
						best = { enemy_cands[i], enemy_cands[j] };
					}
				}
			}
			board[enemy_cands[j].first][enemy_cands[j].second] = 0;
			restore_eval(enemy_cands[j]);
		}
		board[enemy_cands[i].first][enemy_cands[i].second] = 0;
		restore_eval(enemy_cands[i]);
	}
	return best;
}
