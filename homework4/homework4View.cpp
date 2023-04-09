
// homework4View.cpp: Chomework4View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "homework4.h"
#endif

#include "homework4Doc.h"
#include "homework4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Chomework4View

IMPLEMENT_DYNCREATE(Chomework4View, CView)

BEGIN_MESSAGE_MAP(Chomework4View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(mysave, &Chomework4View::Onmysave)
	ON_COMMAND(myopen, &Chomework4View::Onmyopen)
END_MESSAGE_MAP()

// Chomework4View 생성/소멸

Chomework4View::Chomework4View() noexcept
{
	BlackNum = 0;
	WhiteNum = 0;
	openfile = 0;

	MessageBox(L"<오목 게임> \n 2인용 게임입니다.\n 흑돌과 백돌을 번갈아 놓습니다.\n 돌이 일렬로 5개가 되면 게임 종료.\n [저장하기] 및 [불러오기] 버튼으로 \n 복기가 가능합니다.\n(2020113569 안소연/2020110878 서호정 제작)", L"게임 방법");
	// TODO: 여기에 생성 코드를 추가합니다.

}

Chomework4View::~Chomework4View()
{
}

BOOL Chomework4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// Chomework4View 그리기

void Chomework4View::OnDraw(CDC* /*pDC*/)
{
	Chomework4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CClientDC dc(this);
	CRect rect;
	GetClientRect(rect);

	CBrush bk(RGB(255, 255, 255));
	dc.SelectObject(&bk);
	dc.Rectangle(0, 0, rect.right, rect.bottom);

	CBrush bc(RGB(243, 207, 152));
	dc.SelectObject(&bc);

	for (int i = 0; i < 10; i++) {
		dc.Rectangle(50, i * 49 + 50, 100, 100 + i * 49);
		for (int j = 1; j < 10; j++) {
			dc.Rectangle(50 + 49 * j, 50 + i * 49, 100 + 49 * j, 100 + i * 49);
		}
	}

	COLORREF hb[] = { RGB(0,0,0),RGB(255,255,255) };

	for (int i = 0; i < ball.size(); i++) {
		if (openfile)
			Sleep(1000);
		int x = ((ceil(ball[i].x / 10.0) * 10) - 50) / 50;
		int y = ((ceil(ball[i].y / 10.0) * 10) - 50) / 50;
		CBrush bs = hb[i % 2];
		dc.SelectObject(&bs);
		dc.Ellipse(ball[i].x - 20, ball[i].y - 20, ball[i].x + 20, ball[i].y + 20);
		CString str;
		str.Format(L"(%d,%d) %s", x, y, (i % 2 == 0) ? L"흑돌" : L"백돌");
		dc.TextOutW(rect.right - 200, 30 * (i + 1), str);
	}

	openfile = 0;

	SearchBoard();

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// Chomework4View 인쇄

BOOL Chomework4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void Chomework4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void Chomework4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// Chomework4View 진단

#ifdef _DEBUG
void Chomework4View::AssertValid() const
{
	CView::AssertValid();
}

void Chomework4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Chomework4Doc* Chomework4View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Chomework4Doc)));
	return (Chomework4Doc*)m_pDocument;
}
#endif //_DEBUG


// Chomework4View 메시지 처리기


void Chomework4View::OnLButtonDown(UINT nFlags, CPoint point)
{

	for (int i = 0; i < ball.size(); i++) {
		int x = ball[i].x - point.x;
		int y = ball[i].y - point.y;
		if (sqrt(x * x + y * y) <= 25) {
			MessageBox(L"해당 자리에 이미 바둑돌이 있으므로 착수가 불가합니다.", L"중복된 돌");
			return;
		}
	}

	if (point.x < 25 || point.y < 25 || point.x>550 || point.y>550) {
		MessageBox(L"범위를 벗어나 착수가 불가능합니다.", L"착수 불가");
		return;
	}

	int x = round(point.x / 50.0) * 50 - (point.x / 100.0) + 1;
	int y = round(point.y / 50.0) * 50 - (point.y / 100.0) + 1;
	int xboard = (ceil(x / 10.0) * 10 - 50) / 50;
	int yboard = (ceil(y / 10.0) * 10 - 50) / 50;

	bd[xboard][yboard] = ball.size() % 2 + 1; // 공백은 0, 흑돌은 1, 백돌은 2로 지정
	ball.push_back(CPoint(x, y));//1픽셀씩 겹치면 좌표도 그만큼 줄어듦
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

int Chomework4View::SearchBoard()
{
	if (ball.size()) {
		WhiteNum = 0, BlackNum = 0;
		int BlackWin = 0;
		int WhiteWin = 0;
		int now = ball.size() - 1; // 마지막 기준
		int sX = ((ceil(ball[now].x / 10.0) * 10) - 50) / 50;
		int sY = ((ceil(ball[now].y / 10.0) * 10) - 50) / 50;
		int k = (sX > sY) ? sX : sY;

		for (int i = 0; i < 11; i++) {
			if (BlackNum == 5 || WhiteNum == 5)
				now % 2 ? WhiteWin++ : BlackWin++;
			else if (BlackNum > 5 || WhiteNum > 5)
				now % 2 ? WhiteWin = 0 : BlackWin = 0;

			if (bd[sX][i] == now % 2 + 1) {
				now % 2 ? WhiteNum++ : BlackNum++;
			}
			else if (bd[sX][i] != now % 2 + 1) {
				now % 2 ? WhiteNum = 0 : BlackNum = 0;
			}
		}

		for (int i = 0; i < 11; i++) {
			if (BlackNum == 5 || WhiteNum == 5)
				now % 2 ? WhiteWin++ : BlackWin++;
			else if (BlackNum > 5 || WhiteNum > 5)
				now % 2 ? WhiteWin = 0 : BlackWin = 0;

			if (bd[sY][i] == now % 2 + 1) {
				now % 2 ? WhiteNum++ : BlackNum++;
			}
			else if (bd[sY][i] != now % 2 + 1) {
				now % 2 ? WhiteNum = 0 : BlackNum = 0;
			}
		}

		for (int i = 0; i < 10 - sX + k; i++) {
			if (BlackNum == 5 || WhiteNum == 5)
				now % 2 ? WhiteWin++ : BlackWin++;
			else if (BlackNum > 5 || WhiteNum > 5)
				now % 2 ? WhiteWin = 0 : BlackWin = 0;

			if (bd[sX - k + i][sY - k + i] == now % 2 + 1) {
				now % 2 ? WhiteNum++ : BlackNum++;
			}
			else if (bd[sX - k + i][sY - k + i] != now % 2 + 1) {
				now % 2 ? WhiteNum = 0 : BlackNum = 0;
			}
		}

		for (int i = 0; i <= sX + k; i++) {
			if (BlackNum == 5 || WhiteNum == 5)
				now % 2 ? WhiteWin++ : BlackWin++;
			else if (BlackNum > 5 || WhiteNum > 5)
				now % 2 ? WhiteWin = 0 : BlackWin = 0;

			if (bd[sX + k - i][sY - k + i] == now % 2 + 1) {
				now % 2 ? WhiteNum++ : BlackNum++;
			}
			else if (bd[sX + k - i][sY - k + i] != now % 2 + 1) {
				now % 2 ? WhiteNum = 0 : BlackNum = 0;
			}

			if (WhiteWin) {
				MessageBox(L"백돌 승리.", L"게임 종료");
				return 0;
			}
			if (BlackWin) {
				MessageBox(L"흑돌 승리.", L"게임 종료");
				return 0;
			}
		}
	}
}


void Chomework4View::Onmysave()
{
	ofstream fout("data.txt");
	fout << ball.size() << endl;
	for (int i = 0; i < ball.size(); i++) {
		fout << ball[i].x << "  " << ball[i].y << endl;
	}
	fout.close();
}


void Chomework4View::Onmyopen()
{
	ball.clear();
	ifstream fin("data.txt");
	int n;
	fin >> n;
	int x, y;
	for (int i = 0; i < n; i++) {
		fin >> x >> y;
		ball.push_back(CPoint(x, y));
	}
	fin.close();
	openfile++;
	Invalidate();
}
