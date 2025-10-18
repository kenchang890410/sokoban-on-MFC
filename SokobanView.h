
// SokobanView.h: CSokobanView 類別的介面
//

#pragma once


class CSokobanView : public CView
{
protected: // 僅從序列化建立
	CSokobanView() noexcept;
	DECLARE_DYNCREATE(CSokobanView)

// 屬性
public:
	CSokobanDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CSokobanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	enum Direction //for controlworker
	{
		LEFT,
		UP,
		DOWN,
		RIGHT
	};
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int x = 5;
	int y = 5;
	int c = 0;
	void change_size_and_print(int x, int y);
	void readMap(int l);
	void showMap();
	char NextToWorker(Direction d);
	char NextNextToWorker(Direction d);
	void MoveWorker(Direction d);
	void MoveBox(Direction d);
	int ControlWorker(Direction d);
	int nextlevel=0;
	int passlevel=0;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	int firstin=0;
	void print_state();
	int level=1;//init level
	int dest_number=0;//record how many destinations
	int box_number=0;
	int arrival;//record how many arrivals
	int step = 1; //record how many steps
	int row = 0; //record current row
	int column = 0; //record current column
	int workerY, workerX; //record location
	int dest_record[25][80]; //record map is destination or not
	char map[25][80]; //record map
};

#ifndef _DEBUG  // 對 SokobanView.cpp 中的版本進行偵錯
inline CSokobanDoc* CSokobanView::GetDocument() const
   { return reinterpret_cast<CSokobanDoc*>(m_pDocument); }
#endif

