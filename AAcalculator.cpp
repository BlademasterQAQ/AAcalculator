#include <iostream>
#include <fstream>
#include <string>//用于cin string
#include <ctime>//用于读取系统时间
#include <conio.h>//用于getch
using namespace std;

#define OK 0
#define ERROR -1
#define FileStreamOver -2

typedef struct Refund {	//用于计算最终的还款
	double money;		//钱数
	Refund *next;
}Refund, *RefundPtr;

typedef struct User {	//参与付款的用户
	string UserName;
	RefundPtr refund;	//账单
	User *next;
}User, *UserPtr;

typedef struct Account {//账单流水
	int PayerID;		//付款者ID
	int BePaidID;		//被付款者ID(ID表示在链表中的位置，为负时表示为全体付款的人数(包括自己)
	double money;
	string time;			//付款时间
	string ps;			//备注
	int isEffective;	//这调记录是否被删除(删除为0)
	Account *next;
}Account, *AccountPtr;

string getSystemTime();//获取系统时间

int ClearRefundList(UserPtr &userHead);//删除Refund链表
int CreateRefundList(UserPtr &userHead);//由User生成空的Enlist链表
double ReadRefundList(UserPtr &userHead, int x, int y);//返回Refund链表中第x个User的第y个账单
int WriteRefundList(UserPtr &userHead, int x, int y, double Value);//在Refund链表中第x个User的第y个账单处写入值Value
int autoWriteRefund(UserPtr &userHead, int x, int y, double Value);//自动写入金额，会比较对方对自己是否有欠款以自动抵消以及查看自己是否含有欠款
int EnRefundList(UserPtr &userHead, AccountPtr &accountHead);//区分账单是否有效和是否为为所有人付款，通过不同的方式调用autoWriteRefund实现获取欠款信息

int CreateUserList(UserPtr &userHead);//创建链表
int EnUserList(UserPtr &userHead, string UserName);////添加一个用户
int UserListLength(UserPtr &userHead);//返回User链表中元素的个数
int IndexUserName(UserPtr &userHead, string str);//查找用户名在链表中的位置，若找不到则返回ERROR
int ShowUserName(UserPtr &userHead);//显示所有用户名
string GetUserName(UserPtr &userHead, int local);//根据数据在链表中的位置返回名字（负数返回'所有人'）

int CreateAccountList(AccountPtr &accountHead);//创建链表
int EnAccountList(AccountPtr &accountHead, int PayerID, int BePaidID, double money, string ps, string time = getSystemTime(), int isEffective = 1);//写入一个新账单
int ClearAccountList(AccountPtr &accountHead);//清空Account

int GetSpacebarCount();//获取空格出现次数，用于确定用户的个数
int ReadFile(UserPtr& U, AccountPtr& A);//读文件
int WriteFile(UserPtr& U, AccountPtr& A);//写文件


//得到合法输入
/*
char getLegalInput(char* legalchar) {
	char a;
	int length = 0;
	while ((int)legalchar[length] >= 0) {//获取legalchar的长度
		length++;
	}
	do {
		a = _getch();

		for (int i = 0; i < length; i++) {
			if (a == legalchar[i])
				return a;
		}
	} while (1);
	
}
*/
int main() {
	restart:
	UserPtr U;
	CreateUserList(U);

	AccountPtr A;
	CreateAccountList(A);

	cout << "正在读取文件..." << endl;
	ReadFile(U, A);
	system("pause");
	system("cls");

	while (1) {
		system("cls");
		cout << "**主菜单**" << endl;
		cout << "1.用户" << endl;
		cout << "2.付款记录" << endl;
		cout << "3.结算" << endl;
		cout << "4.账单分析" << endl;
		cout << "5.清空全部信息" << endl;
		cout << "0.退出系统" << endl;

		char n = _getch();

		switch (n) {

			//添加用户
		case '1': {
			int i = 1;
			while (i) {
				;
				system("cls");
				cout << "1.添加用户名" << endl;
				cout << "2.查看用户名" << endl;
				cout << "3.修改用户名" << endl;
				cout << "输入q返回上一级" << endl;

				char n = _getch();
				switch (n) {
				case '1': {
					system("cls");
					cout << "添加用户名中...输入q返回上一级" << endl;
					cout << "请输入用户名字:";
					string str;
					cin >> str;
					if (str == "q")break;

					cout << endl << "是否保存?\t1.保存\t2.不保存" << endl;

					int i = 1;//处理非法输入模块(因手机版段溢出)
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':
							EnUserList(U, str);
							WriteFile(U, A);
							i = 0;
							break;
						case '2':i = 0;
							break;
						}
					}
				}break;

				case '2':
					system("cls");
					if (UserListLength(U) == 0) {
						cout << "无可用用户，请先添加用户" << endl;
						system("pause");
						break;
					}
					cout << "可用用户名:" << endl;
					ShowUserName(U);
					system("pause");
					break;

				case '3': {

				changeUser:
					system("cls");
					if (UserListLength(U) == 0) {
						cout << "无可用用户，请先添加用户" << endl;
						system("pause");
						break;
					}
					cout << "修改用户名中...输入q返回上一级" << endl;
					cout << "可用用户名:" << endl;
					ShowUserName(U);
					cout << "请输入想要修改的用户名:";
					int UserID;
					string str;
					cin >> str;
					if (str == "q")break;
					if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//输入付款人在链表中的位置
						UserID = atoi(str.c_str());
					}
					else {
						if (IndexUserName(U, str) != ERROR)
							UserID = IndexUserName(U, str);
						else {
							cout << "找不到该用户" << endl;
							system("pause");
							goto changeUser;
						}
					}

					system("cls");
					cout << "请输入更改后的用户名:";
					cin >> str;

					cout << endl << "是否保存?\t1.保存\t2.不保存" << endl;

					int i = 1;
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':{
							UserPtr Urear = U;
							for (int i = 0; i < UserID; i++) {
								Urear = Urear->next;
							}
							Urear->UserName = str;
							WriteFile(U, A);
							i = 0; 
						}break;
						case '2':i = 0;
							break;
						}
					}
				}break;

				case 'q':i = 0;
					break;
				}
			}
		}break;

			//添加付款记录
		case '2': {
			int i = 1;
			while (i) {
				system("cls");
				cout << "1.添加付款记录" << endl;
				cout << "2.查看付款记录" << endl;
				cout << "3.删除付款记录" << endl;
				cout << "4.查看历史付款记录(包括已删除的)" << endl;
				cout << "0.清空付款记录(不可恢复)" << endl;
				cout << "输入q返回上一级" << endl;

				char n = _getch();
				switch (n) {

				case '1': {
					system("cls");
					string str;
					AccountPtr Aptr = new Account();
					if (UserListLength(U) == 0) {
						cout << "无可用用户，请先添加用户" << endl;
						system("pause");
						break;
					}

					//增加付款人
				addPayer:
					system("cls");
					cout << "添加付款记录中...输入q返回上一级" << endl;
					cout << "可用付款人:" << endl;
					ShowUserName(U);
					cout << "请选择付款人:";

					cin >> str;
					if (str == "q")break;
					if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//输入付款人在链表中的位置
						Aptr->PayerID = atoi(str.c_str());
					}
					else {
						if (IndexUserName(U, str) != ERROR)
							Aptr->PayerID = IndexUserName(U, str);
						else {
							cout << "找不到该用户" << endl;
							system("pause");
							goto addPayer;
						}
					}

					//增加被付款人
				addBePaid:
					system("cls");
					cout << "添加付款记录中...输入q返回上一级" << endl;
					cout << "可用被付款人:" << endl;
					cout << "0.当前所有人\t";
					ShowUserName(U);
					cout << "请选择被付款人:";

					cin >> str;
					if (str == "q")break;

					if (str.length() == 1 && str[0] == '0')//输入0时代表所有人
						Aptr->BePaidID = -UserListLength(U);
					else {
						if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//输入付款人在链表中的位置
							Aptr->BePaidID = atoi(str.c_str());
						}
						else {
							if (IndexUserName(U, str) != ERROR)
								Aptr->BePaidID = IndexUserName(U, str);
							else {
								cout << "找不到该用户" << endl;
								system("pause");
								goto addBePaid;
							}
						}
					}

					//增加金额
				addMoney:
					system("cls");
					cout << "请输入付款金额:";
					int money;
					cin >> money;
					if (cin.fail()) {
						cin.clear(); cin.sync();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						goto addMoney;
					}
					Aptr->money = money;

					//增加备注
					system("cls");
					cout << "请输入备注:";
					cin >> Aptr->ps;

					system("cls");
					cout << "付款信息录入完成" << endl;
					cout << "付款信息如下:" << endl;
					cout << GetUserName(U, Aptr->PayerID) << " 为 " << GetUserName(U, Aptr->BePaidID) << " 支付 " << Aptr->money << " 元" << endl;
					cout << "ps:" << Aptr->ps << endl << endl;

					cout << endl << "是否保存?\t1.保存\t2.不保存" << endl;

					int i = 1;
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':
							EnAccountList(A, Aptr->PayerID, Aptr->BePaidID, Aptr->money, Aptr->ps);
							WriteFile(U, A);
							i = 0;
							break;
						case '2':i = 0;
							break;
						}
					}
				}break;

				case '2': {
					system("cls");
					if (A->next == NULL) {
						cout << "无付款记录，请先添加付款记录" << endl;
						system("pause");
						break;
					}

					cout << "所有有效付款记录如下:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//账单的次序
					while (Aptr->next) {
						Aptr = Aptr->next;
						if (Aptr->isEffective)
							cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\t为   " << GetUserName(U, Aptr->BePaidID) << "\t支付 " << Aptr->money << " 元\t" << Aptr->time << "  " << Aptr->ps << endl;
					}
					if (local == 0) {
						system("cls");
						cout << "无付款记录，请先添加付款记录" << endl;
					}
					system("pause");
				}break;

				case '3': {
					system("cls");
					if (A->next == NULL) {
						cout << "无付款记录，请先添加付款记录" << endl;
						system("pause");
						break;
					}

					cout << "所有有效付款记录如下:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//账单的次序
					while (Aptr->next) {
						Aptr = Aptr->next;
						if (Aptr->isEffective)
							cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\t为   " << GetUserName(U, Aptr->BePaidID) << "\t支付 " << Aptr->money << " 元\t" << Aptr->time << "  " << Aptr->ps << endl;
					}
					if (local == 0) {
						system("cls");
						cout << "无付款记录，请先添加付款记录" << endl;
						system("pause");
						break;
					}

					cout << endl << "删除付款记录中...输入0返回上一级" << endl;
					cout << "请输入要删除的付款记录的序号:";
					int order;

				reset:
					cin >> order;
					if (order == 0)break;
					if (cin.fail() || order > local) {
						cin.clear(); cin.sync();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						goto reset;
					}

					cout << endl << "是否保存?\t1.保存\t2.不保存" << endl;

					int i = 1;
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':
							Aptr = A;
							for (int i = 0; i < order; i++) {
								Aptr = Aptr->next;
								if (!Aptr->isEffective)//无效的跳过
									i--;
							}
							Aptr->isEffective = 0;

							WriteFile(U, A);
							i = 0;
							break;
						case '2':i = 0;
							break;
						}
					}
				}break;

				case '4': {
					system("cls");
					if (A->next == NULL) {
						cout << "无付款记录，请先添加付款记录" << endl;
						system("pause");
						break;
					}

					cout << "所有历史付款记录(包括已删除的)如下:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//账单的次序
					while (Aptr->next) {
						Aptr = Aptr->next;
						cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\t为   " << GetUserName(U, Aptr->BePaidID) << "\t支付 " << Aptr->money << " 元\t" << Aptr->time << "  " << Aptr->ps;
						if (!Aptr->isEffective)
							cout << " (已删除)";
						cout << endl;
					}
					system("pause");
				}break;

				case '0': {
					system("cls");
					if (A->next == NULL) {
						cout << "无付款记录，请先添加付款记录" << endl;
						system("pause");
						break;
					}

					cout << "确认清空付款记录?(无法恢复)\t1.清空\t2.不清空" << endl;
					int i = 1;
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':
							ClearAccountList(A);
							WriteFile(U, A);
							i = 0;
							break;
						case '2':i = 0;
							break;
						}
					}
				}break;

				case 'q':i = 0;
					break;

				}

			}
		}break;


		case '3': {
			system("cls");
			if (CreateRefundList(U) == ERROR)//创建Refund链表，等于ERROR时无用户
				return 0;
			if (EnRefundList(U, A) == ERROR)//写入Refund链表，等于ERROR时无付款记录，此处创建完Refund链表
				return 0;
			
			cout << "P2P(点对点)结算:(仅简化为每两个人之间的欠款)" << endl;
			UserPtr Urear = U;
			int x = 0;//用于存储在链表中的位置
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0)
						cout << GetUserName(U, x) << " 需还 " << GetUserName(U, y) << " " << Rrear->money << " 元"<<endl;
				}
			}
			//cout << U->next->refund->next->money;//第一个User的第一个Refund
			//cout << U->next->next->next->next->refund->next->next->next->next->money;
			
			cout << endl;
			cout << "进一步简化后的结算:(目的使流动金额和支付次数尽可能的少)" << endl;
			Urear = U;
			x = 0;//用于存储在链表中的位置
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0) {//当发现(x,y)(第x行第y列)存在欠款时
						//检查是否构成A->B->C的还款顺序
						for (int i = 1; i <= UserListLength(U); i++) {//查找第y行第i列是否与其构成x->y->i的还款顺序，i应不等于x和y(本质上也不可能)
							if (ReadRefundList(U, y, i) != 0) {
								double sub = Rrear->money - ReadRefundList(U, y, i);
								if (sub >= 0) {//x替y还完y需要还i的，再还差额给y
									WriteRefundList(U, x, y, sub);
									WriteRefundList(U, x, i, ReadRefundList(U, y, i) + ReadRefundList(U, x, i));//x替y还一部分还i的钱，可能x原本就需要还i
									WriteRefundList(U, y, i, 0);//x替y还完i，y不再需要还i
								}
								else {//x替y还欠y的部分给i，剩余部分由y还
									WriteRefundList(U, y, i, -sub);
									WriteRefundList(U, x, i, ReadRefundList(U, x, y) + ReadRefundList(U, x, i));
									WriteRefundList(U, x, y, 0);//x替y还一部分i，x不再需要还y
								}
							}
						}
					}
				}
			}
			Urear = U;
			x = 0;//用于存储在链表中的位置
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0)
						cout << GetUserName(U, x) << " 需还 " << GetUserName(U, y) << " " << Rrear->money << " 元" << endl;
				}
			}
			system("pause");
		}break;


		case '4': {
			system("cls");
			cout << "账单分析:" << endl << endl;
			int UserCount=UserListLength(U);//获取用户人数

			cout << "共有" << UserCount << "个用户" << endl;
			cout << "用户名为:";
			ShowUserName(U);
			cout << endl;

			AccountPtr Aptr = A;
			double AllPay = 0;//总付款
			double AAPay = 0;//AA制总付款
			while (Aptr->next) {
				Aptr = Aptr->next;
				
				AllPay = AllPay + Aptr->money;
				if (Aptr->BePaidID < 0) {
					AAPay = AAPay + Aptr->money;
				}
			}
			cout << "总付款分析:" << endl;
			cout << "总付款:" << AllPay << endl;
			cout << "平均总付款:" << AllPay / UserCount << endl;

			cout << endl;
			cout << "AA付款分析:(不含为个人付款的记录)" << endl;
			cout << "AA总付款:" << AAPay << endl;
			cout << "AA平均付款:" << AAPay / UserCount << endl;

			cout << endl;
			cout << "个人付款分析:" << endl;
			for (int i = 1; i <= UserCount; i++) {//分别计算每个用户的账单数据
				Aptr = A;
				double PayMoney = 0;
				while (Aptr->next) {
					Aptr = Aptr->next;
					if (Aptr->PayerID == i) {//付款者为正在查找的付款者id
						PayMoney = PayMoney + Aptr->money;
					}
				}
				cout << GetUserName(U, i) << "共支付" << PayMoney << "元" << endl;
			}
			system("pause");
		}break;


		case '5': {
			system("cls");
			cout << "确认清空全部记录?(无法恢复)\t1.清空\t2.不清空" << endl;
			int i = 1;
			while (i) {
				char save = _getch();
				switch (save) {
				case '1': {
					fstream writeFile("basedata.txt", ifstream::out);
					writeFile << '\n';
					writeFile.close();
					system("cls");
					goto restart;
				}break;
				case '2':i = 0;
					break;
				}
			}
			
		}break;

		case '0':return 0;
		}

	}
}

//Refund基本操作
int ClearRefundList(UserPtr &userHead) {//删除Refund链表
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;

		RefundPtr R1, R2;
		R1 = Urear->refund;
		while (R1) {
			R2 = R1->next;
			free(R1);
			R1 = R2;
		}
		Urear->refund = NULL;
	}
	return OK;
}

int CreateRefundList(UserPtr &userHead) {//由User生成空的Enlist链表
	ClearRefundList(userHead);//首先清理已存在的Refund链表

	UserPtr Urear = userHead;

	int UserCount = UserListLength(userHead);
	
	if (UserCount == 0) {
		system("cls");
		cout << "无可用用户，请先添加用户" << endl;
		return ERROR;
	}

	Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;

		RefundPtr Rhead;
		Rhead = (RefundPtr)malloc(sizeof(Refund));
		Urear->refund = Rhead;
		Rhead->next = NULL;
		for (int i = 0; i < UserCount; i++) {
			RefundPtr Rrear;
			Rrear = (RefundPtr)malloc(sizeof(Refund));
			Rrear->money = 0;
			Rrear->next = NULL;

			Rhead->next = Rrear;
			Rhead = Rhead->next;
		}
	}
	return OK;
}

double ReadRefundList(UserPtr &userHead,int x, int y) {//返回Refund链表中第x个User的第y个账单
	if (x > UserListLength(userHead) || y > UserListLength(userHead))
		return ERROR;

	UserPtr Urear = userHead;
	while (x--) {
		Urear = Urear->next;
	}
	RefundPtr Rrear = Urear->refund;
	while (y--) {
		Rrear = Rrear->next;
	}
	return Rrear->money;
}

int WriteRefundList(UserPtr &userHead, int x, int y, double Value) {//在Refund链表中第x个User的第y个账单处写入值Value
	if (x > UserListLength(userHead) || y > UserListLength(userHead))
		return ERROR;

	UserPtr Urear = userHead;
	while (x--) {
		Urear = Urear->next;
	}
	RefundPtr Rrear = Urear->refund;
	while (y--) {
		Rrear = Rrear->next;
	}
	Rrear->money = Value;
	return OK;
}

int autoWriteRefund(UserPtr &userHead, int x, int y, double Value) {//自动写入金额，会比较对方对自己是否有欠款以自动抵消以及查看自己是否含有欠款
	if (ReadRefundList(userHead, y, x) != 0) {//y对x有欠款时
		double sub = Value - ReadRefundList(userHead, y, x);//差值
		if (sub >= 0) {//x欠y大于y欠x，应删除y欠x，并将差值赋给x欠y
			WriteRefundList(userHead, y, x, 0);
			WriteRefundList(userHead, x, y, sub);
		}
		else {//y欠x大于x欠y
			WriteRefundList(userHead, y, x, -sub);
		}
	}
	else {
		if (ReadRefundList(userHead, x, y) != 0) {//x对y有欠款时
			double add = Value + ReadRefundList(userHead, x, y);
			WriteRefundList(userHead, x, y, add);
		}
		else//x，y均无欠款
			WriteRefundList(userHead, x, y, Value);
	}
	return OK;
}

int EnRefundList(UserPtr &userHead, AccountPtr &accountHead) {
	AccountPtr Arear = accountHead;
	int count = 0;
	while (Arear->next) {
		Arear = Arear->next;
		if (Arear->isEffective == 1) {
			count++;
		}
	}
	if (count == 0) {
		cout << "无有效付款记录，请先添加付款记录" << endl;
		return ERROR;
	}
	Arear = accountHead;
	while (Arear->next) {
		Arear = Arear->next;
		if (Arear->isEffective == 1) {
			if (Arear->BePaidID > 0) {//为个人付款时
				autoWriteRefund(userHead, Arear->BePaidID, Arear->PayerID, Arear->money);//写入被付款者的User链表下
			}
			if (Arear->BePaidID < 0) {//为所有人付款
				for (int i = 1; i <= -Arear->BePaidID; i++) {
					if(i!=Arear->PayerID)//除去为自己付款的情况
						autoWriteRefund(userHead, i, Arear->PayerID, Arear->money / -Arear->BePaidID);
				}
			}
		}
	}
	return OK;
}

//User基本操作
int CreateUserList(UserPtr &userHead) {				//创建链表
	userHead = (UserPtr)malloc(sizeof(User));
	if (!userHead) return ERROR;
	userHead->next = NULL;
	return OK;
}

int EnUserList(UserPtr &userHead, string UserName) {		//添加一个用户
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;
	}

	UserPtr U = new User();			//当结构体含有string时不能用malloc
	U->UserName = UserName;
	U->next = NULL;

	Urear->next = U;
	return OK;
}

int UserListLength(UserPtr &userHead) {//返回User链表中元素的个数
	int i = 0;
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;
		i++;
	}
	return i;
}

int IndexUserName(UserPtr &userHead, string str) {
	int i = 0;
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;
		i++;
		if (str == Urear->UserName)
			return i;
	}
	return ERROR;
}

int ShowUserName(UserPtr &userHead) {
	int i = 0;
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;
		i++;
		cout << i << "." << Urear->UserName << "   ";
	}
	cout << endl;
	return OK;
}

string GetUserName(UserPtr &userHead, int local) {//根据数据在链表中的位置返回名字
	if (local < 0)
		return "所有人";
	if (local > 0 && local <= UserListLength(userHead)) {
		UserPtr Urear = userHead;
		for (int i = 0; i < local; i++) {
			Urear = Urear->next;
		}
		return Urear->UserName;
	}
	return "";
}

//Account基本操作
int CreateAccountList(AccountPtr &accountHead) {				//创建链表
	accountHead = (AccountPtr)malloc(sizeof(Account));
	if (!accountHead) return ERROR;
	accountHead->next = NULL;
	return OK;
}

int EnAccountList(AccountPtr &accountHead, int PayerID, int BePaidID, double money, string ps, string time, int isEffective) {
	AccountPtr Arear = accountHead;
	while (Arear->next) {
		Arear = Arear->next;
	}

	AccountPtr A = new Account();
	cout << A->time;
	A->PayerID = PayerID;
	A->BePaidID = BePaidID;
	A->money = money;
	A->time = time;
	A->ps = ps;
	A->isEffective = isEffective;
	A->next = NULL;

	Arear->next = A;
	return OK;
}

int ClearAccountList(AccountPtr &accountHead) {//清空Account
	AccountPtr A1, A2;
	A1 = accountHead->next;
	while (A1) {
		A2 = A1->next;
		free(A1);
		A1 = A2;
	}
	accountHead->next = NULL;
	return OK;
}


//获取系统时间
string getSystemTime() {
	time_t now;
	struct tm *fmt = new tm[1];
	time(&now);
	localtime_s(fmt, &now);
	//cout << fmt->tm_hour << ":" << fmt->tm_min << ":" << fmt->tm_sec;
	string year;
	year = to_string(fmt->tm_year + 1900);//tm_year为1900开始的年数，应加1900
	string month;
	month = to_string(fmt->tm_mon + 1);//月份范围为0~11，应手动加1
	string day;
	day = to_string(fmt->tm_mday);
	string hour;
	if ((int)fmt->tm_hour / 10 == 0)
		hour = " " + to_string(fmt->tm_hour);
	else
		hour = to_string(fmt->tm_hour);
	string min;
	if ((int)fmt->tm_min / 10 == 0)
		min = "0" + to_string(fmt->tm_min);
	else
		min = to_string(fmt->tm_min);
	string sec;
	if ((int)fmt->tm_sec / 10 == 0)
		sec = "0" + to_string(fmt->tm_sec);
	else
		sec = to_string(fmt->tm_sec);
	string s;
	s = year + "/" + month + "/" + day + "--" + hour + ":" + min + ":" + sec;
	return s;
}

//读文件
int GetSpacebarCount() {
	fstream readFile("basedata.txt", ifstream::in);
	int i = 0;//用于记录空格出现次数
	char a;
	if (readFile.get() == '\n') {//无数据的清况(开头就是空格)
		readFile.seekg(-1, ios::cur);
		return ERROR;
	}
	readFile.seekg(-1, ios::cur);
	do {
		a = readFile.get();
		if (a == ' ')
			i++;
	} while (a != '\n');

	readFile.close();
	return i;//返回元素个数
}

int ReadFile(UserPtr& U, AccountPtr& A) {
	fstream readFile("basedata.txt", ifstream::in);

	if (readFile.fail()) {//文件不存在时
		readFile.close();
		fstream writeFile("basedata.txt", ifstream::out);
		writeFile << '\n';
		writeFile.close();
	}

	if (GetSpacebarCount() == ERROR) {//无用户名时
		cout << "无可用用户名，请添加用户名" << endl;

		//清空
		fstream writeFile("basedata.txt", ifstream::out);
		writeFile << '\n';
		writeFile.close();

		readFile.close();
		return ERROR;
	}
	for (int i = 0; i < GetSpacebarCount(); i++) {
		string str;
		readFile >> str;
		EnUserList(U, str);
	}

	cout << "用户读取完成 (共" << GetSpacebarCount() << "个用户)" << endl;
	readFile.get();//去除残留的' '
	readFile.get();//去除残留的'\n'
				   //readFile.getline(new char[0], 100, '\n');//去除残留的'\n'

	if (readFile.get() == -1) {//当到达文件末尾后，get -1
		cout << "无付款记录" << endl;
		readFile.close();
		return OK;
	}
	int count = 0;
	do {
		readFile.seekg(-1, ios::cur);

		AccountPtr Aptr = new Account();
		readFile >> Aptr->PayerID;
		readFile >> Aptr->BePaidID;
		readFile >> Aptr->money;
		readFile >> Aptr->time;
		readFile >> Aptr->ps;
		readFile >> Aptr->isEffective;
		if (Aptr->isEffective == 1)
			count++;

		EnAccountList(A, Aptr->PayerID, Aptr->BePaidID, Aptr->money, Aptr->ps, Aptr->time, Aptr->isEffective);

		readFile.get();//去除残留的' '
		readFile.get();//去除残留的'\n'
	} while (readFile.get() != -1);
	if (count == 0)
		cout << "无付款记录" << endl;
	else
		cout << "付款记录读取完成 (共" << count << "个有效付款记录)" << endl;

	readFile.close();
	return OK;
}

//写文件
int WriteFile(UserPtr& U, AccountPtr& A) {
	if (UserListLength(U) == 0) {
		return ERROR;
	}
	fstream writeFile("basedata.txt", ifstream::out);

	UserPtr Urear = U;
	while (Urear->next) {
		Urear = Urear->next;
		writeFile << Urear->UserName << " ";
	}
	writeFile << '\n';

	AccountPtr Arear = A;
	int count = 0;
	while (Arear->next) {
		Arear = Arear->next;
		writeFile << Arear->PayerID << " " << Arear->BePaidID << " " << Arear->money << " " << Arear->time << " " << Arear->ps << " " << Arear->isEffective << " " << endl;
		count++;
	}

	writeFile.close();
	return OK;
}
