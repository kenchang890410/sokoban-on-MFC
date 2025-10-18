
// SokobanView.cpp: CSokobanView 類別的實作
//
#include "pch.h"
#include "framework.h"
#include <sstream>
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Sokoban.h"
#endif
#include<iostream>
#include <ctime>
#include "SokobanDoc.h"
#include "SokobanView.h"
#include<string>
#include<cstring>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
// CSokobanView
IMPLEMENT_DYNCREATE(CSokobanView, CView)

BEGIN_MESSAGE_MAP(CSokobanView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CSokobanView 建構/解構

CSokobanView::CSokobanView() noexcept
{
	// TODO: 在此加入建構程式碼

}

CSokobanView::~CSokobanView()
{
}

BOOL CSokobanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CSokobanView 繪圖
void CSokobanView::OnDraw(CDC* pDC)
{
	CSokobanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	readMap(level);
	showMap();
	print_state();
	pDC->TextOut(50, column * 32+ 50, L"按WASD或HJKL可控制worker方向。");
	pDC->TextOut(50, column * 32 + 70, L"或是在worker的上下左右點擊滑鼠左鍵進行移動。");
	// TODO: 在此加入原生資料的描繪程式碼
}


// CSokobanView 列印

BOOL CSokobanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CSokobanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CSokobanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CSokobanView 診斷

#ifdef _DEBUG
void CSokobanView::AssertValid() const
{
	CView::AssertValid();
}

void CSokobanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSokobanDoc* CSokobanView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSokobanDoc)));
	return (CSokobanDoc*)m_pDocument;
}
#endif //_DEBUG

// CSokobanView 訊息處理常式

//CSokobanView
void CSokobanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	print_state();
	switch (nChar)
	{
	case 'W':
		ControlWorker(UP);
		break;
	case 'K':
		ControlWorker(UP);
		break;
	case 'S':
		ControlWorker(DOWN);
		break;
	case 'J':
		ControlWorker(DOWN);
		break;
	case 'A':
		ControlWorker(LEFT);
		break;
	case 'H':
		ControlWorker(LEFT);
		break;
	case 'D':
		ControlWorker(RIGHT);
		break;
	case 'L':
		ControlWorker(RIGHT);
		break;
	default:
		break;
	}
	print_state();
	//if all box get into destination or box number bigger then destination number go to next level
	if (arrival == dest_number || box_number > dest_number)
	{
		AfxMessageBox(L"Pass!\nPress SPACE to next level", MB_OK);
		level++;
		RedrawWindow();
	}
}

void CSokobanView::change_size_and_print(int x, int y)
{
	// TODO: 請在此新增您的實作程式碼.
	CClientDC aDC(this);
	CBitmap wallBitmap, workerBitmap, boxBitmap, destBitmap, arrivalBitmap, blankBitmap;//get bitmap
	wallBitmap.LoadBitmap(IDB_BITMAP1);
	workerBitmap.LoadBitmap(IDB_BITMAP2);
	boxBitmap.LoadBitmap(IDB_BITMAP3);
	destBitmap.LoadBitmap(IDB_BITMAP4);
	arrivalBitmap.LoadBitmap(IDB_BITMAP5);
	blankBitmap.LoadBitmap(IDB_BITMAP6);
	CDC memDC;
	memDC.CreateCompatibleDC(&aDC);
	switch (map[x][y])
	{
	case 'H':
		memDC.SelectObject(&wallBitmap);
		break;
	case 'B':
		memDC.SelectObject(&boxBitmap);
		if (dest_record[x][y]) //  box get into destination change bitmap
		{
			memDC.SelectObject(&arrivalBitmap);
		}
		break;
	case 'W':
		memDC.SelectObject(&workerBitmap);
		break;
	default:
		if (dest_record[x][y])
		{
			memDC.SelectObject(&destBitmap);
		}
		else
		{
			memDC.SelectObject(&blankBitmap);
		}
		break;
	}
	aDC.BitBlt(y * 32, x * 32, 200, 200, &memDC, 0, 0, SRCCOPY);//print bitmap
	
}


void CSokobanView::readMap(int l)
{
	// TODO: 請在此新增您的實作程式碼.
	char filename[11], tmp_map[80];
	sprintf(filename, "map%03d.txt", l);//store level to filename:map???.txt
	FILE* map_data;//open file
	map_data = fopen(filename, "r");//read file
	dest_number = 0;//init information
	arrival = 0;
	step = 0;
	row = 0;
	column = 0;
	box_number = 0;
	memset(map, ' ', sizeof(map));//init map with ' '
	memset(dest_record, 0, sizeof(dest_record));//init dest_record with 0
	while (fgets(tmp_map, 80, map_data))//read line untill end
	{
		int len = strlen(tmp_map);
		if (len > row)//record max row
			row = len;
		for (int i = 0; i < len; i++)
		{
			char c = tmp_map[i];
			switch (c)
			{
			case 'H':
				map[column][i] = tmp_map[i];//store map.txt to map[][]
				dest_record[column][i] = 0;//record char is destination or not
				break;
			case 'B':
				map[column][i] = tmp_map[i];
				dest_record[column][i] = 0;
				box_number++;
				break;
			case 'W':
				map[column][i] = tmp_map[i];
				dest_record[column][i] = 0;
				workerX = i;
				workerY = column;
				break;
			case 'D':
				map[column][i] = ' ';
				dest_record[column][i] = 1;
				dest_number++;
				break;
			case 'C':
				map[column][i] = 'B';
				dest_record[column][i] = 1;
				dest_number++;
				arrival++;
				break;
			default:
				break;
			}
		}
		column++;//record max column
	}
	fclose(map_data);//close file
}


void CSokobanView::showMap()
{
	// TODO: 請在此新增您的實作程式碼.
	CClientDC aDC(this);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			change_size_and_print(j, i);//change size and print map
		}
	}
}


char CSokobanView::NextToWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	switch (d)
	{
	case LEFT:
		return map[workerY][workerX - 1];//return nexttoworker in map
		break;
	case UP:
		return map[workerY - 1][workerX];
		break;
	case DOWN:
		return map[workerY + 1][workerX];
		break;
	case RIGHT:
		return map[workerY][workerX + 1];
		break;
	}
}


char CSokobanView::NextNextToWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	switch (d)
	{
	case UP:
		return map[workerY - 2][workerX];//return nextnexttoworker in map
		break;
	case DOWN:
		return map[workerY + 2][workerX];
		break;
	case LEFT:
		return map[workerY][workerX - 2];
		break;
	case RIGHT:
		return map[workerY][workerX + 2];
		break;
	}
}


void CSokobanView::MoveWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	map[workerY][workerX] = ' ';//clear worker current location
	change_size_and_print(workerY, workerX);
	switch (d)
	{
	case UP:
		workerY--;
		break;
	case DOWN:
		workerY++;
		break;
	case LEFT:
		workerX--;
		break;
	case RIGHT:
		workerX++;
		break;
	}
	map[workerY][workerX] = 'W';//print worker after move 
	change_size_and_print(workerY, workerX);
	step++;//while worker move step+1
}


void CSokobanView::MoveBox(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	switch (d)
	{
	case LEFT:
		map[workerY][workerX - 2] = map[workerY][workerX - 1];//print box after move 
		change_size_and_print(workerY, workerX - 2);
		map[workerY][workerX - 1] = ' ';//clear box before move
		if (dest_record[workerY][workerX - 1])//if box leave destination arrival-1
			arrival--;
		if (dest_record[workerY][workerX - 2])//if box go into destination arrival-1
			arrival++;
		break;
	case UP:
		map[workerY - 2][workerX] = map[workerY - 1][workerX];
		change_size_and_print(workerY - 2, workerX);
		map[workerY - 1][workerX] = ' ';
		if (dest_record[workerY - 1][workerX])
			arrival--;
		if (dest_record[workerY - 2][workerX])
			arrival++;
		break;
	case DOWN:
		map[workerY + 2][workerX] = map[workerY + 1][workerX];
		change_size_and_print(workerY + 2, workerX);
		map[workerY + 1][workerX] = ' ';
		if (dest_record[workerY + 1][workerX])
			arrival--;
		if (dest_record[workerY + 2][workerX])
			arrival++;
		break;
	case RIGHT:
		map[workerY][workerX + 2] = map[workerY][workerX + 1];
		change_size_and_print(workerY, workerX + 2);
		map[workerY][workerX + 1] = ' ';
		if (dest_record[workerY][workerX + 1])
			arrival--;
		if (dest_record[workerY][workerX + 2])
			arrival++;
		break;
	default:
		break;
	}
}


int CSokobanView::ControlWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	char c = NextToWorker(d);
	if (c == ' ')//if nexttoworker is nothing there 
	{
		MoveWorker(d);
	}
	else
	{
		if (c == 'B' && NextNextToWorker(d) == ' ')//if nextnexttoworker is nothing there and nexttoworker is box
		{
			MoveBox(d);
			MoveWorker(d);
		}
	}
	return 0;
}

void CSokobanView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CClientDC aDC(this);
	CString s;
	s.Format(L"(%d,%d) ", point.x, point.y);//get click
	print_state();
	if ((point.x)  < (workerX)*32 && (point.y)  > workerY* 32 && point.y  < (workerY+1)*32)//if Click to the left  of the worker
		ControlWorker(LEFT);
	else if (point.x  > (workerX+1)*32 && (point.y) > workerY * 32 && point.y < (workerY + 1) * 32)//if Click to the right  of the worker
		ControlWorker(RIGHT);
	else if ((point.y)  < (workerY)*32 && (point.x)  > workerX*32 && point.x  < (workerX+1)*32)//if Click to the above  of the worker
		ControlWorker(UP);
	else if (point.y  > (workerY+1)*32 && (point.x) > workerX * 32 && point.x < (workerX + 1) * 32)//if Click to the below of the worker 
		ControlWorker(DOWN);
	print_state();
	//if all box get into destination or box number bigger then destination number go to next level
	if (arrival == dest_number|| box_number > dest_number)
	{
		AfxMessageBox(L"Pass!\nPress SPACE to next level", MB_OK);
		level++;
		RedrawWindow();
	}
}


void CSokobanView::print_state()//for print information
{
	// TODO: 請在此新增您的實作程式碼.
	CClientDC aDC(this);
	string strlevel, strdest, strarr, strstep;//change int to CString 
	stringstream ss, ss1, ss2, ss3;
	ss << level;
	ss >> strlevel;
	ss1 << dest_number;
	ss1 >> strdest;
	ss2 << arrival;
	ss2 >> strarr;
	ss3 << step;
	ss3 >> strstep;
	CString sstrlevel(strlevel.c_str(), strlevel.length());
	CString sstrdest(strdest.c_str(), strdest.length());
	CString sstrarr(strarr.c_str(), strarr.length());
	CString sstrstep(strstep.c_str(), strstep.length());
	aDC.TextOut(row * 32 + 50, 50, L"level: ");
	aDC.TextOut(row * 32 + 100, 50, sstrlevel);
	aDC.TextOut(row * 32 + 50, 100, L"destination:");
	aDC.TextOut(row * 32 + 150, 100, sstrdest);
	aDC.TextOut(row * 32 + 50, 150, L"arrival: ");
	aDC.TextOut(row * 32 + 110, 150, sstrarr);
	aDC.TextOut(row * 32 + 50, 200, L"step: ");
	aDC.TextOut(row * 32 + 90, 200, sstrstep);
}
