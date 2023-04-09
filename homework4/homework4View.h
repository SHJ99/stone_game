
// homework4View.h: Chomework4View 클래스의 인터페이스
//

#pragma once
#include<iostream>
#include <string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<cmath>
#include <windows.h>
#include <stdio.h>

using namespace std;

class Chomework4View : public CView
{
protected: // serialization에서만 만들어집니다.
	Chomework4View() noexcept;
	DECLARE_DYNCREATE(Chomework4View)

// 특성입니다.
public:
	Chomework4Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~Chomework4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	vector<CPoint> ball;

	int BlackNum;
	int WhiteNum;
	int openfile;

	int bd[11][11] = { 0, }; // 2차원 배열 
	int BallCount();

//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
	int SearchBoard();
	afx_msg void Onmysave();
	afx_msg void Onmyopen();
};

#ifndef _DEBUG  // homework4View.cpp의 디버그 버전
inline Chomework4Doc* Chomework4View::GetDocument() const
   { return reinterpret_cast<Chomework4Doc*>(m_pDocument); }
#endif

