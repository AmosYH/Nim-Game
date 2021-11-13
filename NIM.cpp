#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <time.h>
#include <iomanip>
using namespace std;

//only when 4 point all in the left side of the line, then you can win!

int game[32][32];
vector<pair<int, int>> point(16);
string ord[16] = { "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelveth", "thirdteenth", "fourteenth", "fifteenth", "sixteenth" };

void displaygame();
bool inputvalid(int pos, char dirc, int len);

class gamer {
private:
	int pos;
	int len;
	char dirc;
	string name;
	void upwardmove(int p, int l);
	void leftwardmove(int p, int l);
	vector<pair<int, int>> findinterval(vector<pair<int, int>> point);
	pair<int, int> findbound();
	void randomexecute(vector<pair<int, int>> interval, pair<int, int>bound);
	bool latiexecute(vector<pair<int, int>> interval, pair<int, int>bound);
	void longiexecute(vector<pair<int, int>> interval, pair<int, int>bound);
	int findtrialxorvalue(int p, int l, int d);
	int findmaxcapacity(int p, int d);
public:
	gamer(string aName) {
		pos = 0;
		len = 0;
		dirc = ' ';
		name = aName;
	}
	string getname() { return name; }
	void setprocedure(int p, char d, int l);
	void playerexecute();
	void computerexecute();
	bool iswin();
};

void gamer::upwardmove(int p, int l) {
	game[point[p].first][point[p].second] = 0;
	point[p].first -= l;
	game[point[p].first][point[p].second] = p + 1;
	cout << endl << name << " move " << ord[p] << " point upward with " << l << " unit length." << endl;
	displaygame();
	return;
}

void gamer::leftwardmove(int p, int l) {
	game[point[p].first][point[p].second] = 0;
	point[p].second -= l;
	game[point[p].first][point[p].second] = p + 1;
	cout << endl << name << " move " << ord[p] << " point to left with " << l << " unit length." << endl;
	displaygame();
	return;
}

vector<pair<int, int>> gamer::findinterval(vector<pair<int,int>> point) {
	vector<pair<int, int>> ret(16);
	bool lathasblockpoint;
	bool lgthasblockpoint;
	for (int i = 0; i < 16; i++) {
		lathasblockpoint = false;
		lgthasblockpoint = false;
		int maxlat = -1;
		int maxlgt = -1;
		for (int j = 0; j < 16; j++) {
			if (point[i].first > point[j].first && point[i].second == point[j].second) {
				if (point[j].first > maxlgt) {
					maxlgt = point[j].first;
					lgthasblockpoint = true;
				}
			}
			if (point[i].second > point[j].second && point[i].first == point[j].first) {
				if (point[j].second > maxlat) {
					maxlat = point[j].second;
					lathasblockpoint = true;
				}
			}
		}
		ret[i].first = !lgthasblockpoint ? point[i].first - 0 : point[i].first - maxlgt - 1;
		ret[i].second = !lathasblockpoint ? point[i].second - 0 : point[i].second - maxlat - 1;
	}
	return ret;
}

pair<int, int> gamer::findbound() {
	pair<int, int> ret = make_pair(0, 0);
	int ccnt = 0;
	int rcnt = 0;
	int row[4];
	int column[4];
	for (int i = 0; i < 4; i++) {
		rcnt = 0;
		ccnt = 0;
		for (int j = 0; j < 16; j++) {
			if (point[j].first == i) {
				rcnt++;
			}
			if (point[j].second == i) {
				ccnt++;
			}
		}
		row[i] = rcnt;
		column[i] = ccnt;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (game[i][j] == 0) {
				if (row[i] == 4) {
					column[j]++;
				}
				else if (column[j] == 4) {
					row[i]++;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (row[i] == 4) {
			ret.first++;
		}
		else {
			break;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (column[i] == 4) {
			ret.second++;
		}
		else {
			break;
		}
	}

	return ret;
}

void gamer::randomexecute(vector<pair<int, int>> interval, pair<int, int> bound) {
	int dirc;
	srand(time(0));
	dirc = rand() % 2;
	pos = rand() % 16;
	while (true) {
		if (dirc == 0) {
			len = interval[pos].second;
			while (len > 0) {
				if (interval[pos].second - len >= 0 && point[pos].second - len >= bound.second) {
					leftwardmove(pos, len);
					return;
				}
				len--;
			}
		}
		if (dirc == 1) {
			len = interval[pos].first;
			while (len > 0) {
				if (interval[pos].first - len >= 0 && point[pos].first - len >= bound.first) {
					upwardmove(pos, len);
					return;
				}
				len--;
			}
		}
		dirc = rand() % 2;
		pos = rand() % 16;
	}
	return;
}

bool gamer::latiexecute(vector<pair<int, int>> interval, pair<int, int>bound) {
	for (int i = 0; i < 4; i++) {
		int ps[4];
		int idx = 0;
		for (int j = 0; j < 16; j++) {
			if (point[j].first == i) {
				ps[idx] = j;
				idx++;
			}
		}
		if (idx == 4) {
			if (interval[ps[0]].second == 0 && interval[ps[1]].second == 0 && interval[ps[2]].second == 0 && interval[ps[3]].second != 0) {
				leftwardmove(ps[3], interval[ps[3]].second);
				return true;
			}
			if (interval[ps[0]].second == 0 && interval[ps[1]].second == 0 && interval[ps[2]].second == 0 && interval[ps[3]].second == 0) {
				continue;
			}
			int xorvalue = 0;
			for (auto i = interval.begin(); i != interval.end(); i++) {
				xorvalue ^= (*i).first;
				xorvalue ^= (*i).second;
			}
			if (xorvalue == 0) {
				for (int i = 0; i < 4; i++) {
					if (interval[ps[i]].second != 0) {
						leftwardmove(ps[i], interval[ps[i]].second);
						return true;
					}
				}
			}
			else {
				for (int i = 3; i >= 0; i--) {
					if (interval[ps[i]].second == 0) {
						continue;
					}
					for (int j = interval[ps[i]].second; j > 0; j--) {
						int temp = 0;
						temp = findtrialxorvalue(i, j, 0);
						if (temp == 0) {
							leftwardmove(ps[i], j);
							return true;
						}
					}
				}
				for (int i = 0; i < 4; i++) {
					for (int i = 0; i < 4; i++) {
						if (interval[ps[i]].second != 0) {
							leftwardmove(ps[i], interval[ps[i]].second);
							return true;
						}
					}
				}
				return true;
			}
		}
	}
	return false;
}

void gamer::longiexecute(vector<pair<int, int>> interval, pair<int, int>bound) {
	for (int i = 0; i < 4; i++) {
		int ps[4];
		int idx = 0;
		for (int j = 0; j < 16; j++) {
			if (point[j].second == i) {
				ps[idx] = j;
				idx++;
			}
		}
		if (idx == 4) {
			if (interval[ps[0]].first == 0 && interval[ps[1]].first == 0 && interval[ps[2]].first == 0 && interval[ps[3]].first != 0) {
				upwardmove(ps[3], interval[ps[3]].first);
				return;
			}
			if (interval[ps[0]].first == 0 && interval[ps[1]].first == 0 && interval[ps[2]].first == 0 && interval[ps[3]].first == 0) {
				continue;
			}
			int xorvalue = 0;
			for (auto i = interval.begin(); i != interval.end(); i++) {
				xorvalue ^= (*i).first;
				xorvalue ^= (*i).second;
			}
			if (xorvalue == 0) {
				for (int i = 0; i < 4; i++) {
					if (interval[ps[i]].first != 0) {
						upwardmove(ps[i], interval[ps[i]].first);
						return;
					}
				}
			}
			else {
				for (int i = 3; i >= 0; i--) {
					if (interval[ps[i]].first == 0) {
						continue;
					}
					for (int j = interval[ps[i]].first; j > 0; j--) {
						int temp = 0;
						temp = findtrialxorvalue(i, j, 1);
						if (temp == 0) {
							upwardmove(ps[i], j);
							return;
						}
					}
				}
				for (int i = 0; i < 4; i++) {
					if (interval[ps[i]].first != 0) {
						upwardmove(ps[i], 1);
						return;
					}
				}
				return;
			}
		}
	}
}

int gamer::findtrialxorvalue(int p, int l, int d) {
	vector<pair<int, int>> temppoint = point;
	vector<pair<int, int>> tempinterval;
	int ret = 0;
	int f, s;
	for (int i = 0; i < 16; i++) {
		if (i == p) {
			d == 0 ? temppoint[i].second -= l : temppoint[i].first -= l;
		}
	}
	point;
	tempinterval = findinterval(temppoint);
	for (auto e = tempinterval.begin(); e != tempinterval.end(); e++) {
		ret ^= (*e).first;
		ret ^= (*e).second;
	}
	return ret;
}

int gamer::findmaxcapacity(int p, int d) {
	int ccnt = 0;
	int rcnt = 0;
	int row[4];
	int column[4];
	for (int i = 0; i < 4; i++) {
		rcnt = 0;
		ccnt = 0;
		for (int j = 0; j < 16; j++) {
			if (point[j].first == i) {
				rcnt++;
			}
			if (point[j].second == i) {
				ccnt++;
			}
		}
		row[i] = rcnt;
		column[i] = ccnt;
	}
	int ret = 0;
	if (d == 0) {
		for (int i = 0; i <= p; i++) {
			ret += column[i];
		}
		return ret;
	}
	if (d == 1) {
		for (int i = 0; i <= p; i++) {
			ret += row[i];
		}
		return ret;
	}
}

void gamer::setprocedure(int p, char d, int l) {
	pos = p;
	dirc = d;
	len = l;
}

void gamer::playerexecute() {
	vector<pair<int, int>> interval = findinterval(point);
	pair<int, int> bound = findbound();
	int ccnt = 0;
	int rcnt = 0;
	int row[4];
	int column[4];
	for (int i = 0; i < 4; i++) {
		rcnt = 0;
		ccnt = 0;
		for (int j = 0; j < 16; j++) {
			if (point[j].first == i) {
				rcnt++;
			}
			if (point[j].second == i) {
				ccnt++;
			}
		}
		row[i] = rcnt;
		column[i] = ccnt;
	}
	//follow loop used to check input valid or over bound;
	while (true) {
		if (dirc == 'l') {
			if (interval[pos - 1].second -len < 0) {
				cout << endl << "You cannot move a point over its left boudary position." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].second < bound.second) {
				cout << endl << "You can only move this point upward!" << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].second - len < bound.second && point[pos - 1].first >= bound.first) {
				cout << endl << "You cannnot move a point over " << ord[bound.second - 1] << " cloumn because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].second - len < 4 && findmaxcapacity(point[pos - 1].second - len, 0) == 4 * (point[pos - 1].second - len + 1)) {
				cout << endl << "You cannnot move a point over " << ord[bound.second - 1] << " cloumn because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].first < bound.first && point[pos - 1].second - len < 4 - row[point[pos - 1].first]) {
				cout << endl << "You cannnot move a point over " << ord[4 - row[point[pos - 1].first] - 1] << " cloumn because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else {
				break;
			}
		}
		if (dirc == 'u') {
			if (interval[pos - 1].first - len < 0) {
				cout << endl << "You cannot move a point over its up boudary position." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].first < bound.first) {
				cout << endl << "You can only move this point leftward!" << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].first - len < bound.first && point[pos - 1].second >= bound.second) {
				cout << endl << "You cannnot move a point over " << ord[bound.first - 1] << " row because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].first - len < 4 && findmaxcapacity(point[pos - 1].first - len, 1) == 4 * (point[pos - 1].first - len + 1)) {
				cout << endl << "You cannnot move a point over " << ord[point[pos - 1].first - len] << " row because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else if (point[pos - 1].second < bound.second && point[pos - 1].first - len < 4 - column[point[pos - 1].second]) {
				cout << endl << "You cannnot move a point over " << ord[4 - column[point[pos - 1].second] - 1] << " row because it is full." << endl;
				cout << name << ", please input a correct direction, point order and move length." << endl;
				cin >> pos >> dirc >> len;
			}
			else {
				break;
			}
		}
		while (!inputvalid(pos, dirc, len)) {
			cout << name << ", please input a correct point order, direction and move length." << endl;
			cin >> pos >> dirc >> len;
		}
	}
	pos--;
	if (dirc == 'l') {
		leftwardmove(pos, len);
	}
	if (dirc == 'u') {
		upwardmove(pos, len);
	}
	return;
}

void gamer::computerexecute() {
 	vector<pair<int, int>> interval = findinterval(point);
	pair<int, int>bound = findbound();
	findbound();
	if (bound.first == 4 && bound.second == 4) {
		if (!latiexecute(interval, bound)){
			longiexecute(interval, bound);
		}
		return;
	}
	int xorvalue = 0;
	for (auto i = interval.begin(); i != interval.end(); i++) {
		xorvalue ^= (*i).first;
		xorvalue ^= (*i).second;
	}
	int dirc;
	if (xorvalue == 0) {
		randomexecute(interval, bound);
	}
	else {
		for (int i = 15; i >= 0; i--) {
			if (interval[i].first == 0 && interval[i].second == 0) {
				continue;
			}
			int temp;
			if (interval[i].second != 0 && point[i].second >= bound.second) {
				for (int j = interval[i].second; j > 0; j--) {
					temp = 0;
					if (point[i].second - j >= bound.second && point[i].first >= bound.first) {
						temp = findtrialxorvalue(i, j, 0);
						if (temp == 0) {
							leftwardmove(i, j);
							return;
						}
					}
				}
			}
			if (interval[i].first != 0 && point[i].first >= bound.first) {
				for (int j = interval[i].first; j > 0; j--) {
					temp = 0;
					if (point[i].first - j >= bound.first && point[i].second >= bound.second) {
						temp = findtrialxorvalue(i, j, 1);
						if (temp == 0) {
							upwardmove(i, j);
							return;
						}
					}
				}
			}
		}
		randomexecute(interval, bound);
		return;
	}

}

bool gamer::iswin() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (game[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}

void modepvp() {
	string name;
	cout << endl << "Input player1's name:" << endl;
	cin >> name;
	gamer* p1 = new gamer(name);
	cout << endl << "Input player2's name" << endl;
	cin >> name;
	gamer* p2 = new gamer(name);
	bool player1play = true;
	while (true) {
		char direction;
		int len, pos;
		if (player1play) {
			cout << endl << "Hello, " << p1->getname() << "! Please input the point order(1 to 16), the direction('l'(left) or 'u'(up)) and the relevant length (no less than 1) you want to move: " << endl;
			cin >> pos >> direction >> len;
			while (!inputvalid(pos, direction, len)) {
				cout << p1->getname() << ", please input a correct point order, direction and move length." << endl;
				cin >> pos >> direction >> len;
			}
			p1->setprocedure(pos, direction, len);
			p1->playerexecute();
			player1play = false;
			if (p1->iswin()) {
				cout << p1->getname() << " win!" << endl;
				return;
			}
		}
		else {
			cout << endl << "Hello, " << p2->getname() << "! Please input the point order(1 to 16), the direction('l'(left) or 'u'(up)) and the relevant length (no less than 1) you want to move: " << endl;
			cin >> pos >> direction >> len;
			while (!inputvalid(pos, direction, len)) {
				cout << p2->getname() << ", please input a correct point order, direction and move length." << endl;
				cin >> pos >> direction >> len;
			}
			p2->setprocedure(pos, direction, len);
			p2->playerexecute();
			player1play = true;
			if (p2->iswin()) {
				cout << p2->getname() << " win!" << endl;
				return;
			}
		}
	}
}

void modepvc() {
	int order;
	bool playerplay = true;
	string name;
	cout << endl << "Input your name:" << endl;
	cin >> name;
	gamer* p = new gamer(name);
	gamer* c = new gamer("Computer");

	cout << endl << "Please choose whether you want to do first, 0(yes) or 1(no) (p.s. Any other input not 0 or 1 would be contemplated as 1):" << endl;
	cin >> order;

	order = order == 0 ? 0 : 1;
	playerplay = order == 1 ? false : true;

	while (true) {
		char direction;
		int pos, len;
		if (playerplay) {
			cout << endl << "Hello, " << p->getname() << "! Please input the point order(1 to 16), the direction('l'(left) or 'u'(up)) and the relevant length (no less than 1) you want to move: " << endl;
			cin >> pos >> direction >> len;
			while (!inputvalid(pos, direction, len)) {
				cout << p->getname() << ", please input a correct point order, direction and move length." << endl;
				cin >> pos >> direction >> len;
			}
			p->setprocedure(pos, direction, len);
			p->playerexecute();
			playerplay = false;
			if (p->iswin()) {
				cout << p->getname() << " win!" << endl;
				return;
			}
		}
		else {
			c->computerexecute();
			playerplay = true;
			if (c->iswin()) {
				cout << c->getname() << " win!" << endl;
				return;
			}
		}
	}
}

void modecvc() {
	bool computer1play = true;
	gamer* c1 = new gamer("Computer1");
	gamer* c2 = new gamer("Computer2");

	while (true) {
		if (computer1play) {
			c1->computerexecute();
			computer1play = false;
			if (c1->iswin()) {
				cout << c1->getname() << " win!" << endl;
				return;
			}
		}
		else {
			c2->computerexecute();
			computer1play = true;
			if (c2->iswin()) {
				cout << c2->getname() << " win!" << endl;
				return;
			}
		}
	}
}

void displaygame() {
	cout << endl << "Game area Shows as follow: " << endl;
	int maxlat = 0;
	int maxlgt = 0;
	for (int i = 0; i < 16; i++) {
		if (point[i].first > maxlgt) {
			maxlgt = point[i].first;
		}
		if (point[i].second > maxlat) {
			maxlat = point[i].second;
		}
	}
	for (int i = 0; i <= maxlgt; i++) {
		for (int j = 0; j <= maxlat; j++) {
			cout << setw(3) << game[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

bool inputvalid(int pos, char dirc, int len) {
	if (pos > 16) {
		cout << endl << "There is only 16 point!" << endl;
		return false;
	}
	else if (pos < 1) {
		cout << endl << "The point position must be positive!" << endl;
		return false;
	}
	else if (!(dirc == 'l' || dirc == 'u')) {
		cout << endl << "The dirction you input can not be identified!" << endl;
		return false;
	}
	else if (len < 1) {
		cout << endl << "The length you want to move must be positive!" << endl;
		return false;
	}
	else {
		return true;
	}
}

int main() {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			game[i][j] = 0;
		}
	}
	for (int i = 31; i > 15; i--) {
		point[i - 16] = make_pair(i, i);
	}
	for (int i = 0; i < 16; i++) {
		game[point[i].first][point[i].second] = i + 1;
	}
	displaygame();

	int mode;
	cout << "Please choose a game mode(1 indicates computer vs. computer(p.s any other input not -1 or 0 or 1 would be contemplated choose mode 1))" << endl;
	cin >> mode;
	mode = mode == -1 || mode == 0 ? mode : 1;
	if (mode == -1) {
		cout << endl << "You have chose the player vs. player mode." << endl;
		modepvp();
	}
	if (mode == 0) {
		cout << endl << "You have chose the player vs.computer mode." << endl;
		modepvc();
	}
	if (mode == 1) {
		cout << endl << "You have chose the computer vs. computer mode." << endl;
		modecvc();
	}
	return 0;
}