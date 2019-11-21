#include <iostream>
#include <fstream>
#include <string>//����cin string
#include <ctime>//���ڶ�ȡϵͳʱ��
#include <conio.h>//����getch
using namespace std;

#define OK 0
#define ERROR -1
#define FileStreamOver -2

typedef struct Refund {	//���ڼ������յĻ���
	double money;		//Ǯ��
	Refund *next;
}Refund, *RefundPtr;

typedef struct User {	//���븶����û�
	string UserName;
	RefundPtr refund;	//�˵�
	User *next;
}User, *UserPtr;

typedef struct Account {//�˵���ˮ
	int PayerID;		//������ID
	int BePaidID;		//��������ID(ID��ʾ�������е�λ�ã�Ϊ��ʱ��ʾΪȫ�帶�������(�����Լ�)
	double money;
	string time;			//����ʱ��
	string ps;			//��ע
	int isEffective;	//�����¼�Ƿ�ɾ��(ɾ��Ϊ0)
	Account *next;
}Account, *AccountPtr;

string getSystemTime();//��ȡϵͳʱ��

int ClearRefundList(UserPtr &userHead);//ɾ��Refund����
int CreateRefundList(UserPtr &userHead);//��User���ɿյ�Enlist����
double ReadRefundList(UserPtr &userHead, int x, int y);//����Refund�����е�x��User�ĵ�y���˵�
int WriteRefundList(UserPtr &userHead, int x, int y, double Value);//��Refund�����е�x��User�ĵ�y���˵���д��ֵValue
int autoWriteRefund(UserPtr &userHead, int x, int y, double Value);//�Զ�д�����Ƚ϶Է����Լ��Ƿ���Ƿ�����Զ������Լ��鿴�Լ��Ƿ���Ƿ��
int EnRefundList(UserPtr &userHead, AccountPtr &accountHead);//�����˵��Ƿ���Ч���Ƿ�ΪΪ�����˸��ͨ����ͬ�ķ�ʽ����autoWriteRefundʵ�ֻ�ȡǷ����Ϣ

int CreateUserList(UserPtr &userHead);//��������
int EnUserList(UserPtr &userHead, string UserName);////���һ���û�
int UserListLength(UserPtr &userHead);//����User������Ԫ�صĸ���
int IndexUserName(UserPtr &userHead, string str);//�����û����������е�λ�ã����Ҳ����򷵻�ERROR
int ShowUserName(UserPtr &userHead);//��ʾ�����û���
string GetUserName(UserPtr &userHead, int local);//���������������е�λ�÷������֣���������'������'��

int CreateAccountList(AccountPtr &accountHead);//��������
int EnAccountList(AccountPtr &accountHead, int PayerID, int BePaidID, double money, string ps, string time = getSystemTime(), int isEffective = 1);//д��һ�����˵�
int ClearAccountList(AccountPtr &accountHead);//���Account

int GetSpacebarCount();//��ȡ�ո���ִ���������ȷ���û��ĸ���
int ReadFile(UserPtr& U, AccountPtr& A);//���ļ�
int WriteFile(UserPtr& U, AccountPtr& A);//д�ļ�


//�õ��Ϸ�����
/*
char getLegalInput(char* legalchar) {
	char a;
	int length = 0;
	while ((int)legalchar[length] >= 0) {//��ȡlegalchar�ĳ���
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

	cout << "���ڶ�ȡ�ļ�..." << endl;
	ReadFile(U, A);
	system("pause");
	system("cls");

	while (1) {
		system("cls");
		cout << "**���˵�**" << endl;
		cout << "1.�û�" << endl;
		cout << "2.�����¼" << endl;
		cout << "3.����" << endl;
		cout << "4.�˵�����" << endl;
		cout << "5.���ȫ����Ϣ" << endl;
		cout << "0.�˳�ϵͳ" << endl;

		char n = _getch();

		switch (n) {

			//����û�
		case '1': {
			int i = 1;
			while (i) {
				;
				system("cls");
				cout << "1.����û���" << endl;
				cout << "2.�鿴�û���" << endl;
				cout << "3.�޸��û���" << endl;
				cout << "����q������һ��" << endl;

				char n = _getch();
				switch (n) {
				case '1': {
					system("cls");
					cout << "����û�����...����q������һ��" << endl;
					cout << "�������û�����:";
					string str;
					cin >> str;
					if (str == "q")break;

					cout << endl << "�Ƿ񱣴�?\t1.����\t2.������" << endl;

					int i = 1;//����Ƿ�����ģ��(���ֻ�������)
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
						cout << "�޿����û�����������û�" << endl;
						system("pause");
						break;
					}
					cout << "�����û���:" << endl;
					ShowUserName(U);
					system("pause");
					break;

				case '3': {

				changeUser:
					system("cls");
					if (UserListLength(U) == 0) {
						cout << "�޿����û�����������û�" << endl;
						system("pause");
						break;
					}
					cout << "�޸��û�����...����q������һ��" << endl;
					cout << "�����û���:" << endl;
					ShowUserName(U);
					cout << "��������Ҫ�޸ĵ��û���:";
					int UserID;
					string str;
					cin >> str;
					if (str == "q")break;
					if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//���븶�����������е�λ��
						UserID = atoi(str.c_str());
					}
					else {
						if (IndexUserName(U, str) != ERROR)
							UserID = IndexUserName(U, str);
						else {
							cout << "�Ҳ������û�" << endl;
							system("pause");
							goto changeUser;
						}
					}

					system("cls");
					cout << "��������ĺ���û���:";
					cin >> str;

					cout << endl << "�Ƿ񱣴�?\t1.����\t2.������" << endl;

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

			//��Ӹ����¼
		case '2': {
			int i = 1;
			while (i) {
				system("cls");
				cout << "1.��Ӹ����¼" << endl;
				cout << "2.�鿴�����¼" << endl;
				cout << "3.ɾ�������¼" << endl;
				cout << "4.�鿴��ʷ�����¼(������ɾ����)" << endl;
				cout << "0.��ո����¼(���ɻָ�)" << endl;
				cout << "����q������һ��" << endl;

				char n = _getch();
				switch (n) {

				case '1': {
					system("cls");
					string str;
					AccountPtr Aptr = new Account();
					if (UserListLength(U) == 0) {
						cout << "�޿����û�����������û�" << endl;
						system("pause");
						break;
					}

					//���Ӹ�����
				addPayer:
					system("cls");
					cout << "��Ӹ����¼��...����q������һ��" << endl;
					cout << "���ø�����:" << endl;
					ShowUserName(U);
					cout << "��ѡ�񸶿���:";

					cin >> str;
					if (str == "q")break;
					if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//���븶�����������е�λ��
						Aptr->PayerID = atoi(str.c_str());
					}
					else {
						if (IndexUserName(U, str) != ERROR)
							Aptr->PayerID = IndexUserName(U, str);
						else {
							cout << "�Ҳ������û�" << endl;
							system("pause");
							goto addPayer;
						}
					}

					//���ӱ�������
				addBePaid:
					system("cls");
					cout << "��Ӹ����¼��...����q������һ��" << endl;
					cout << "���ñ�������:" << endl;
					cout << "0.��ǰ������\t";
					ShowUserName(U);
					cout << "��ѡ�񱻸�����:";

					cin >> str;
					if (str == "q")break;

					if (str.length() == 1 && str[0] == '0')//����0ʱ����������
						Aptr->BePaidID = -UserListLength(U);
					else {
						if (atoi(str.c_str()) > 0 && atoi(str.c_str()) <= UserListLength(U)) {//���븶�����������е�λ��
							Aptr->BePaidID = atoi(str.c_str());
						}
						else {
							if (IndexUserName(U, str) != ERROR)
								Aptr->BePaidID = IndexUserName(U, str);
							else {
								cout << "�Ҳ������û�" << endl;
								system("pause");
								goto addBePaid;
							}
						}
					}

					//���ӽ��
				addMoney:
					system("cls");
					cout << "�����븶����:";
					int money;
					cin >> money;
					if (cin.fail()) {
						cin.clear(); cin.sync();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						goto addMoney;
					}
					Aptr->money = money;

					//���ӱ�ע
					system("cls");
					cout << "�����뱸ע:";
					cin >> Aptr->ps;

					system("cls");
					cout << "������Ϣ¼�����" << endl;
					cout << "������Ϣ����:" << endl;
					cout << GetUserName(U, Aptr->PayerID) << " Ϊ " << GetUserName(U, Aptr->BePaidID) << " ֧�� " << Aptr->money << " Ԫ" << endl;
					cout << "ps:" << Aptr->ps << endl << endl;

					cout << endl << "�Ƿ񱣴�?\t1.����\t2.������" << endl;

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
						cout << "�޸����¼��������Ӹ����¼" << endl;
						system("pause");
						break;
					}

					cout << "������Ч�����¼����:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//�˵��Ĵ���
					while (Aptr->next) {
						Aptr = Aptr->next;
						if (Aptr->isEffective)
							cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\tΪ   " << GetUserName(U, Aptr->BePaidID) << "\t֧�� " << Aptr->money << " Ԫ\t" << Aptr->time << "  " << Aptr->ps << endl;
					}
					if (local == 0) {
						system("cls");
						cout << "�޸����¼��������Ӹ����¼" << endl;
					}
					system("pause");
				}break;

				case '3': {
					system("cls");
					if (A->next == NULL) {
						cout << "�޸����¼��������Ӹ����¼" << endl;
						system("pause");
						break;
					}

					cout << "������Ч�����¼����:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//�˵��Ĵ���
					while (Aptr->next) {
						Aptr = Aptr->next;
						if (Aptr->isEffective)
							cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\tΪ   " << GetUserName(U, Aptr->BePaidID) << "\t֧�� " << Aptr->money << " Ԫ\t" << Aptr->time << "  " << Aptr->ps << endl;
					}
					if (local == 0) {
						system("cls");
						cout << "�޸����¼��������Ӹ����¼" << endl;
						system("pause");
						break;
					}

					cout << endl << "ɾ�������¼��...����0������һ��" << endl;
					cout << "������Ҫɾ���ĸ����¼�����:";
					int order;

				reset:
					cin >> order;
					if (order == 0)break;
					if (cin.fail() || order > local) {
						cin.clear(); cin.sync();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						goto reset;
					}

					cout << endl << "�Ƿ񱣴�?\t1.����\t2.������" << endl;

					int i = 1;
					while (i) {
						char save = _getch();
						switch (save) {
						case '1':
							Aptr = A;
							for (int i = 0; i < order; i++) {
								Aptr = Aptr->next;
								if (!Aptr->isEffective)//��Ч������
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
						cout << "�޸����¼��������Ӹ����¼" << endl;
						system("pause");
						break;
					}

					cout << "������ʷ�����¼(������ɾ����)����:" << endl;
					AccountPtr Aptr = A;
					int local = 0;//�˵��Ĵ���
					while (Aptr->next) {
						Aptr = Aptr->next;
						cout << ++local << "    " << GetUserName(U, Aptr->PayerID) << "\tΪ   " << GetUserName(U, Aptr->BePaidID) << "\t֧�� " << Aptr->money << " Ԫ\t" << Aptr->time << "  " << Aptr->ps;
						if (!Aptr->isEffective)
							cout << " (��ɾ��)";
						cout << endl;
					}
					system("pause");
				}break;

				case '0': {
					system("cls");
					if (A->next == NULL) {
						cout << "�޸����¼��������Ӹ����¼" << endl;
						system("pause");
						break;
					}

					cout << "ȷ����ո����¼?(�޷��ָ�)\t1.���\t2.�����" << endl;
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
			if (CreateRefundList(U) == ERROR)//����Refund��������ERRORʱ���û�
				return 0;
			if (EnRefundList(U, A) == ERROR)//д��Refund��������ERRORʱ�޸����¼���˴�������Refund����
				return 0;
			
			cout << "P2P(��Ե�)����:(����Ϊÿ������֮���Ƿ��)" << endl;
			UserPtr Urear = U;
			int x = 0;//���ڴ洢�������е�λ��
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0)
						cout << GetUserName(U, x) << " �軹 " << GetUserName(U, y) << " " << Rrear->money << " Ԫ"<<endl;
				}
			}
			//cout << U->next->refund->next->money;//��һ��User�ĵ�һ��Refund
			//cout << U->next->next->next->next->refund->next->next->next->next->money;
			
			cout << endl;
			cout << "��һ���򻯺�Ľ���:(Ŀ��ʹ��������֧�����������ܵ���)" << endl;
			Urear = U;
			x = 0;//���ڴ洢�������е�λ��
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0) {//������(x,y)(��x�е�y��)����Ƿ��ʱ
						//����Ƿ񹹳�A->B->C�Ļ���˳��
						for (int i = 1; i <= UserListLength(U); i++) {//���ҵ�y�е�i���Ƿ����乹��x->y->i�Ļ���˳��iӦ������x��y(������Ҳ������)
							if (ReadRefundList(U, y, i) != 0) {
								double sub = Rrear->money - ReadRefundList(U, y, i);
								if (sub >= 0) {//x��y����y��Ҫ��i�ģ��ٻ�����y
									WriteRefundList(U, x, y, sub);
									WriteRefundList(U, x, i, ReadRefundList(U, y, i) + ReadRefundList(U, x, i));//x��y��һ���ֻ�i��Ǯ������xԭ������Ҫ��i
									WriteRefundList(U, y, i, 0);//x��y����i��y������Ҫ��i
								}
								else {//x��y��Ƿy�Ĳ��ָ�i��ʣ�ಿ����y��
									WriteRefundList(U, y, i, -sub);
									WriteRefundList(U, x, i, ReadRefundList(U, x, y) + ReadRefundList(U, x, i));
									WriteRefundList(U, x, y, 0);//x��y��һ����i��x������Ҫ��y
								}
							}
						}
					}
				}
			}
			Urear = U;
			x = 0;//���ڴ洢�������е�λ��
			while (Urear->next) {
				Urear = Urear->next;
				x++;
				RefundPtr Rrear = Urear->refund;
				int y = 0;
				while (Rrear->next) {
					Rrear = Rrear->next;
					y++;
					if (Rrear->money != 0)
						cout << GetUserName(U, x) << " �軹 " << GetUserName(U, y) << " " << Rrear->money << " Ԫ" << endl;
				}
			}
			system("pause");
		}break;


		case '4': {
			system("cls");
			cout << "�˵�����:" << endl << endl;
			int UserCount=UserListLength(U);//��ȡ�û�����

			cout << "����" << UserCount << "���û�" << endl;
			cout << "�û���Ϊ:";
			ShowUserName(U);
			cout << endl;

			AccountPtr Aptr = A;
			double AllPay = 0;//�ܸ���
			double AAPay = 0;//AA���ܸ���
			while (Aptr->next) {
				Aptr = Aptr->next;
				
				AllPay = AllPay + Aptr->money;
				if (Aptr->BePaidID < 0) {
					AAPay = AAPay + Aptr->money;
				}
			}
			cout << "�ܸ������:" << endl;
			cout << "�ܸ���:" << AllPay << endl;
			cout << "ƽ���ܸ���:" << AllPay / UserCount << endl;

			cout << endl;
			cout << "AA�������:(����Ϊ���˸���ļ�¼)" << endl;
			cout << "AA�ܸ���:" << AAPay << endl;
			cout << "AAƽ������:" << AAPay / UserCount << endl;

			cout << endl;
			cout << "���˸������:" << endl;
			for (int i = 1; i <= UserCount; i++) {//�ֱ����ÿ���û����˵�����
				Aptr = A;
				double PayMoney = 0;
				while (Aptr->next) {
					Aptr = Aptr->next;
					if (Aptr->PayerID == i) {//������Ϊ���ڲ��ҵĸ�����id
						PayMoney = PayMoney + Aptr->money;
					}
				}
				cout << GetUserName(U, i) << "��֧��" << PayMoney << "Ԫ" << endl;
			}
			system("pause");
		}break;


		case '5': {
			system("cls");
			cout << "ȷ�����ȫ����¼?(�޷��ָ�)\t1.���\t2.�����" << endl;
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

//Refund��������
int ClearRefundList(UserPtr &userHead) {//ɾ��Refund����
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

int CreateRefundList(UserPtr &userHead) {//��User���ɿյ�Enlist����
	ClearRefundList(userHead);//���������Ѵ��ڵ�Refund����

	UserPtr Urear = userHead;

	int UserCount = UserListLength(userHead);
	
	if (UserCount == 0) {
		system("cls");
		cout << "�޿����û�����������û�" << endl;
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

double ReadRefundList(UserPtr &userHead,int x, int y) {//����Refund�����е�x��User�ĵ�y���˵�
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

int WriteRefundList(UserPtr &userHead, int x, int y, double Value) {//��Refund�����е�x��User�ĵ�y���˵���д��ֵValue
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

int autoWriteRefund(UserPtr &userHead, int x, int y, double Value) {//�Զ�д�����Ƚ϶Է����Լ��Ƿ���Ƿ�����Զ������Լ��鿴�Լ��Ƿ���Ƿ��
	if (ReadRefundList(userHead, y, x) != 0) {//y��x��Ƿ��ʱ
		double sub = Value - ReadRefundList(userHead, y, x);//��ֵ
		if (sub >= 0) {//xǷy����yǷx��Ӧɾ��yǷx��������ֵ����xǷy
			WriteRefundList(userHead, y, x, 0);
			WriteRefundList(userHead, x, y, sub);
		}
		else {//yǷx����xǷy
			WriteRefundList(userHead, y, x, -sub);
		}
	}
	else {
		if (ReadRefundList(userHead, x, y) != 0) {//x��y��Ƿ��ʱ
			double add = Value + ReadRefundList(userHead, x, y);
			WriteRefundList(userHead, x, y, add);
		}
		else//x��y����Ƿ��
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
		cout << "����Ч�����¼��������Ӹ����¼" << endl;
		return ERROR;
	}
	Arear = accountHead;
	while (Arear->next) {
		Arear = Arear->next;
		if (Arear->isEffective == 1) {
			if (Arear->BePaidID > 0) {//Ϊ���˸���ʱ
				autoWriteRefund(userHead, Arear->BePaidID, Arear->PayerID, Arear->money);//д�뱻�����ߵ�User������
			}
			if (Arear->BePaidID < 0) {//Ϊ�����˸���
				for (int i = 1; i <= -Arear->BePaidID; i++) {
					if(i!=Arear->PayerID)//��ȥΪ�Լ���������
						autoWriteRefund(userHead, i, Arear->PayerID, Arear->money / -Arear->BePaidID);
				}
			}
		}
	}
	return OK;
}

//User��������
int CreateUserList(UserPtr &userHead) {				//��������
	userHead = (UserPtr)malloc(sizeof(User));
	if (!userHead) return ERROR;
	userHead->next = NULL;
	return OK;
}

int EnUserList(UserPtr &userHead, string UserName) {		//���һ���û�
	UserPtr Urear = userHead;
	while (Urear->next) {
		Urear = Urear->next;
	}

	UserPtr U = new User();			//���ṹ�庬��stringʱ������malloc
	U->UserName = UserName;
	U->next = NULL;

	Urear->next = U;
	return OK;
}

int UserListLength(UserPtr &userHead) {//����User������Ԫ�صĸ���
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

string GetUserName(UserPtr &userHead, int local) {//���������������е�λ�÷�������
	if (local < 0)
		return "������";
	if (local > 0 && local <= UserListLength(userHead)) {
		UserPtr Urear = userHead;
		for (int i = 0; i < local; i++) {
			Urear = Urear->next;
		}
		return Urear->UserName;
	}
	return "";
}

//Account��������
int CreateAccountList(AccountPtr &accountHead) {				//��������
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

int ClearAccountList(AccountPtr &accountHead) {//���Account
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


//��ȡϵͳʱ��
string getSystemTime() {
	time_t now;
	struct tm *fmt = new tm[1];
	time(&now);
	localtime_s(fmt, &now);
	//cout << fmt->tm_hour << ":" << fmt->tm_min << ":" << fmt->tm_sec;
	string year;
	year = to_string(fmt->tm_year + 1900);//tm_yearΪ1900��ʼ��������Ӧ��1900
	string month;
	month = to_string(fmt->tm_mon + 1);//�·ݷ�ΧΪ0~11��Ӧ�ֶ���1
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

//���ļ�
int GetSpacebarCount() {
	fstream readFile("basedata.txt", ifstream::in);
	int i = 0;//���ڼ�¼�ո���ִ���
	char a;
	if (readFile.get() == '\n') {//�����ݵ����(��ͷ���ǿո�)
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
	return i;//����Ԫ�ظ���
}

int ReadFile(UserPtr& U, AccountPtr& A) {
	fstream readFile("basedata.txt", ifstream::in);

	if (readFile.fail()) {//�ļ�������ʱ
		readFile.close();
		fstream writeFile("basedata.txt", ifstream::out);
		writeFile << '\n';
		writeFile.close();
	}

	if (GetSpacebarCount() == ERROR) {//���û���ʱ
		cout << "�޿����û�����������û���" << endl;

		//���
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

	cout << "�û���ȡ��� (��" << GetSpacebarCount() << "���û�)" << endl;
	readFile.get();//ȥ��������' '
	readFile.get();//ȥ��������'\n'
				   //readFile.getline(new char[0], 100, '\n');//ȥ��������'\n'

	if (readFile.get() == -1) {//�������ļ�ĩβ��get -1
		cout << "�޸����¼" << endl;
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

		readFile.get();//ȥ��������' '
		readFile.get();//ȥ��������'\n'
	} while (readFile.get() != -1);
	if (count == 0)
		cout << "�޸����¼" << endl;
	else
		cout << "�����¼��ȡ��� (��" << count << "����Ч�����¼)" << endl;

	readFile.close();
	return OK;
}

//д�ļ�
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
