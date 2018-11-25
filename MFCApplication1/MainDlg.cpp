
// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MainDlg.h"
#include "afxdialogex.h"


#include <GL\glew.h>
#include <GL\freeglut.h>

#include<iostream>
#include <string> 
#include <sstream>
#include <gl\GLU.h>


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMaimDlg dialog



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, text1(_T(""))
	, text2(_T(""))
	, text3(_T(""))
	, text4(_T(""))
	, in(_T(""))
	, num_error()
	, text_error(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, text1);
	DDX_Text(pDX, IDC_EDIT2, text2);
	DDX_Text(pDX, IDC_EDIT3, text3);
	DDX_Text(pDX, IDC_EDIT4, text4);
	DDX_Text(pDX, IDC_EDIT5, in);
	DDX_Text(pDX, IDC_EDIT6, num_error);
	DDX_Text(pDX, IDC_EDIT7, text_error);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDlg::OnBnClickedButton3)
	
END_MESSAGE_MAP()


// CMaimDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int countOnes(int x)
{
	int k = 0;
	while (x != 0)
	{
		k++;
		x &= x - 1;
	}
	return k;
}

int inv16(int a)
{
	int b = 0;

	for (int i = 0; i < 16; i++)
	{
		b = b << 1;
		b += a % 2;
		a = a >> 1;
	}
	return b;
}

//Кодування 15 біт на вхід вхідне слово
// ПОВЕРТАЄ закодоване слово
// !!!!! ВАЖНО !!!!! для використання без шифрування  return rez; 
int cod16(int in1)
{
	in1 = (in1 << 4);
	int num = in1;
	int tmp = num;
	int ygric = in1;
	int rez = 0;
	int coun = 0;
	bool flg = 0;

	for (int i = 0; i<15; i++)
	{
		bool f = false;
		int a = 0, b = 0;

		ygric = tmp;
		tmp <<= 1;        // здвигаємо S
		num = tmp;

		flg = (num & 0b1000000000000000);
		ygric &= 0b0001111000000000000000;

		ygric &= 0b1100000000000000000;
		coun = countOnes(ygric);
		if (coun == 1) {   // множення С на S 
			a = 1;
			f = true;
		}
		if (flg) {      // множення D на U
			b = 1;
			f = true;
		}
		rez <<= 1; // утворюємо вихідне слово (здвих для формування кожного окремого біту)
		if (f) {
			rez += (a^b);  // рахуємо вижідне слово 
		}
	}
	return inv16(rez) >> 1;
	//return rez;
}

int decod16(int in1)
{
	int num = in1;
	int tmp = num;
	int ygric = in1;
	int rez = 0;
	int coun = 0;
	bool flg = 0;
	bool flg1 = 0;

	for (int i = 0; i<15; i++)
	{
		bool f = false;
		int a = 0, b = 0;


		ygric = tmp;
		tmp <<= 1;
		num = tmp;

		/// відмінність від кодера  ПОПРАВКА
		flg1 = (tmp & 0b10000000000000000000);
		if (flg1)
			tmp = tmp ^ 0b0011000000000000000;
		///

		flg = (num & 0b1000000000000000);
		ygric &= 0b0001111000000000000000;

		ygric &= 0b1100000000000000000;
		coun = countOnes(ygric);
		if (coun == 1) {
			a = 1;
			f = true;
		}
		if (flg) {
			b = 1;
			f = true;
		}
		rez <<= 1;
		if (f) {
			rez += (a^b);
		}
	}
	return rez >> 4;
}

// ШИВРУВАННЯ 16 бітне секретне слово і 15 бітів які будуть шифруватися
// ПОВЕРТАЄ шивроване слово
int shufr16(int sek_sl, int in_sl)
{
	sek_sl = inv16(sek_sl);
	bool flag = 0, flag1 = 0;
	//int polinom = 0b11010001;
	int polinom = 0b1010111010001;
	for (int i = 0; i < 15; i++)
	{
		flag = (sek_sl & 0b1000000000000000);
		sek_sl <<= 1;
		flag1 = (in_sl & 0b100000000000000);
		in_sl <<= 1;
		if (flag)
			sek_sl ^= polinom;
		if (flag1)
			sek_sl ^= 0b0000001;
		sek_sl &= 0xffff;
	}
	return sek_sl;
}


// дешифрування на вхід приходить шифроване слово 
// ПОВЕРТАЄ КЛЮЧ
int deshufr16(int in)
{
	int mas[16]{ 2, 4, 8, 16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,5585 };
	bool flag = 0;
	bool flag1 = 0;
	int rez = 0;

	// утворюємо матрицю А^15
	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 16; i++)
		{
			flag = (mas[i] & 0b1000000000000000);
			mas[i] <<= 1;
			if (flag) {
				mas[i] ^= 5585;
			}
			mas[i] &= 0xffff;
		}
	}
	// знаходимо ключ
	for (int i = 0; i < 16; i++)
	{
		flag1 = (in & 0b1000000000000000);
		in <<= 1;
		if (flag1) {
			rez ^= mas[i];
		}
	}
	rez &= 0xffff;
	int a = inv16(rez);
	return a;
}

int desh_key16(int in_word, int key)
{
	int a = inv16(in_word);
	a = a^key;

	//int a = inv(in_word);
	return a >>= 1;
}

// на вхід 2 числа int
// повертає кількість відмінних біт 
int porivn(int a, int b) 
{	
	int a1;
	int b1;
	int res = 0;
	for (size_t i = 0; i < 15; i++)
	{
		a1 = a;
		b1 = b;

		if ((a1 &= 0x1) != (b1 &= 0x1))
		{
			res++;
		}
		a >>= 1;
		b >>= 1;
	}
	return 15-res;
}


int sekret_word = 150;
void CMainDlg::OnBnClickedButton1()
{
		
	in.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"),
		mas[0],mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);
	
	for (size_t i = 0; i < num_cilk; i++)
	{
		mas_poch[i] = mas[i];
		mas[i] = cod16(mas[i]);

	}
	
	text1.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"),
		mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);

	for (size_t i = 0; i < num_cilk; i++)
	{
		mas[i] = shufr16(sekret_word, mas[i]);
	}
	 
	text2.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"),
		mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);
	err = GetDlgItemInt(IDC_EDIT6);
	num_error = err; // щоб не пропадало значення кількості помилок
	UpdateData(FALSE);
	
}


void display(int masznach[])
{
	glClear(GL_COLOR_BUFFER_BIT);
	

	float start1 = -0.8;
	float start2 = -0.75;
	
	/*
	for (int k = 0; k < 3; k++)
	{
		//float f = mas[i] / 10 - 1;
		//рисуем прямоугольник

		glRectf(-0.8, -0.9, -0.75, 0.5);
		//
		start1 += 0.1;
		start2 += 0.1;
	}
	*/
	glColor3ub(239, 211, 52);
	//1
	float f = (float)masznach[0] / 10 - 1;
	glRectf(start1, -0.9, start2, f);
	start1 += 0.15;
	start2 += 0.15;

	//2
	f = (float)masznach[1] / 10 - 1;
	glRectf(start1, -0.9, start2, f);
	
	start1 += 0.15;
	start2 += 0.15;

	//3
	f = (float)masznach[2] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;

	//4
	f = (float)masznach[3] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//5
	f = (float)masznach[4] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//6
	f = (float)masznach[5] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//7
	f = (float)masznach[6] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//8
	f = (float)masznach[7] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//9
	f = (float)masznach[8] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;
	//10
	f = (float)masznach[9] / 10 - 1;
	glRectf(start1, -0.9, start2, f);

	start1 += 0.15;
	start2 += 0.15;

	


	// осі
	glLineWidth(10); //товщина
	glColor3ub(0, 0, 0);

	glBegin(GL_LINES);
	glVertex2f(-0.9, -0.9);
	glVertex2f(0.9, -0.9);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-0.9, 0.9);
	glVertex2f(-0.9, -0.9);
	glEnd();

	glLineWidth(10); //товщина
	glColor3ub(0, 255, 0);

	
	glBegin(GL_LINES);
	glVertex2f(-0.9, 0.03);
	glVertex2f(0.9, 0.03);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-0.9, -0.43);
	glVertex2f(0.9, -0.43);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-0.9, 0.5);
	glVertex2f(0.9, 0.5);
	glEnd();

	glFlush();
}


void CMainDlg::OnBnClickedButton2()
{
	
	int argc = 1;
	char *argv[1] = { (char*)"Something" };
	glutInit(&argc, argv);
	//we initizlilze the glut. functions
	glutInitWindowSize(690, 480);
	glutInitWindowPosition(100, 140);
	glutCreateWindow("OpenGL window");
	glClearColor(1, 1, 1, 1);


	display(mas);

	//glutDisplayFunc(display);
	
	glutMainLoop();

}

void CMainDlg::OnBnClickedButton3()
{

	int key= deshufr16(sekret_word);

	for (size_t i = 0; i < num_cilk; i++)
	{
		mas[i] = desh_key16(mas[i], key);
	}
	text3.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"), 
		mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);
	

	for (size_t i = 0; i < num_cilk; i++)
	{
		for (size_t j = 0; j < err; j++)
		{
			mas[i] ^= (1 << (rand() % 15));
		}
	}
	text_error.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"),
		mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);

	for (size_t i = 0; i < num_cilk; i++)
	{
		mas[i] = decod16(mas[i]);
	}
	text4.Format(_T("%i, %i, %i, %i, %i, %i, %i, %i, %i, %i"),
		mas[0], mas[1], mas[2], mas[3], mas[4], mas[5], mas[6], mas[7], mas[8], mas[9]);
	

	for (size_t i = 0; i < num_cilk; i++)
	{
		mas[i] = porivn(mas[i], mas_poch[i]);
	}

	UpdateData(FALSE);
}
